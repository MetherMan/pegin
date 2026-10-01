"""Blue Moon colour concepts for warrior armour sets (preview only).

Recolours the native Golden (male ma_*_b003, female fe_*_b001) and Elf (ma_*_b008 / fe_*_b011)
sets: dark base → black-navy, bright surfaces → vivid blue, highlights → near-white blue.
Skin textures are left untouched. Writes assets/blue-moon-armor/<set>-bluemoon.json for
index.html?data=<set>-bluemoon.
"""
from pathlib import Path
from io import BytesIO
import json, sys

R = Path(__file__).resolve().parents[1]
sys.path[:0] = [str(R/'runtime/pylibs'), str(R/'client-overlay/Tools/SkillColors'), str(R/'tools')]
import numpy as np
from PIL import Image, ImageFilter
from native_assets import png_from_wtm
from tuning import resource_reader
import bluemoon_armor_decor as decor
import bluemoon_armor_ornaments as orn
import bluemoon_armor_kitbash as kitbash

O = R/'assets/blue-moon-armor'
SKIN = {'Girl_torso_0-1.wtm', 'Girl_leg_0-.wtm'}  # bare skin under the female sets
# single: one material per texture. Elf steel is grey/olive, so hue would split it inconsistently.
# tone: (Otsu threshold factor, dark base range, bright median target, bright floor). Elf uses a
# lighter split after the user found its first pass too dark; Golden keeps the approved values.
BASES = {'golden': dict(name='황금', models='남 b003 / 여 b001', single=False, tone=(1.0, (0, .33), .4, .4)),
         'elf': dict(name='엘프', models='남 b008 / 여 b011', single=True, tone=(.8, (.16, .4), .45, .42),
                     decorate=True)}
# Rank-9 decoration (elf): crescent + star on the chest and back centre line. (facing, height, radius)
EMBLEMS = {'ma_': [(-1, .64, .072), (1, .64, .072)], 'fe_': [(-1, .45, .05), (1, .5, .05)]}
# 3D crescent ornaments (model units): shoulder crests, raised chest medallion over the painted
# emblem (same radius and height), and a crescent floating behind the back plate.
ORNAMENTS = {'0': dict(shoulder=.075, chest=.072, back=.17, thickness=.02, chest_height=.64, back_height=.64),
             '1': dict(shoulder=.058, chest=.05, back=.12, thickness=.016, chest_height=.45, back_height=.5)}
# Third pass (user): dark parts black-navy, bright parts near-white blue, with a clear gap
# between them. Each texture's own luminance range is stretched (darkest 2% → black-navy,
# brightest 2% → near white) and pushed apart with an S-curve; the variants differ in strength.
METAL = [(0, '#02040c'), (.2, '#060b26'), (.38, '#0f2266'), (.55, '#1f4cb4'), (.72, '#4a84e2'), (.86, '#a6c6f6'),
         (.95, '#e4eeff'), (1, '#f8fbff')]
# Elf "new texture" (user picked the Black Knight pauldrons and asked for a texture redesign):
# brightest engravings stop at silvery blue instead of white so they no longer read as chalk.
POLISHED = [(0, '#02040c'), (.2, '#070d2b'), (.38, '#10256c'), (.55, '#2150b8'), (.72, '#4c86e0'), (.86, '#93b8ec'),
            (.95, '#bcd3f3'), (1, '#d6e6fb')]
CLOTH = [(0, '#02040c'), (.3, '#081233'), (.55, '#1a3577'), (.78, '#7d9fd6'), (1, '#eef4ff')]
# In game the approved elf look (A) read like a blue bodysuit (2026-10-01). After trying lower
# highlights the user asked for the opposite: more bright area. Same palette as A; the tone split
# (fraction of Otsu's threshold) moves down so more surface counts as bright plate, and the plate
# median/floor rise. levels = (threshold factor, dark range, bright median target, bright floor).
ELF_LOOKS = {'prev': ('A · 처음 적용본', '첫 배포 색 · 게임에서 파란 쫄쫄이처럼 보인다는 의견', METAL, None),
             'bright': ('D · 현재 적용본 (밝은 부분 더 많게)', '밝은 판금으로 보는 면적을 넓히고 판금 밝기를 한 단계 올림 · 게임 적용', METAL,
                        (.68, (.16, .4), .53, .47)),
             'brighter': ('E · 밝은 부분 훨씬 많게', 'D보다 한 단계 더: 어두운 곳은 틈새·가장자리 위주로만 남김', METAL,
                          (.56, (.18, .42), .6, .52))}
VARIANTS = {
    'contrast': dict(
        title='푸른달 시안 A · 흑남 / 백청',
        note='어두운 곳 흑남색 · 밝은 곳 흰빛 푸른색 · 중간은 선명한 파랑',
        curve=1.5),
    'contrast-strong': dict(
        title='푸른달 시안 B · 흑남 / 백청 (대비 더 강하게)',
        note='중간 톤을 줄여 흑남색과 흰빛 푸른색 차이를 최대로',
        curve=2.3),
}


