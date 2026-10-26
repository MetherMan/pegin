"""Golden Knight weapon concepts (preview only): existing high-tier weapon models in gold.

The user wants a "cool golden knight" set and left the design open. As with the Blue Moon armour
(the kitbashed and recoloured artist models were liked, procedural geometry was not), the
candidates are the game's own higher-tier two-handed swords, longbows and staves, recoloured:

- gold: every metal surface in a royal gold ramp (deep bronze shadows -> pale gold highlights).
- whitegold: neutral metal (blades, limbs) becomes polished white silver, warm/coloured fittings
  become gold, the paladin look.
- original: game texture as is.
Saturated gems keep their own hue in both styles. Reads game files only; writes
assets/golden-knight/glb/<key>-<style>.glb and catalog.json for assets/golden-knight/index.html.
"""
from pathlib import Path
from io import BytesIO
import json, struct, sys, zlib

R = Path(__file__).resolve().parents[1]
sys.path[:0] = [str(R/'runtime/pylibs'), str(R/'tools'), str(R/'client-overlay/Tools/SkillColors')]
import numpy as np
from PIL import Image
from tuning import resource_reader
from bluemoon_armor_native import parse

O = R/'assets/golden-knight'
# (weapon kind, model prefix, item id, name)
CANDIDATES = [
    ('sword', 'ws_0078', 967, '성기사의 양손검'), ('sword', 'ws_0081', 11424, '블랙나이트 양손검'),
    ('sword', 'ws_0076', 11403, '레드나이트 양손검'), ('sword', 'ws_0080', 1009, '데스나이트의 양손검'),
    ('bow', 'wlb_0017', 3466, '성기사의 롱보우'), ('bow', 'wb_0040', 11739, '화이트헌터 롱보우'),
    ('bow', 'wb_0038', 11718, '레드헌터 롱보우'), ('bow', 'wlb_0020', 3529, '전사의 롱보우'),
    ('staff', 'ww_0015', 11865, '달의정령 스태프'), ('staff', 'ww_0002', 11844, '대지의정령 스태프'),
    ('staff', 'wf_0032', 2668, '실버 스태프'), ('staff', 'wf_0031', 2689, '전기석 지팡이'),
]
GOLD = [(0, '#120a02'), (.18, '#3a2208'), (.38, '#7a4c12'), (.56, '#c08a22'), (.72, '#e8b84a'), (.86, '#ffe08a'),
        (.95, '#fff3c8'), (1, '#fffbea')]
SILVER = [(0, '#0d0f15'), (.25, '#343a48'), (.5, '#8c95a8'), (.72, '#d4dbe8'), (.88, '#f1f4fa'), (1, '#ffffff')]
STYLES = ('gold', 'whitegold', 'original')


def ramp(stops, t):
    t = np.clip(t, 0, 1)
    xs = [s[0] for s in stops]
    cols = np.array([[int(s[1][i:i+2], 16) for i in (1, 3, 5)] for s in stops], float)
    return np.stack([np.interp(t, xs, cols[:, k]) for k in range(3)], -1)


def smooth(x, a, b):
    t = np.clip((x-a)/(b-a), 0, 1)
    return t*t*(3-2*t)


def recolour(image, style):
    rgb = np.asarray(image.convert('RGB'), float)
    if style == 'original':
        return image.convert('RGB')
    hsv = np.asarray(image.convert('RGB').convert('HSV'), float)/255
    hue, sat = hsv[..., 0]*360, hsv[..., 1]
    lum = (rgb@[.299, .587, .114])/255
    used = lum > .03
    lo, hi = np.percentile(lum[used], 2), np.percentile(lum[used], 99)
    t = np.clip((lum-lo)/max(hi-lo, 1e-3), 0, 1)
    t = t**.85  # lift the mid-tones a little so gold reads bright, not brown
    t = t**1.4/(t**1.4+(1-t)**1.4)  # gentle S-curve keeps engraving contrast
    # Gems: clearly saturated and not already gold/brown/red-brown metal.
    warm = smooth(sat, .12, .28)*(1-smooth(hue, 55, 70))*smooth(hue, -1, 3)
    gem = smooth(sat, .4, .6)*(1-warm)
    gem_rgb = np.clip(rgb*1.15+12, 0, 255)
    if style == 'gold':
        metal = ramp(GOLD, t)
    else:
        neutral = 1-smooth(sat, .1, .24)
        metal = ramp(SILVER, t)*neutral[..., None]+ramp(GOLD, t)*(1-neutral)[..., None]
    out = metal*(1-gem)[..., None]+gem_rgb*gem[..., None]
    out = np.where(used[..., None], out, rgb)
    return Image.fromarray(np.clip(out+.5, 0, 255).astype(np.uint8))


