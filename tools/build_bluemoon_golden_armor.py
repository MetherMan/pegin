"""Blue Moon colour concepts for the Golden warrior armour set (preview only).

Reads the native Golden set (male ma_*_b003, female fe_*_b001) and recolours its textures
towards the Blue Moon weapons: warm gold/leather by luminance into silver-blue or sapphire,
patina into light-blue trim, neutral cloth and mesh with a cool tint. Skin textures are left
untouched. Writes a data file for assets/blue-moon-armor/index.html?data=golden-bluemoon.
"""
from pathlib import Path
from io import BytesIO
import json, sys

R = Path(__file__).resolve().parents[1]
sys.path[:0] = [str(R/'runtime/pylibs'), str(R/'client-overlay/Tools/SkillColors'), str(R/'tools')]
import numpy as np
from PIL import Image
from native_assets import png_from_wtm
from tuning import resource_reader

O = R/'assets/blue-moon-armor'
SKIN = {'Girl_torso_0-1.wtm', 'Girl_leg_0-.wtm'}  # bare skin under the female golden set
# Saturated deep blues. Highlights stop at vivid azure instead of white so the plates read
# as enamelled blue metal rather than pale ice (user feedback on the first silver-blue pass).
VARIANTS = {
    'cobalt': dict(
        title='푸른달 시안 A · 코발트 달빛',
        note='금판 → 선명한 코발트 판금 · 가죽 → 짙은 남색 · 테두리 → 밝은 하늘색',
        metal=[(0, '#04072a'), (.25, '#0b1a6e'), (.5, '#1840c0'), (.75, '#2f74f0'), (1, '#77b6ff')],
        trim=[(0, '#0a3a8a'), (.45, '#1e8ee8'), (1, '#6fd2ff')],
        cloth=[(0, '#03051c'), (.4, '#101d63'), (.75, '#2446b8'), (1, '#4f7ce8')]),
    'ultramarine': dict(
        title='푸른달 시안 B · 군청 심야',
        note='금판 → 보랏빛이 도는 군청 판금 · 가죽 → 먹남색 · 테두리 → 하늘색',
        metal=[(0, '#050524'), (.25, '#12146a'), (.5, '#2530b8'), (.75, '#4461ec'), (1, '#8ea2ff')],
        trim=[(0, '#0c2f86'), (.45, '#2a80e6'), (1, '#78ccff')],
        cloth=[(0, '#04041a'), (.4, '#141660'), (.75, '#2c38b0'), (1, '#5a6ae0')]),
    'navysilver': dict(
        title='푸른달 시안 C · 청람 은테',
        note='금판 → 짙은 청람 판금 · 모서리 테두리만 은색 · 가죽 → 먹남색',
        metal=[(0, '#030a24'), (.25, '#082366'), (.5, '#1349ad'), (.75, '#2a78e0'), (1, '#6fb0f5')],
        trim=[(0, '#56657f'), (.5, '#aab8cf'), (1, '#e8eef8')],
        cloth=[(0, '#02061a'), (.4, '#0c1f5c'), (.75, '#1d44a8'), (1, '#4574d8')]),
}


def ramp(stops, t):
    t = np.clip(t, 0, 1)
    xs = [s[0] for s in stops]
    cols = np.array([[int(s[1][i:i+2], 16) for i in (1, 3, 5)] for s in stops], float)
    return np.stack([np.interp(t, xs, cols[:, k]) for k in range(3)], -1)


def smooth(x, a, b):
    t = np.clip((x-a)/(b-a), 0, 1)
    return t*t*(3-2*t)


def recolour(image, spec):
    rgb = np.asarray(image.convert('RGB'), float)
    hsv = np.asarray(image.convert('RGB').convert('HSV'), float)/255
    hue, sat = hsv[..., 0]*360, hsv[..., 1]
    lum = (rgb@[.299, .587, .114])/255
    warm = smooth(sat, .12, .28)*np.maximum(smooth(hue, -1, 4)*(1-smooth(hue, 58, 70)), smooth(hue, 335, 345))
    patina = smooth(sat, .08, .2)*smooth(hue, 62, 75)*(1-smooth(hue, 160, 180))
    cool = smooth(sat, .12, .25)*smooth(hue, 185, 200)*(1-smooth(hue, 300, 320))
    neutral = np.clip(1-warm-patina-cool, 0, 1)
    t = np.clip((lum-.03)/.85, 0, 1)**.95
    t = .5+(t-.5)*.85  # slightly flatter: fewer bright scratches that read as frost
    out = ramp(spec['metal'], t)*warm[..., None]
    out += ramp(spec['trim'], t*1.1)*patina[..., None]
    out += ramp(spec['cloth'], t)*(cool+neutral)[..., None]
    return Image.fromarray(np.clip(out+.5, 0, 255).astype(np.uint8))


def main():
    read = resource_reader(R/'client-overlay')
    base = json.loads((O/'armor-sets.json').read_text(encoding='utf-8'))
    golden = next(s for s in base['sets'] if s['key'] == 'golden')
    names = sorted({c['texture'] for sex in ('0', '1') for c in golden['parts'][sex]})
    (O/'golden').mkdir(exist_ok=True)
    textures = {k: v for k, v in base['textures'].items()}
    sets = [dict(golden, key='golden-original', title='원본 · 황금 세트', note='게임 원본 텍스처 그대로')]
    for key, spec in VARIANTS.items():
        remap = {}
        for name in names:
            if name in SKIN:
                continue
            image = Image.open(BytesIO(png_from_wtm(read('Texture/Body/'+name))))
            target = f'golden/{key}-{Path(name).stem}.png'
            recolour(image, spec).save(O/target)
            remap[name] = f'{key}/{name}'
            textures[remap[name]] = target
        parts = {sex: [dict(c, texture=remap.get(c['texture'], c['texture'])) for c in golden['parts'][sex]]
                 for sex in ('0', '1')}
        sets.append(dict(golden, key=key, parts=parts, title=spec['title'], note=spec['note']))
    page = dict(title='푸른달 갑옷 · 황금 세트 색 시안',
                lead='황금 세트(남 b003 / 여 b001)의 형태는 그대로 두고 텍스처 색만 푸른달 무기 컨셉으로 바꾼 시안입니다. '
                     '얼음처럼 보이지 않도록 흰빛 하이라이트를 줄이고 채도를 높였습니다. 금판은 명암을 유지한 채 코발트·군청·청람으로, 가죽과 천은 짙은 남색 계열로 바꿨습니다. '
                     '여자 세트의 맨살 텍스처는 바꾸지 않았습니다. 아무 화면이나 드래그하면 모든 캐릭터가 같이 돌아갑니다.')
    data = dict(people=base['people'], sets=sets, textures=textures, page=page)
    (O/'golden-bluemoon.json').write_text(json.dumps(data, ensure_ascii=False, separators=(',', ':')), encoding='utf-8')
    print(json.dumps(dict(textures=[n for n in names if n not in SKIN], skin_untouched=sorted(SKIN),
                          variants=list(VARIANTS)), ensure_ascii=False))


if __name__ == '__main__':
    main()
