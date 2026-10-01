"""Inventory icons for the Blue Moon armour, rendered from the real game models.

Each icon is a shot of the new MOD (with its approved textures) on the black tile with the 1px gold
frame used by the Black Knight armour and the Twilight weapons. Rendered at 8x and box-filtered
to the native 28x28 sprite. Writes assets/blue-moon-armor/icons/*.png.
"""
from pathlib import Path
from io import BytesIO
import sys, zlib
R = Path(__file__).resolve().parents[1]
sys.path[:0] = [str(R/'runtime/pylibs'), str(R/'tools')]
import numpy as np
from PIL import Image, ImageFilter
import bluemoon_armor_native as nat

O = R/'assets/blue-moon-armor/icons'
SS = 8
# Per part: side to keep (x sign, None = whole model), yaw, pitch (degrees), shown height range
# (fractions from the top of the model). The models are in the bind T-pose, so the torso shot
# lowers the upper arms 62 degrees around the shoulder joint and leaves out forearms and hands,
# like the original armour icons which show only the shoulders and chest.
VIEWS = {'tor': (None, 0, 6, (0, 1)), 'leg': (None, 0, 4, (0, .62)), 'gun': (-1, 35, 25, (0, 1)),
         'boo': (None, 18, 6, (0, 1))}
ARM_DROP = 62


def wtm_image(raw):
    return Image.open(BytesIO(zlib.decompress(raw[13:]))).convert('RGB')


def frame():
    bk = np.asarray(wtm_image((nat.GAME/'Item/ma_tor_b032.wtm').read_bytes()), int)
    tw = np.asarray(wtm_image((R/'client-overlay/Item/mt_twilight_icon.wtm').read_bytes()), int)
    gold = (bk[..., 0] > 150) & (bk[..., 0] > bk[..., 2]+60) & (np.abs(bk-tw).max(-1) == 0)
    return bk, gold


def rotation(yaw, pitch):
    y, p = np.radians(yaw), np.radians(pitch)
    ry = np.array([[np.cos(y), 0, np.sin(y)], [0, 1, 0], [-np.sin(y), 0, np.cos(y)]])
    rx = np.array([[1, 0, 0], [0, np.cos(p), -np.sin(p)], [0, np.sin(p), np.cos(p)]])
    return rx@ry


def lower_arms(c, bones):
    """Points/normals with each upper arm turned down around its own shoulder joint."""
    names = [b['name'] for b in bones]
    rest = {n: np.array(b['rest'][12:15], float) for n, b in zip(names, bones)}
    P = np.array(c['points'], float)
    N = {}
    for side in ('L', 'R'):
        joint, elbow = rest[f'Bip01 {side} UpperArm'], rest[f'Bip01 {side} Forearm']
        a = np.radians(ARM_DROP)*(-1 if elbow[0] > joint[0] else 1)
        m = np.array([[np.cos(a), -np.sin(a), 0], [np.sin(a), np.cos(a), 0], [0, 0, 1]])
        ids = {i for i, n in enumerate(names) if n.startswith(f'Bip01 {side} ') and 'Clavicle' not in n}
        sel = np.array([b in ids for b in c['bones']])
        P[sel] = (P[sel]-joint)@m.T+joint
        N[side] = (ids, m)
    return P, N


