"""Preview data for the proposed melee 140 skill 칠익난무 (Sevenfold Wing Dance), not game files.

Caster: native male character in the Blue Moon armour (Body/High/*_b050) holding the 천상의 날개
sword, playing the real two-handed sword skill motion (character.wad weapon 4 state 14,
m_ths_mgc_A: raise 1-56, release 57-89) and the two-handed stance as idle. Effect textures: four
new additive sprites drawn here (crescent slash, X flash streak, star core, feather) plus the
game's own radial burst (vb0001_1 / ta_03) and ring (bwind03 / bwind01) recoloured along the same
gold ramp as the Celestial weapon effects, and the sword's own gold trail texture (celestial_gold,
cw_sword_trail.wtm). Sounds: two-handed sword swings and the normal hit (the 브레이크레볼루션
vb_0002.wav is not in this client).
Writes assets/golden-knight/skills/sevenfold/{actors.json, *.png}.
"""
from pathlib import Path
from io import BytesIO
import base64, json, struct, sys
R = Path(__file__).resolve().parents[1]
sys.path[:0] = [str(R/'runtime/pylibs'), str(R/'client-overlay/Tools/SkillColors'), str(R/'tools')]
import numpy as np
from PIL import Image, ImageDraw, ImageFilter
from native_actor import model, animation
from native_assets import png_from_wtm
from tuning import resource_reader
from build_celestial_effects import gold

O = R/'assets/golden-knight/skills/sevenfold'
SOUNDS = ['m_swd_atk_1_b.wav', 'm_swd_atk_2_b.wav', 'm_swd_atk_3_b.wav', 'm_swd_atk_4_b.wav', 'attack100049.wav']


def data_url(raw, mime):
    return f'data:{mime};base64,'+base64.b64encode(raw).decode()


def actors(read):
    out = dict(people={}, weapons={}, textures={}, sounds={})

    def load(path, folder):
        chunks = model(read(path))
        for c in chunks:
            name = folder+'/'+c['texture']
            c['texture'] = name
            if name not in out['textures']:
                out['textures'][name] = data_url(png_from_wtm(read('Texture/'+name)), 'image/png')
        return chunks
    parts = []
    for part in ('tor', 'leg', 'boo', 'gun'):
        parts += load(f'Body/High/ma_{part}_b050_1.mod', 'Body')
    parts += load('Body/High/mb_hea_0001.mod', 'Body')+load('Body/High/mb_hir_0001.mod', 'Body')
    skill = animation(read('Body/Animation/m_ths_mgc_A__56-57-89.ani'), list(range(1, 90)))
    skill['duration'] = 89*1000/30
    idle = animation(read('Body/Animation/m_ths_at_std_61.ani'), list(range(1, 62)))
    idle['duration'] = 61*1000/30
    out['people']['0'] = dict(parts=parts, skill=skill, idle=idle)
    out['weapons']['skill'] = dict(parts=load('Equip/cw_sword_1.mod', 'Equip'), bone='Bip01 R Finger0')
    for name in SOUNDS:
        out['sounds'][name] = data_url(read('Sound/'+name), 'audio/wav')
    return out


def ramp_image(rgb):
    a = np.asarray(rgb, float)
    out = np.zeros_like(a)
    for y in range(a.shape[0]):
        for x in range(a.shape[1]):
            out[y, x] = gold(*a[y, x])
    return Image.fromarray(out.astype(np.uint8))


def slash_arc():
    """Crescent: an elliptical band thickest in the middle, bright leading (outer) edge, soft inside."""
    w, h = 512, 256
    u, v = np.meshgrid((np.arange(w)+.5)/w, (np.arange(h)+.5)/h)
    d = np.sqrt(((u-.5)/.47)**2+((v-1.12)/.98)**2)-1
    thick = .16*np.sin(np.pi*np.clip(u, 0, 1))**.8+1e-4
    inside = np.clip(-d/thick, 0, None)
    band = np.where(d <= 0, np.exp(-2.6*inside)*(inside <= 1.35), 0)
    edge = np.exp(-(d/.010)**2)*np.sin(np.pi*u)**.6
    glow = np.exp(-np.maximum(d, 0)/.035)*(d > 0)*.35*np.sin(np.pi*u)
    i = np.clip(band*.95+edge*1.1+glow, 0, 1)
    return ramp_image(np.dstack([i*255]*3))


