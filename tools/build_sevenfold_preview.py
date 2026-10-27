"""Preview data for the proposed melee 140 skill 칠익난무 (Sevenfold Wing Dance), not game files.

Caster: native male character in the Blue Moon armour (Body/High/*_b050) holding the 천상의 날개
sword. The game has a single skill motion per weapon (m_ths_mgc_A, a slow raise that reads as
spell casting), so the preview uses the real two-handed sword attack motions instead: A (diagonal
downward cut, hit frame 9), B (rising cut, 12), C (thrust, 22) and the two-handed stance as idle.
Effect textures: new additive sprites drawn here (X flash streak, star core, feather, light
wing, ground seal; the sword-energy arcs, star lines and sparks are drawn in the page) plus the
game's own radial burst (vb0001_1 / ta_03) and ring (bwind03 / bwind01) recoloured along the same
gold ramp as the Celestial weapon effects, and the sword's own gold trail texture (celestial_gold,
cw_sword_trail.wtm). Sounds: two-handed sword swings (a = swing, b = hit), the normal hit, the
meteor launch/hit and the lightning attack (the 브레이크레볼루션 vb_0002.wav is not in this client).
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
SOUNDS = [*[f'm_swd_atk_{k}_{v}.wav' for k in (1, 2, 3, 4) for v in 'ab'], 'attack100049.wav', 'mm_meteor_launch.wav',
          'mm_meteor_hit.wav', 'Elec_Attack.wav']


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
    clips = dict(idle=animation(read('Body/Animation/m_ths_at_std_61.ani'), list(range(1, 62))))
    for key, name, frames in [('atkA', 'm_ths_atk_A_9_44', 44), ('atkB', 'm_ths_atk_B_12_51', 51), ('atkC', 'm_ths_atk_C_22_54', 54)]:
        clips[key] = animation(read(f'Body/Animation/{name}.ani'), list(range(1, frames+1)))
    for clip in clips.values():
        clip['duration'] = clip['frames']*1000/30
    out['people']['0'] = dict(parts=parts, **clips)
    out['weapons']['atkA'] = dict(parts=load('Equip/cw_sword_1.mod', 'Equip'), bone='Bip01 R Finger0')
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


def wing():
    """One wing of light, root at the lower left: long primaries hanging from the arm, shorter
    coverts along it, each feather a soft vane with a bright shaft."""
    s = 1024
    img = Image.new('L', (s, s), 0)
    d = ImageDraw.Draw(img)

    def vane(x, y, angle, length, width, fill):
        c, n = np.cos(angle), np.sin(angle)
        side = []
        for k in range(25):
            t = k/24
            r = width*np.sin(np.pi*min(1, t*1.15))**.7
            side.append((x+c*length*t, y+n*length*t, r))
        pts = [(px-n*r, py+c*r) for px, py, r in side]+[(px+n*r, py-c*r) for px, py, r in reversed(side)]
        d.polygon(pts, fill=fill)
        d.line([(x, y), (x+c*length*.92, y+n*length*.92)], fill=255, width=5)
    root, wrist = np.array([.06, .55])*s, np.array([.5, .08])*s    # root = WING_ROOT in the page
    for k in range(14):                      # primaries hang from the arm, longest towards the wrist
        t = k/13
        x, y = root+(wrist-root)*t
        angle = np.radians(82-62*t**1.2)
        vane(x, y, angle, s*(.2+.34*np.sin(np.pi*min(1, t*.6))), s*.042, 120)
    for k in range(11):                      # coverts along the arm
        t = k/10
        x, y = root+(wrist-root)*t
        vane(x, y, np.radians(70-45*t), s*(.1+.07*t), s*.032, 95)
    d.line([tuple(root), tuple(wrist)], fill=255, width=12)
    img = img.filter(ImageFilter.GaussianBlur(4)).resize((512, 512), Image.LANCZOS)
    a = np.asarray(img, float)
    glow = np.asarray(img.filter(ImageFilter.GaussianBlur(14)), float)
    return ramp_image(np.dstack([np.clip(a+glow*.6, 0, 255)]*3))


def seal():
    """Ground seal under the target: double outer ring with ticks, a broken rune ring, inner rings."""
    s = 2048
    img = Image.new('L', (s, s), 0)
    d = ImageDraw.Draw(img)
    c = s/2

    def ring(r, w):
        d.ellipse([c-r*s, c-r*s, c+r*s, c+r*s], outline=255, width=w)

    def at(a, r):
        return c+np.cos(a)*r*s, c+np.sin(a)*r*s
    ring(.485, 12)
    ring(.452, 6)
    for k in range(144):
        a = 2*np.pi*k/144
        r0, r1 = (.456, .482) if k % 4 == 0 else (.461, .473)
        d.line([at(a, r0), at(a, r1)], fill=220, width=5)
    for k in range(28):
        a0 = 360*k/28
        d.arc([c-.405*s, c-.405*s, c+.405*s, c+.405*s], a0+2, a0+9.5, fill=235, width=10)
        x, y = at(np.radians(a0+11.5), .405)
        d.ellipse([x-8, y-8, x+8, y+8], fill=255)
    ring(.365, 5)
    ring(.2, 8)
    ring(.178, 4)
    for k in range(7):
        a = 2*np.pi*k/7
        d.line([at(a, .2), at(a, .365)], fill=200, width=4)
    img = img.filter(ImageFilter.GaussianBlur(2)).resize((1024, 1024), Image.LANCZOS)
    a = np.asarray(img, float)
    glow = np.asarray(img.filter(ImageFilter.GaussianBlur(10)), float)
    return ramp_image(np.dstack([np.clip(a*.9+glow*.9, 0, 255)]*3))


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
    x_flash().save(O/'xflash.png')
    star().save(O/'star.png')
    wing().save(O/'wing.png')
    seal().save(O/'seal.png')
    feather().save(O/'feather.png')
    recoloured(read, 'ta_03.wtm', .12).save(O/'burst.png')    # vb0001_1.wed radial burst
    recoloured(read, 'bwind01.wtm', .3).save(O/'ring.png')    # bwind03.wed ring
    (O/'trail.png').write_bytes(png_from_wtm(read('Texture/Effect/cw_sword_trail.wtm')))  # trace.tdf celestial_gold
    print('sevenfold preview data written', sorted(p.name for p in O.iterdir()))


if __name__ == '__main__':
    main()
