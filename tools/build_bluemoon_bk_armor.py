"""Blue Moon colours on the Black Knight set (preview only).

The applied Elf-based armour looked cheap in game and its painted chest crescent stretched, so the
user asked to try the Black Knight set (male ma_*_b032, female fe_*_b034, Rank 6 model) instead.
Every non-skin texture gets the same Blue Moon palette and the applied look D tone split from
tools/build_bluemoon_golden_armor.py; torso textures keep their own back-plate range. One card adds
a slightly smaller crescent on the outermost chest plate (male ma_tor_b032 front/back, female
fe_tor_b034_1 front). Writes assets/blue-moon-armor/bk-bluemoon.json for index.html?data=bk-bluemoon.
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
from build_bluemoon_golden_armor import recolour, VARIANTS, ELF_LOOKS, SKIN, METAL

O = R/'assets/blue-moon-armor'
D_LEVELS = ELF_LOOKS['bright'][3]
# texture: [(facing, height fraction of the chest piece, radius)]
MOONS = {'ma_tor_b032.wtm': [(-1, .65, .062), (1, .65, .062)], 'fe_tor_b034_1.wtm': [(-1, .45, .045)]}
# The male Black Knight back is a grey V-shaped plate over dark leather with crossing straps. With
# the bright Blue Moon tones the leather turned light blue too, so plate + leather + the glowing
# crescent disc read as an Iron Man mask. 'calm' keeps only the front crescent and gives the upper
# back (back-facing texels of the main torso piece above the skirt) a darker tone split, so the
# leather returns to dark navy while the metal V-plate stays bright.
CALM = {'ma_tor_b032.wtm': dict(min_y=1.12, levels=(1.15, (.06, .3), .5, .5))}
A_LEVELS = (.8, (.16, .4), .45, .42)  # look A (BASES['elf']['tone']): darker, as first released
# tag: (title, note, crescent: False / True (front and back) / 'front', levels, calm back plate)
LOOKS = {'d': ('푸른달 색 D · 블랙나이트', '블랙나이트 갑옷 모양 그대로 · 적용 중인 D 색 · 문양 없음', False, D_LEVELS, False),
         'd-moon': ('푸른달 색 D · 블랙나이트 + 초승달', '같은 색 · 바깥 가슴판에 조금 작은 초승달(여자는 앞만)', True, D_LEVELS, False),
         'd-moon-calm': ('D + 초승달 · 등판 정리', '등 초승달 원판을 빼고(앞만) 등 가죽 부분을 원래처럼 짙은 남색으로 · V자 판금은 밝게 유지', 'front', D_LEVELS, True),
         'a': ('푸른달 색 A · 블랙나이트', '처음 색(흑남이 더 많음) · 블랙나이트 바탕이 원래 어두워 D가 많이 밝게 나와 비교용', False, A_LEVELS, False)}


def calm_back(rgb, image, tris, spec, style):
    """Upper back of the main torso piece re-toned with a darker split (leather dark, plate bright)."""
    main = [t for t in tris if t['verts'] == max(x['verts'] for x in tris)]
    h, w, _ = rgb.shape
    mask = decor.raster(main, (w, h), lambda t: t['n'][2] > .3 and t['pos'][:, 1].mean() > spec['min_y'])
    soft = np.asarray(Image.fromarray((mask*255).astype(np.uint8)).filter(ImageFilter.GaussianBlur(1.2)), float)/255
    soft = np.minimum(soft, mask)  # feather inward only, never past the island edge
    dark = np.asarray(recolour(image, style, True, spec['levels'], None, METAL), float)
    return rgb*(1-soft[..., None])+dark*soft[..., None]


def main():
    read = resource_reader(R/'client-overlay')
    donor = json.loads((O/'donor-sets.json').read_text(encoding='utf-8'))
    bk = next(s for s in donor['sets'] if s['key'] == 'blackknight')
    (O/'bk').mkdir(exist_ok=True)
    for old in (O/'bk').glob('*.png'):
        old.unlink()
    textures = {k: v for k, v in donor['textures'].items()}
    spec = VARIANTS['contrast']
    sets = [dict(bk, key='bk-original', title='원본 · 블랙나이트 세트', note='게임 원본 텍스처 그대로 (남 b032 · 여 b034)')]
    for tag, (title, note, moon, levels, calm) in LOOKS.items():
        parts = {}
        for sex in ('0', '1'):
            chunks = bk['parts'][sex]
            for name in sorted({c['texture'] for c in chunks}-SKIN):
                key = f'bk-{tag}/{name}'
                if key in textures:
                    continue
                image = Image.open(BytesIO(png_from_wtm(read('Texture/Body/'+name))))
                tris = decor.triangles({'x': [c for c in chunks if c['texture'] == name]}, name)
                back = decor.back_mask(tris, image.size) if '_tor_' in name else None
                rgb = np.asarray(recolour(image, spec, True, levels, back, METAL), float)
                if calm and name in CALM:
                    rgb = calm_back(rgb, image, tris, CALM[name], spec)
                for facing, height, radius in (MOONS.get(name, []) if moon else []):
                    if moon == 'front' and facing > 0:
                        continue
                    rgb = decor.emblem3d(rgb, tris, facing, height, radius)
                target = f'bk/{tag}-{Path(name).stem}.png'
                Image.fromarray(np.clip(rgb+.5, 0, 255).astype(np.uint8)).save(O/target)
                textures[key] = target
            parts[sex] = [dict(c, texture=f'bk-{tag}/{c["texture"]}' if c['texture'] not in SKIN else c['texture'])
                          for c in chunks]
        sets.append(dict(bk, key=f'bk-{tag}', parts=parts, title=title, note=note))
    page = dict(title='푸른달 갑옷 · 블랙나이트 바탕 시안',
                lead='엘프 바탕이 게임에서 싸 보이고 가슴 초승달이 늘어져 보인다는 의견으로, 블랙나이트 세트(남 b032 · 여 b034) 모양 그대로 '
                     '지금 적용한 D 색(흑남 바탕 · 밝은 청은빛 판금)을 입혔습니다. 초승달은 없는 것과, 바깥 가슴판에 조금 작게 넣은 것을 비교합니다. 블랙나이트 바탕은 원래 어두워 D가 꽤 밝게 나오므로 처음 색 A도 함께 둡니다. '
                     '여자 세트의 맨살 텍스처는 바꾸지 않았습니다. 아무 화면이나 드래그하면 모든 캐릭터가 같이 돌아갑니다.')
    out = dict(people=donor['people'], sets=sets, textures=textures, page=page)
    (O/'bk-bluemoon.json').write_text(json.dumps(out, ensure_ascii=False, separators=(',', ':')), encoding='utf-8')
    print(json.dumps([s['key'] for s in sets]), len([k for k in textures if k.startswith('bk-')]))


if __name__ == '__main__':
    main()