def x_flash():
    w, h = 512, 64
    u, v = np.meshgrid((np.arange(w)+.5)/w, (np.arange(h)+.5)/h)
    i = np.exp(-((v-.5)/.07)**2)*np.clip(1-np.abs(u-.5)*2, 0, 1)**.7+np.exp(-((v-.5)/.2)**2)*.25*np.clip(1-np.abs(u-.5)*2, 0, 1)
    return ramp_image(np.dstack([np.clip(i, 0, 1)*255]*3))


def star():
    """Finale core: bright soft centre with eight rays, long and short alternating."""
    s = 256
    u, v = np.meshgrid((np.arange(s)+.5)/s-.5, (np.arange(s)+.5)/s-.5)
    r, t = np.hypot(u, v), np.arctan2(v, u)
    i = np.exp(-(r/.06)**2)+.45*np.exp(-r/.11)
    for k in range(8):
        a = k*np.pi/4
        along, across = r*np.cos(t-a), r*np.abs(np.sin(t-a))
        i += (along > 0)*np.exp(-(across/(.01+.03*along))**2)*np.exp(-along/(.2 if k % 2 else .1))
    i *= np.clip(1-r/.5, 0, 1)**.5
    return ramp_image(np.dstack([np.clip(i, 0, 1)*255]*3))


def feather():
    s = 4*128
    img = Image.new('L', (s, s), 0)
    d = ImageDraw.Draw(img)
    # vane: leaf shape along the diagonal, with notches cut out
    pts = []
    for k in range(41):
        t = k/40
        r = .19*np.sin(np.pi*t)**.8*s
        x, y = .18*s+.64*s*t, .82*s-.64*s*t
        pts.append((x+r*.7071, y+r*.7071))
    for k in range(40, -1, -1):
        t = k/40
        r = .19*np.sin(np.pi*t)**.8*s
        x, y = .18*s+.64*s*t, .82*s-.64*s*t
        pts.append((x-r*.7071, y-r*.7071))
    d.polygon(pts, fill=150)
    for t in (.35, .55, .72):
        x, y = .18*s+.64*s*t, .82*s-.64*s*t
        d.line([(x, y), (x+.16*s, y+.02*s)], fill=0, width=7)
    d.line([(.12*s, .88*s), (.84*s, .16*s)], fill=255, width=10)
    img = img.filter(ImageFilter.GaussianBlur(5)).resize((128, 128), Image.LANCZOS)
    a = np.asarray(img, float)
    return ramp_image(np.dstack([a]*3))


def recoloured(read, texture, floor):
    """Gold ramp over the texture's brightness, dropping tones below `floor` so the dim interior
    does not add up to a brown disc when several copies overlap additively."""
    a = np.asarray(Image.open(BytesIO(png_from_wtm(read('Texture/Effect/'+texture)))).convert('RGB'), float).max(axis=2)/255
    k = np.clip((a-floor)/(1-floor), 0, 1)
    return ramp_image(np.dstack([k*255]*3))


def main():
    O.mkdir(parents=True, exist_ok=True)
    read = resource_reader(R/'client-overlay')
    (O/'actors.json').write_text(json.dumps(actors(read), separators=(',', ':')), encoding='utf-8')
    slash_arc().save(O/'slash.png')
    x_flash().save(O/'xflash.png')
    star().save(O/'star.png')
    feather().save(O/'feather.png')
    recoloured(read, 'ta_03.wtm', .12).save(O/'burst.png')    # vb0001_1.wed radial burst
    recoloured(read, 'bwind01.wtm', .3).save(O/'ring.png')    # bwind03.wed ring
    (O/'trail.png').write_bytes(png_from_wtm(read('Texture/Effect/cw_sword_trail.wtm')))  # trace.tdf celestial_gold
    print('sevenfold preview data written', sorted(p.name for p in O.iterdir()))


if __name__ == '__main__':
    main()