def meshes(raw):
    """MOD chunks -> (positions, normals, uvs) per chunk, weapon standing upright (as preview_native)."""
    _, chunks = parse(raw)
    out = []
    for c in chunks:
        P = np.array(c['points'], float)
        k = np.array(c['corners'], float)
        idx = k[:, 0].astype(int)
        pos, nrm, uv = P[idx], k[:, 1:4], k[:, 4:6]
        pos = np.column_stack((-pos[:, 1], pos[:, 0], pos[:, 2]))
        nrm = np.column_stack((-nrm[:, 1], nrm[:, 0], nrm[:, 2]))
        tri = pos.reshape(-1, 3, 3)
        dots = (np.cross(tri[:, 1]-tri[:, 0], tri[:, 2]-tri[:, 0])*nrm.reshape(-1, 3, 3).mean(1)).sum(1)
        if np.median(dots) < 0:
            order = [0, 2, 1]
            pos = pos.reshape(-1, 3, 3)[:, order].reshape(-1, 3)
            nrm = nrm.reshape(-1, 3, 3)[:, order].reshape(-1, 3)
            uv = uv.reshape(-1, 3, 2)[:, order].reshape(-1, 2)
        out.append((c['texture'], pos, nrm, uv))
    return out


def write_glb(path, parts, images):
    blobs, views, accessors, gmeshes, gimages, cursor = [], [], [], [], [], 0

    def chunk(data):
        nonlocal cursor
        pad = (-cursor) % 4
        blobs.append(bytes(pad))
        cursor += pad
        views.append(dict(buffer=0, byteOffset=cursor, byteLength=len(data)))
        blobs.append(data)
        cursor += len(data)
        return len(views)-1

    def acc(a, typ):
        a = np.asarray(a, dtype='<f4')
        accessors.append(dict(bufferView=chunk(a.tobytes()), componentType=5126, count=len(a), type=typ,
                              min=a.min(0).tolist(), max=a.max(0).tolist()))
        return len(accessors)-1
    for material, (p, n, u) in parts:
        gmeshes.append(dict(primitives=[dict(material=material, attributes=dict(
            POSITION=acc(p, 'VEC3'), NORMAL=acc(n, 'VEC3'), TEXCOORD_0=acc(u, 'VEC2')))]))
    for im in images:
        b = BytesIO()
        im.save(b, format='PNG')
        gimages.append(dict(bufferView=chunk(b.getvalue()), mimeType='image/png'))
    j = dict(asset=dict(version='2.0', generator='Golden Knight weapon concepts'), scene=0,
             scenes=[dict(nodes=list(range(len(gmeshes))))], nodes=[dict(mesh=i) for i in range(len(gmeshes))],
             meshes=gmeshes, buffers=[dict(byteLength=cursor)], bufferViews=views, accessors=accessors, images=gimages,
             textures=[dict(source=i, sampler=0) for i in range(len(images))], samplers=[dict(wrapS=10497, wrapT=10497)],
             materials=[dict(doubleSided=True, pbrMetallicRoughness=dict(baseColorTexture=dict(index=i), metallicFactor=0,
                                                                           roughnessFactor=.75)) for i in range(len(images))])
    jb = json.dumps(j).encode()
    jb += b' '*((-len(jb)) % 4)
    bb = b''.join(blobs)
    bb += bytes((-len(bb)) % 4)
    path.write_bytes(struct.pack('<3I', 0x46546c67, 2, 28+len(jb)+len(bb))+struct.pack('<I4s', len(jb), b'JSON')+jb
                     + struct.pack('<I4s', len(bb), b'BIN\0')+bb)


def main():
    read = resource_reader(R/'client-overlay')
    (O/'glb').mkdir(parents=True, exist_ok=True)
    catalog = []
    for kind, prefix, item, name in CANDIDATES:
        chunks = meshes(read(f'Equip/{prefix}_1.mod'))
        # A few models lie along another axis; stand every weapon up along its longest extent.
        axis = int(np.argmax(np.ptp(np.concatenate([p for _, p, _, _ in chunks]), 0)))
        if axis != 1:
            turn = (lambda a: np.column_stack((a[:, 1], -a[:, 0], a[:, 2]))) if axis == 0 else                    (lambda a: np.column_stack((a[:, 0], a[:, 2], -a[:, 1])))
            chunks = [(t, turn(pp), turn(nn), uu) for t, pp, nn, uu in chunks]
        textures = sorted({t for t, *_ in chunks})
        sources = {t: Image.open(BytesIO(zlib.decompress(read('Texture/Equip/'+Path(t).with_suffix('.wtm').name)[13:]))).convert('RGB')
                   for t in textures}
        for style in STYLES:
            images = [recolour(sources[t], style) for t in textures]
            parts = [(textures.index(t), (p, n, u)) for t, p, n, u in chunks]
            write_glb(O/'glb'/f'{prefix}-{style}.glb', parts, images)
        height = float(np.ptp(np.concatenate([p for _, p, _, _ in chunks])[:, 1]))
        catalog.append(dict(kind=kind, key=prefix, item=item, name=name, height=round(height, 3), textures=textures))
    (O/'catalog.json').write_text(json.dumps(catalog, ensure_ascii=False, indent=1)+'\n', encoding='utf-8')
    print(json.dumps([(c['key'], c['height'], c['textures']) for c in catalog], ensure_ascii=False))


if __name__ == '__main__':
    main()