def ramp(stops, t):
    t = np.clip(t, 0, 1)
    xs = [s[0] for s in stops]
    cols = np.array([[int(s[1][i:i+2], 16) for i in (1, 3, 5)] for s in stops], float)
    return np.stack([np.interp(t, xs, cols[:, k]) for k in range(3)], -1)


def smooth(x, a, b):
    t = np.clip((x-a)/(b-a), 0, 1)
    return t*t*(3-2*t)


def recolour(image, spec, single=False, levels=(1.0, (0, .33), .4, .4), back=None, palette=None):
    palette = palette or METAL
    rgb = np.asarray(image.convert('RGB'), float)
    hsv = np.asarray(image.convert('RGB').convert('HSV'), float)/255
    hue, sat = hsv[..., 0]*360, hsv[..., 1]
    lum = (rgb@[.299, .587, .114])/255
    warm = smooth(sat, .12, .28)*np.maximum(smooth(hue, -1, 4)*(1-smooth(hue, 58, 70)), smooth(hue, 335, 345))
    patina = smooth(sat, .08, .2)*smooth(hue, 62, 75)*(1-smooth(hue, 160, 180))
    cloth = np.clip(1-warm-patina, 0, 1)
    used = lum > .03  # ignore the unused black atlas background when measuring the range

    def tone(region):
        # Split this material into its dark base (mesh, leather, shadow) and its bright surface
        # (plates, cloth) at a fraction of Otsu's threshold. Dark base → black/deep navy; bright
        # surface → vivid blue at its median, near-white blue only on its brightest highlights.
        pick = used & (region > .5)
        if pick.sum() < 50:
            pick = used
        values = lum[pick]
        hist, edges = np.histogram(values, 64, (0, 1))
        centres = (edges[:-1]+edges[1:])/2
        w0 = np.cumsum(hist)
        w1 = w0[-1]-w0
        m0 = np.cumsum(hist*centres)/np.maximum(w0, 1)
        m1 = (np.sum(hist*centres)-np.cumsum(hist*centres))/np.maximum(w1, 1)
        factor, (dark_lo, dark_hi), target, floor = levels
        threshold = factor*centres[np.argmax(w0*w1*(m0-m1)**2)]
        dark, bright = values[values < threshold], values[values >= threshold]
        lo = np.percentile(dark, 2) if dark.size else 0
        hi = np.percentile(bright, 98) if bright.size else 1
        t_dark = dark_lo+(dark_hi-dark_lo)*np.clip((lum-lo)/max(threshold-lo, 1e-3), 0, 1)
        s = np.clip((lum-threshold)/max(hi-threshold, 1e-3), 0, 1)
        median = float(np.clip(np.median(s[pick & (lum >= threshold)]), .05, .95)) if bright.size else .5
        s = s**(np.log(target)/np.log(median))
        a = spec['curve']
        s = s**a/(s**a+(1-s)**a)
        t_bright = floor+(1-floor)*s
        blend = smooth(lum, threshold-.025, threshold+.025)
        return t_dark*(1-blend)+t_bright*blend

    if single:
        metal = tone(np.ones_like(lum))
        if back is not None and back.max() > .5:
            # The back plate is painted darker than the front; give it its own range.
            metal = metal*(1-back)+tone(back)*back
        out = ramp(palette, metal)*(1-patina)[..., None]
        out += ramp(palette, np.clip(metal+.18, 0, 1))*patina[..., None]
        return Image.fromarray(np.clip(out+.5, 0, 255).astype(np.uint8))
    metal, fabric = tone(warm), tone(cloth)
    out = ramp(METAL, metal)*warm[..., None]
    out += ramp(METAL, np.clip(metal+.18, 0, 1))*patina[..., None]  # plate edges catch the light
    out += ramp(CLOTH, fabric)*cloth[..., None]
    return Image.fromarray(np.clip(out+.5, 0, 255).astype(np.uint8))


def elf_texture(image, tris, spec, info, torso, polished, palette=None, levels=None):
    """Elf/donor texture: Blue Moon tones, own back-plate range, optional polish, chest/back emblems."""
    back = decor.back_mask(tris, image.size) if torso else None
    levels = levels or info['tone']
    if polished:
        # Cleaner metal: drop the painted grain, then bake top-front light and a soft sheen from the
        # actual surface normals.
        image = image.convert('RGB').filter(ImageFilter.MedianFilter(3))
        rgb = np.asarray(recolour(image, spec, True, info['tone'], back, POLISHED), float)
        normals, covered = decor.normal_map(tris, image.size)
        rgb = decor.polish(rgb, normals, covered)
    else:
        rgb = np.asarray(recolour(image, spec, True, levels, back, palette), float)
    return rgb