def render(chunks, textures, side, yaw, pitch, size, band=(0, 1), bones=None):
    rot = rotation(yaw, pitch)
    tris = []
    drop = {i for i, b in enumerate(bones or []) if any(k in b['name'] for k in ('Forearm', 'Hand', 'Finger'))}
    for c in chunks:
        P, turned = lower_arms(c, bones) if bones else (np.array(c['points'], float), {})
        tex = textures[c['texture'].lower()]
        for f in range(0, len(c['corners']), 3):
            k = c['corners'][f:f+3]
            ids = [x[0] for x in k]
            if bones and any(c['bones'][i] in drop for i in ids):
                continue
            pos = P[ids]
            if side is not None and np.sign(pos[:, 0].mean()) != side:
                continue
            nor = np.array([x[1:4] for x in k], float)
            for _, (sel, m) in turned.items():
                nor = np.array([m@n if c['bones'][i] in sel else n for n, i in zip(nor, ids)])
            tris.append((pos@rot.T, nor@rot.T, np.array([x[4:6] for x in k]), tex))
    allp = np.concatenate([t[0] for t in tris])
    # Game front is -Z, up +Y: look along +Z, screen x = -X so the model faces the viewer.
    sx, sy = -allp[:, 0], -allp[:, 1]
    top, bottom = sy.min(), sy.max()
    y0, y1 = top+(bottom-top)*band[0], top+(bottom-top)*band[1]
    keep = (sy >= y0-1e-6) & (sy <= y1+1e-6)
    lo = np.array([sx[keep].min(), y0])
    hi = np.array([sx[keep].max(), y1])
    scale = (size*.94)/max(hi-lo)
    offset = size/2-(lo+hi)/2*scale
    depth = np.full((size, size), np.inf)
    rgb = np.zeros((size, size, 3))
    light = np.array([-.35, -.55, -.75])
    light /= np.linalg.norm(light)
    for pos, nor, uv, tex in tris:
        xy = np.stack([-pos[:, 0], -pos[:, 1]], 1)*scale+offset
        x0, y0 = np.floor(xy.min(0)).astype(int)
        x1, y1 = np.ceil(xy.max(0)).astype(int)
        x0, y0, x1, y1 = max(x0, 0), max(y0, 0), min(x1, size-1), min(y1, size-1)
        if x1 < x0 or y1 < y0:
            continue
        gx, gy = np.meshgrid(np.arange(x0, x1+1)+.5, np.arange(y0, y1+1)+.5)
        (ax, ay), (bx, by), (cx, cy) = xy
        den = (by-cy)*(ax-cx)+(cx-bx)*(ay-cy)
        if abs(den) < 1e-12:
            continue
        w0 = ((by-cy)*(gx-cx)+(cx-bx)*(gy-cy))/den
        w1 = ((cy-ay)*(gx-cx)+(ax-cx)*(gy-cy))/den
        w2 = 1-w0-w1
        inside = (w0 >= 0) & (w1 >= 0) & (w2 >= 0)
        if not inside.any():
            continue
        z = w0*pos[0, 2]+w1*pos[1, 2]+w2*pos[2, 2]
        ys, xs = np.nonzero(inside)
        zz = z[ys, xs]
        py, px = ys+y0, xs+x0
        closer = zz < depth[py, px]
        if not closer.any():
            continue
        ys, xs, py, px, zz = ys[closer], xs[closer], py[closer], px[closer], zz[closer]
        b = np.stack([w0[ys, xs], w1[ys, xs], w2[ys, xs]], 1)
        u, v = (b@uv).T
        n = b@nor
        n /= np.linalg.norm(n, axis=1, keepdims=True)+1e-9
        if (n@[0, 0, -1]).mean() < 0:
            n = -n  # two-sided: the game draws these meshes without back-face lighting differences
        w, h = tex.shape[1], tex.shape[0]
        col = tex[np.clip((v % 1)*h, 0, h-1).astype(int), np.clip((u % 1)*w, 0, w-1).astype(int)]
        shade = .75+.65*np.clip(n@light, 0, 1)
        rgb[py, px] = col*shade[:, None]
        depth[py, px] = zz
    covered = np.isfinite(depth)
    return np.clip(rgb, 0, 255), covered


def icon(chunks, textures, part, bones):
    side, yaw, pitch, band = VIEWS[part]
    bk, gold = frame()
    inner = 26
    rgb, covered = render(chunks, textures, side, yaw, pitch, inner*SS, band, bones if part == 'tor' else None)
    big = Image.fromarray(rgb.astype(np.uint8))
    small = np.asarray(big.resize((inner, inner), Image.BOX), float)
    alpha = np.asarray(Image.fromarray((covered*255).astype(np.uint8)).resize((inner, inner), Image.BOX), float)/255
    tile = np.zeros((28, 28, 3))
    # Box filtering already darkened edge pixels toward the black tile; keep them as rendered.
    tile[1:27, 1:27] = small
    tile = np.where(gold[..., None], bk, tile)
    out = Image.fromarray(np.clip(tile+.5, 0, 255).astype(np.uint8))
    sharp = out.filter(ImageFilter.UnsharpMask(1, 50, 2))
    final = np.where(gold[..., None], bk, np.asarray(sharp))
    return Image.fromarray(final.astype(np.uint8)), alpha


def build(files, bones_by_sex):
    """files: {relative client path: bytes} from bluemoon_armor_native.build."""
    O.mkdir(parents=True, exist_ok=True)
    icons = {}
    for prefix in ('ma', 'fe'):
        for part in nat.PARTS:
            _, chunks = nat.parse(files[f'Body/High/{prefix}_{part}_{nat.NUM}_1.mod'])
            textures = {}
            for c in chunks:
                key = c['texture'].lower()
                rel = 'Texture/Body/'+Path(c['texture']).with_suffix('.wtm').name
                raw = files.get(rel) or (nat.GAME/rel).read_bytes()
                textures[key] = np.asarray(wtm_image(raw), float)
            image, _ = icon(chunks, textures, part, bones_by_sex['0' if prefix == 'ma' else '1'])
            image.save(O/f'{prefix}_{part}.png')
            icons[f'{prefix}_{part}'] = image
    return icons
