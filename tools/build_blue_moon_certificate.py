"""Blue Moon horse certificate icon: the original brown-horse certificate, recoloured pixel by pixel.

Keeps the native 28x28 pixel drawing, frame and outline. Coat → saturated deep blue, white mane →
sky blue, eye → glowing blue, and a small pearl horn between the ears.
"""
from pathlib import Path
from io import BytesIO
import colorsys, json, sys

R = Path(__file__).resolve().parents[1]
sys.path[:0] = [str(R/'runtime/pylibs'), str(R/'client-overlay/Tools/SkillColors')]
from PIL import Image
from native_assets import png_from_wtm

O = R/'assets/blue-moon-horse'
COAT = [(0, (8, 14, 52)), (.35, (22, 52, 150)), (.65, (48, 104, 222)), (1, (150, 196, 255))]
MANE = [(0, (22, 84, 150)), (.5, (86, 180, 240)), (1, (214, 242, 255))]
# Horn from the forehead between the ears up through the top outline (x, y, colour).
HORN = [(18, 7, (150, 176, 214)), (19, 7, (96, 124, 176)), (18, 6, (176, 200, 234)), (19, 6, (110, 138, 190)),
        (17, 5, (198, 220, 246)), (18, 5, (124, 156, 208)), (17, 4, (214, 234, 255)), (18, 4, (140, 186, 236)),
        (17, 3, (230, 246, 255)), (16, 2, (238, 252, 255)), (17, 2, (150, 214, 255)), (16, 1, (190, 240, 255))]
HORN_OUTLINE = [(17, 6), (16, 5), (16, 4), (16, 3), (15, 2), (15, 1), (17, 1), (18, 2), (18, 3), (19, 4), (19, 5), (20, 6),
                (20, 7)]
EYE = [(16, 12), (17, 12), (16, 13), (17, 13)]


def ramp(stops, t):
    t = min(1, max(0, t))
    for (a, ca), (b, cb) in zip(stops, stops[1:]):
        if t <= b:
            k = (t-a)/(b-a)
            return tuple(round(x+(y-x)*k) for x, y in zip(ca, cb))
    return stops[-1][1]


def build():
    source = Image.open(BytesIO(png_from_wtm((R/'runtime/client/GameClient/Item/horse01.wtm').read_bytes()))).convert('RGB')
    icon = source.copy()
    px = icon.load()
    changed = {'coat': 0, 'mane': 0}
    for y in range(28):
        for x in range(28):
            r, g, b = source.getpixel((x, y))
            h, l, s = colorsys.rgb_to_hls(r/255, g/255, b/255)
            inside = 3 <= x <= 22 and 3 <= y <= 24
            if inside and s > .35 and (h*360 < 21 or h*360 > 340) and l > .08:
                px[x, y] = ramp(COAT, (l-.12)/.6)
                changed['coat'] += 1
            elif inside and y <= 12 and s < .25 and l > .3:
                px[x, y] = ramp(MANE, (l-.3)/.65)
                changed['mane'] += 1
    for x, y in HORN_OUTLINE:
        if source.getpixel((x, y)) != (0, 0, 0):
            px[x, y] = (14, 22, 48)
    for x, y, colour in HORN:
        px[x, y] = colour
    for (x, y), colour in zip(EYE, [(120, 230, 255), (40, 150, 255), (40, 150, 255), (20, 90, 220)]):
        px[x, y] = colour
    return source, icon, changed


def main():
    source, icon, changed = build()
    icon.save(O/'certificate-icon.png')
    sheet = Image.new('RGB', (600, 290), (24, 28, 40))
    sheet.paste(source.resize((280, 280), Image.Resampling.NEAREST), (5, 5))
    sheet.paste(icon.resize((280, 280), Image.Resampling.NEAREST), (315, 5))
    sheet.save(O/'certificate-comparison.png')
    print(json.dumps(changed))


if __name__ == '__main__':
    main()