def build_elf(read, base, source, info, textures):
    """Original, Black Knight pauldrons with the previous texture, and with the new texture."""
    set_key = 'elf'
    spec = VARIANTS['contrast']
    donor = next(d for d in json.loads((O/'donor-sets.json').read_text(encoding='utf-8'))['sets'] if d['key'] == 'blackknight')
    cards = []
    # The user preferred the previous (unpolished) texture; the polished pass stays available in
    # elf_texture(..., polished=True) but is no longer shown.
    polished = False
    for tag, (title, note, palette, levels) in ELF_LOOKS.items():
        parts = {}
        for sex in ('0', '1'):
            pieces = kitbash.shoulder_parts(donor['parts'][sex], base['people'][sex]['idle']['bones'])
            chunks = [dict(c) for c in source['parts'][sex]]+pieces
            for name in sorted({c['texture'] for c in chunks}-SKIN):
                key = f'{set_key}-{tag}/{name}'
                if key not in textures:
                    image = Image.open(BytesIO(png_from_wtm(read('Texture/Body/'+name))))
                    tris = decor.triangles({'x': [c for c in chunks if c['texture'] == name]}, name)
                    torso = '_tor_' in name and any(c['texture'] == name for c in source['parts'][sex])
                    rgb = elf_texture(image, tris, spec, info, torso, polished, palette, levels)
                    for facing, height, radius in (EMBLEMS.get(name[:3], []) if torso else []):
                        rgb = decor.emblem3d(rgb, tris, facing, height, radius)
                    target = f'{set_key}/{tag}-{Path(name).stem}.png'
                    Image.fromarray(np.clip(rgb+.5, 0, 255).astype(np.uint8)).save(O/target)
                    textures[key] = target
            parts[sex] = [dict(c, texture=f'{set_key}-{tag}/{c["texture"]}' if c['texture'] not in SKIN else c['texture'])
                          for c in chunks]
        cards.append(dict(source, key=f'{set_key}-bk-{tag}', parts=parts, title=title, note=note))
    return cards


def main():
    read = resource_reader(R/'client-overlay')
    base = json.loads((O/'armor-sets.json').read_text(encoding='utf-8'))
    for set_key, info in BASES.items():
        source = next(x for x in base['sets'] if x['key'] == set_key)
        names = sorted({c['texture'] for sex in ('0', '1') for c in source['parts'][sex]})
        (O/set_key).mkdir(exist_ok=True)
        for old_png in (O/set_key).glob('*.png'):
            old_png.unlink()
        textures = {k: v for k, v in base['textures'].items()}
        sets = [dict(source, key=f'{set_key}-original', title=f'원본 · {info["name"]} 세트', note='게임 원본 텍스처 그대로')]
        if info.get('decorate'):
            sets += build_elf(read, base, source, info, textures)
        for key, spec in ({} if info.get('decorate') else VARIANTS).items():
            remap = {}
            for name in names:
                if name in SKIN:
                    continue
                image = Image.open(BytesIO(png_from_wtm(read('Texture/Body/'+name))))
                target = f'{set_key}/{key}-{Path(name).stem}.png'
                recolour(image, spec, info['single'], info['tone']).save(O/target)
                remap[name] = f'{set_key}-{key}/{name}'
                textures[remap[name]] = target
            parts = {sex: [dict(c, texture=remap.get(c['texture'], c['texture'])) for c in source['parts'][sex]]
                     for sex in ('0', '1')}
            sets.append(dict(source, key=f'{set_key}-{key}', parts=parts, title=spec['title'], note=spec['note']))
        page = dict(title=f'푸른달 갑옷 · {info["name"]} 세트 색 시안',
                    lead=f'{info["name"]} 세트({info["models"]})의 형태는 그대로 두고 텍스처 색만 푸른달 무기 컨셉으로 바꾼 시안입니다. '
                         '어두운 부분은 흑남색, 밝은 부분은 흰빛에 가까운 푸른색으로 명암 차이를 크게 벌렸습니다. B는 대비를 더 강하게 줬습니다. '
                         + ('블랙나이트 견갑·윗팔 보호대를 옮겨 붙이고 가슴·등 중앙에 초승달 문양을 그렸습니다. 문양은 실제 가슴·등 면에 맞춰 그려 정면·뒷면에서 동그랗게 보입니다. ' if info.get('decorate') else '')
                         + '여자 세트의 맨살 텍스처는 바꾸지 않았습니다. 아무 화면이나 드래그하면 모든 캐릭터가 같이 돌아갑니다.')
        data = dict(people=base['people'], sets=sets, textures=textures, page=page)
        (O/f'{set_key}-bluemoon.json').write_text(json.dumps(data, ensure_ascii=False, separators=(',', ':')), encoding='utf-8')
        print(json.dumps(dict(set=set_key, textures=[n for n in names if n not in SKIN], variants=list(VARIANTS)),
                         ensure_ascii=False))


if __name__ == '__main__':
    main()
