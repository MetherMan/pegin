"""Blue Moon horse preview built on the original brown horse rig.

Recolours the original horse21 atlas (coat, sky-blue mane/tail, glowing blue
eyes), adds a spiral unicorn horn skinned to the native head bone and volumetric
hair locks (mane with a darker under-layer, forelock, tail) skinned to the neck, head and
tail bones. Preview only: the model
and textures are written under assets/blue-moon-horse and nothing is
registered, installed or added to the father-update manifest.
"""
from pathlib import Path
from io import BytesIO
import hashlib, json, math, struct, sys, zlib

R = Path(__file__).resolve().parents[1]
sys.path[:0] = [str(R/'runtime/pylibs'), str(R/'client-overlay/Tools/SkillColors')]
import numpy as np
from PIL import Image, ImageDraw, ImageFilter
from native_actor import model, animation
from native_assets import png_from_wtm

O = R/'assets/blue-moon-horse'
C = R/'runtime/client/GameClient'
SIZE = 1024
HEAD_BONE = 8
TAIL_BONES = {29, 30, 31}

# Palettes: (position 0..1 of normalised source luminance, colour).
COATS = {
    'midnight': [(0, '#070b1c'), (.30, '#16285a'), (.62, '#31518f'), (.85, '#5f84c2'), (1, '#b4cdf5')],
    'moonlight': [(0, '#2a3651'), (.30, '#5d7399'), (.62, '#9ab0d2'), (.85, '#c6d5ec'), (1, '#eef4ff')],
    'twilight': [(0, '#0c0922'), (.30, '#262259'), (.62, '#474896'), (.85, '#7a82c8'), (1, '#c8cff5')],
}
MANE = [(0, '#12497e'), (.35, '#3f9ad8'), (.65, '#78c8f6'), (.88, '#b7e6ff'), (1, '#effaff')]
SADDLE_CLOTH = [(0, '#05081a'), (.4, '#132456'), (.75, '#29488c'), (1, '#6f90cc')]
SADDLE_STRIPE = [(0, '#2b4a7c'), (.5, '#6f9fd6'), (1, '#bfe0ff')]
SEAT = [(0, '#060914'), (.5, '#1c2744'), (1, '#5c6f98')]

# Hand-traced source regions on the original 256px atlas (scaled x4).
FORELOCK = [(1, 30), (3, 20), (10, 12), (20, 8), (33, 7), (35, 13), (28, 18), (24, 26), (18, 31), (8, 34), (2, 34)]
NECK_MANE = [(50, 16), (56, 11), (66, 12), (76, 17), (86, 24), (96, 30), (104, 34), (104, 44), (98, 50), (88, 50),
             (78, 46), (70, 40), (62, 32), (56, 26), (52, 22)]
TAIL = [(206, 62), (214, 55), (228, 53), (242, 56), (252, 62), (256, 70), (256, 166), (250, 166), (244, 150),
        (238, 125), (236, 100), (232, 84), (224, 72), (214, 68)]
BLANKET = [(103, 37), (114, 37), (125, 40), (128, 42), (135, 43), (139, 49), (145, 52), (152, 53), (177, 53),
           (178, 60), (178, 96), (170, 98), (104, 98), (102, 90), (102, 40)]
EYE = (136, 141, 26, 18)  # centre x, centre y, radius x, radius y at 1024px
EYE_GLOW = (138, 141, 19, 8.5, .28)  # almond centre, radii and slant of the painted eye


def ramp(stops, t):
    t = np.clip(t, 0, 1)
    xs = [s[0] for s in stops]
    cols = np.array([[int(s[1][i:i+2], 16) for i in (1, 3, 5)] for s in stops], float)
    return np.stack([np.interp(t, xs, cols[:, k]) for k in range(3)], -1)


def normalise(lum, mask, lo=2, hi=98):
    values = lum[mask > .5]
    a, b = np.percentile(values, [lo, hi])
    return np.clip((lum-a)/max(b-a, 1), 0, 1)


def polygon_mask(points, blur):
    img = Image.new('L', (SIZE, SIZE), 0)
    ImageDraw.Draw(img).polygon([(x*4, y*4) for x, y in points], fill=255)
    return np.asarray(img.filter(ImageFilter.GaussianBlur(blur)), float)/255


def uv_masks(chunks):
    masks = {k: Image.new('L', (SIZE, SIZE), 0) for k in ('body', 'tail', 'tack')}
    for ci, chunk in enumerate(chunks):
        cs = chunk['corners']
        for f in range(0, len(cs), 3):
            tri = [(cs[f+k][4]*SIZE, cs[f+k][5]*SIZE) for k in range(3)]
            bones = {chunk['bones'][cs[f+k][0]] for k in range(3)}
            key = 'tack' if ci == 1 else 'tail' if bones & TAIL_BONES else 'body'
            ImageDraw.Draw(masks[key]).polygon(tri, fill=255)
    return masks


def free_rect(used, w, h):
    """First unused atlas rectangle (x, y); the rectangle is then reserved in `used`."""
    grown = np.asarray(used.filter(ImageFilter.MaxFilter(17)), bool)
    integral = np.pad(grown.cumsum(0).cumsum(1), ((1, 0), (1, 0)))
    for x in range(8, SIZE-w-8, 4):
        for y in range(8, SIZE-h-8, 4):
            if integral[y+h, x+w]-integral[y, x+w]-integral[y+h, x]+integral[y, x] == 0:
                ImageDraw.Draw(used).rectangle((x, y, x+w, y+h), fill=255)
                return x, y
    raise RuntimeError('no free atlas space')


# Horn geometry, in the horse bind pose (model space, horse faces -Z, +Y up).
HORN_BASE = np.array([0.0, 1.972, -1.796])
HORN_AXIS = np.array([0.0, 0.64, -0.77])
HORN_LENGTH = 0.46
HORN_RADIUS = 0.048
HORN_RINGS, HORN_SIDES, HORN_TURNS, HORN_STARTS, HORN_RIDGE = 16, 16, 4.5, 2, 0.10
HORN_RECT = (84, 300)  # texture width (circumference) x height (length) in pixels


def horn_basis():
    d = HORN_AXIS/np.linalg.norm(HORN_AXIS)
    up = np.array([0, 1.0, 0])-d*d[1]
    up /= np.linalg.norm(up)
    return d, up, np.cross(d, up)


def horn_radius(t, theta):
    phase = HORN_STARTS*theta+2*math.pi*HORN_TURNS*t
    return HORN_RADIUS*(1-t)**0.8*(1+HORN_RIDGE*math.cos(phase))


def horn_mesh(first_index, rect):
    """Points and native corners (index, normal, uv) for a closed spiral cone."""
    d, up, side = horn_basis()
    x0, y0 = rect
    w, h = HORN_RECT
    points, rows = [], []
    for i in range(HORN_RINGS):
        t = i/HORN_RINGS
        row = []
        for j in range(HORN_SIDES):
            theta = 2*math.pi*j/HORN_SIDES
            radial = math.cos(theta)*up+math.sin(theta)*side
            p = HORN_BASE+d*(t*HORN_LENGTH)+radial*horn_radius(t, theta)
            row.append(first_index+len(points))
            points.append(p)
        rows.append(row)
    tip = first_index+len(points)
    points.append(HORN_BASE+d*HORN_LENGTH)

    def normal(t, theta):
        radial = math.cos(theta)*up+math.sin(theta)*side
        slope = HORN_RADIUS/HORN_LENGTH
        n = radial+d*slope
        return n/np.linalg.norm(n)

    def uv(j, t):
        return ((x0+0.5+(w-1)*j/HORN_SIDES)/SIZE, (y0+0.5+(h-1)*t)/SIZE)

    corners = []

    def corner(index, j, t):
        theta = 2*math.pi*j/HORN_SIDES
        n = normal(min(t, .999), theta)
        u, v = uv(j, t)
        corners.append((index, *[float(x) for x in n], u, v))

    for i in range(HORN_RINGS):
        t0, t1 = i/HORN_RINGS, (i+1)/HORN_RINGS
        for j in range(HORN_SIDES):
            a, b = rows[i][j], rows[i][(j+1) % HORN_SIDES]
            if i+1 < HORN_RINGS:
                c, e = rows[i+1][j], rows[i+1][(j+1) % HORN_SIDES]
                # Same outward (cross . normal > 0) winding as horse_1.mod.
                for tri in ((a, j, t0), (e, j+1, t1), (b, j+1, t0)), ((a, j, t0), (c, j, t1), (e, j+1, t1)):
                    for index, jj, tt in tri:
                        corner(index, jj, tt)
            else:
                for index, jj, tt in ((a, j, t0), (tip, j+.5, 1), (b, j+1, t0)):
                    corner(index, jj, tt)
    pts = np.array([p for p in points])
    for f in range(0, len(corners), 3):
        k = corners[f:f+3]
        pa, pb, pc = [pts[x[0]-first_index] for x in k]
        if np.dot(np.cross(pb-pa, pc-pa), np.array(k[0][1:4])) < 0:
            corners[f+1], corners[f+2] = corners[f+2], corners[f+1]
    return [list(map(float, p)) for p in points], corners




# Volumetric hair locks (first fluffy version, refined): closed, flattened locks along Bezier
# centrelines with rounded ends, a darker under-layer to fill gaps, and the mane end tucked
# under the saddle pommel.
HAIR_RECT = (128, 256)
LOCK_RINGS, LOCK_SIDES = 7, 5
NECK_BONES, TAIL_BONE_SET, FRONT_BODY = {5, 6, 7, 8}, {29, 30, 31}, {3, 4, 5, 6, 7, 8}
# Bind-pose top midline of the neck from behind the ears to the saddle front (seat front z -0.997).
CREST = np.array([(0, 2.128, -1.640), (0, 2.111, -1.475), (0, 2.086, -1.380), (0, 1.954, -1.190), (0, 1.829, -1.053),
                  (0, 1.800, -1.015)])
# Rein strip where it runs down the side of the neck (z, y); locks stay above it.
REINS = np.array([(-1.62, 1.95), (-1.47, 1.81), (-1.19, 1.59), (-1.04, 1.61)])
UP, BACK, FWD, X = np.array([0, 1.0, 0]), np.array([0, 0, 1.0]), np.array([0, 0, -1.0]), np.array([1.0, 0, 0])
DARK, BRIGHT = (.02, .48), (.52, .98)


class Extra:
    """Geometry appended to chunk 0: model-space points, one bone per point, native corners."""

    def __init__(self, first):
        self.first, self.points, self.bones, self.corners = first, [], [], []

    @property
    def next_index(self):
        return self.first+len(self.points)

    def add(self, points, corners, bones):
        self.points += [list(map(float, p)) for p in points]
        self.corners += corners
        self.bones += [int(b) for b in bones]


def crest(p):
    seg = np.linalg.norm(np.diff(CREST, axis=0), axis=1)
    at = np.concatenate([[0], np.cumsum(seg)])/seg.sum()
    point = np.array([np.interp(p, at, CREST[:, k]) for k in range(3)])
    i = min(np.searchsorted(at, p, side='right')-1, len(seg)-1)
    tangent = CREST[i+1]-CREST[i]
    return point, tangent/np.linalg.norm(tangent)


def bezier(ctrl, s):
    p0, p1, p2, p3 = (np.asarray(c, float) for c in ctrl)
    return (1-s)**3*p0+3*(1-s)**2*s*p1+3*(1-s)*s*s*p2+s**3*p3


def tuck(p):
    """1 along the neck; lower and shorter behind the ears and under the saddle pommel."""
    return min(1, max(0, (1-p)/.15))*(.45+.55*min(1, p/.06))


def reins_room(root):
    return root[1]-np.interp(root[2], REINS[:, 0], REINS[:, 1])-.03


class Surface:
    """Skin binding and push-out against the original horse triangles and the horn."""

    def __init__(self, chunk):
        self.points = np.array(chunk['points'])
        self.bones = np.array(chunk['bones'])
        cs = chunk['corners']
        self.tris = np.array([[cs[f+k][0] for k in range(3)] for f in range(0, len(cs), 3)])

    def bind(self, points, allowed):
        ids = np.where(np.isin(self.bones, list(allowed)))[0]
        d = np.linalg.norm(points[:, None, :]-self.points[None, ids, :], axis=2)
        return self.bones[ids[d.argmin(1)]]

    def closest(self, pts, allowed):
        sel = self.tris[np.all(np.isin(self.bones[self.tris], list(allowed)), axis=1)]
        a, b, c = (self.points[sel[:, k]] for k in range(3))
        n = np.cross(b-a, c-a)
        area = np.linalg.norm(n, axis=1)
        keep = area > 1e-9
        a, b, c, n = a[keep], b[keep], c[keep], n[keep]/area[keep, None]
        q = pts[:, None, :]-((pts[:, None, :]-a[None])*n[None]).sum(-1, keepdims=True)*n[None]
        v0, v1, v2 = b-a, c-a, q-a[None]
        d00, d01, d11 = (v0*v0).sum(1), (v0*v1).sum(1), (v1*v1).sum(1)
        d20, d21 = (v2*v0[None]).sum(-1), (v2*v1[None]).sum(-1)
        den = d00*d11-d01*d01
        v, w = (d11*d20-d01*d21)/den, (d00*d21-d01*d20)/den
        inside = (v >= 0) & (w >= 0) & (v+w <= 1)

        def segment(s0, s1):
            d = s1-s0
            t = np.clip(((pts[:, None, :]-s0[None])*d[None]).sum(-1)/(d*d).sum(1), 0, 1)
            return s0[None]+t[..., None]*d[None]
        edges = [segment(a, b), segment(b, c), segment(c, a)]
        dist = np.stack([np.linalg.norm(pts[:, None, :]-e, axis=-1) for e in edges])
        edge = np.take_along_axis(np.stack(edges), dist.argmin(0)[None, ..., None], 0)[0]
        near = np.where(inside[..., None], q, edge)
        k = np.linalg.norm(pts[:, None, :]-near, axis=-1).argmin(1)
        return near[np.arange(len(pts)), k], n[k]

    def push(self, pts, clearance, allowed, rounds=3):
        out = pts.copy()
        for _ in range(rounds):
            near, normal = self.closest(out, allowed)
            sd = ((out-near)*normal).sum(1)
            low = sd < clearance
            out[low] += normal[low]*(clearance-sd[low])[:, None]
        axis = HORN_AXIS/np.linalg.norm(HORN_AXIS)
        t = np.clip((out-HORN_BASE)@axis, 0, HORN_LENGTH)
        radial = out-(HORN_BASE+axis*t[:, None])
        dist = np.linalg.norm(radial, axis=1)
        need = HORN_RADIUS*(1.1-t/HORN_LENGTH)+clearance
        close = dist < need
        out[close] += radial[close]/np.maximum(dist[close], 1e-6)[:, None]*(need-dist)[close][:, None]
        return out


def lock_s():
    """Ring positions along a lock, denser towards the rounded end."""
    return np.array([1-(1-i/LOCK_RINGS)**1.4 for i in range(LOCK_RINGS)]+[1.0])


def lock_mesh(first, centre, width, w0, th0, rect, u0, u1):
    """Closed flattened lock with a root cap and a rounded (not pointed) end."""
    (x0, y0), (w, h) = rect
    svals = lock_s()
    n = len(centre)-1
    points, rows, frames = [], [], []
    for i in range(n):
        s = svals[i]
        tangent = centre[min(i+1, n)]-centre[max(i-1, 0)]
        tangent = tangent/np.linalg.norm(tangent)
        wd = np.cross(tangent, X) if width is None else width-tangent*np.dot(width, tangent)
        wd = wd/np.linalg.norm(wd)
        hd = np.cross(tangent, wd)
        end = (1-s**1.8)**.75  # soft taper: neither a needle point nor a leaf-like round end
        ws = w0*(.85+.3*math.sin(math.pi*s))*end
        ts = th0*end
        row = []
        for j in range(LOCK_SIDES):
            phi = 2*math.pi*j/LOCK_SIDES
            row.append(first+len(points))
            points.append(centre[i]+wd*math.cos(phi)*ws+hd*math.sin(phi)*ts)
        rows.append(row)
        frames.append((wd, hd, ws, ts, tangent))
    tip = first+len(points)
    points.append(centre[n])
    root = first+len(points)
    points.append(centre[0]-frames[0][4]*th0*.5)

    corners = []

    def corner(index, i, j):
        if index == tip:
            normal = frames[-1][4]
        elif index == root:
            normal = -frames[0][4]
        else:
            wd, hd, ws, ts, _ = frames[min(i, n-1)]
            phi = 2*math.pi*j/LOCK_SIDES
            normal = wd*math.cos(phi)/ws+hd*math.sin(phi)/ts
        normal = normal/np.linalg.norm(normal)
        u = (x0+.5+(w-1)*(u0+(u1-u0)*j/LOCK_SIDES))/SIZE
        v = (y0+.5+(h-1)*svals[min(i, n)])/SIZE
        corners.append((index, *map(float, normal), u, v))

    for i in range(n):
        for j in range(LOCK_SIDES):
            a, b = rows[i][j], rows[i][(j+1) % LOCK_SIDES]
            if i+1 < n:
                c, e = rows[i+1][j], rows[i+1][(j+1) % LOCK_SIDES]
                for tri in ((a, i, j), (e, i+1, j+1), (b, i, j+1)), ((a, i, j), (c, i+1, j), (e, i+1, j+1)):
                    for index, ii, jj in tri:
                        corner(index, ii, jj)
            else:
                for index, ii, jj in ((a, i, j), (tip, n, j+.5), (b, i, j+1)):
                    corner(index, ii, jj)
    for j in range(LOCK_SIDES):
        for index, ii, jj in ((rows[0][j], 0, j), (rows[0][(j+1) % LOCK_SIDES], 0, j+1), (root, 0, j+.5)):
            corner(index, ii, jj)
    pts = np.array(points)
    for f in range(0, len(corners), 3):
        k = corners[f:f+3]
        pa, pb, pc = (pts[x[0]-first] for x in k)
        if np.dot(np.cross(pb-pa, pc-pa), np.array(k[0][1:4])) < 0:
            corners[f+1], corners[f+2] = corners[f+2], corners[f+1]
    return pts, corners


def hair_locks():
    """(control points, width axis or None for sagittal, width, thickness, bones, push-out, tile) per lock."""
    rng = np.random.default_rng(20260930)
    locks = []
    # Darker under-layer close to the neck so no neck shows between the outer locks.
    for p in np.linspace(.02, .98, 10):
        for side in (-1, 1):
            root, tangent = crest(p)
            k, sx = tuck(p), X*side
            length = min((.2+.08*math.sin(math.pi*p))*(.35+.65*k), reins_room(root))
            ctrl = [root+sx*.015-UP*.012, root+UP*.04*k+sx*.05+BACK*.02, root+sx*(.09+.04*k)+BACK*.05,
                    root+sx*(.1+.05*k)-UP*length+BACK*.09]
            locks.append((ctrl, tangent, .08, .016, NECK_BONES, True, DARK))
    # Outer mane falling on both sides of the neck, staggered, with varied length, sweep and flick.
    for p in np.linspace(0, 1, 20):
        for side in (-1, 1):
            q = min(1, p+(.025 if side > 0 else 0))
            root, tangent = crest(q)
            k, sx = tuck(q), X*side
            length = (.25+.12*math.sin(math.pi*min(q*1.2, 1))+rng.uniform(-.04, .04))*(.35+.65*k)
            length = min(length, reins_room(root)+.02)
            ctrl = [root+sx*.02-UP*.012,
                    root+UP*(.06+rng.uniform(0, .03))*k+sx*.05+BACK*.02,
                    root+sx*(.13+.05*k+rng.uniform(0, .02))+UP*.01*k+BACK*(.05+rng.uniform(0, .03)),
                    root+sx*(.14+.07*k+rng.uniform(-.015, .025))-UP*length+BACK*(.09+rng.uniform(0, .07))]
            locks.append((ctrl, tangent, .055, .013, NECK_BONES, True, BRIGHT))
    # Crest locks lying back along the top of the neck (fuller top line, no standing fins).
    for p in np.linspace(0, .94, 12):
        root, _ = crest(p)
        k = tuck(p)
        sx = np.array([rng.uniform(-.03, .03), 0, 0])
        ctrl = [root-UP*.012, root+UP*.06*k+BACK*.03, root+UP*.08*k+BACK*.13+sx,
                root+UP*(.02+rng.uniform(0, .03))*k+BACK*(.2+.06*k)+sx*2]
        locks.append((ctrl, None, .058, .022, NECK_BONES, False, BRIGHT))
    # Forelock: tufts between the ears lying back into the mane, then locks framing the horn.
    for side in (-1, 0, 1):
        root = np.array([side*.03, 2.128, -1.690])
        sx = np.array([side, 0, 0])
        ctrl = [root-UP*.01, root+UP*.06+FWD*.01, root+UP*.09+BACK*.06+sx*.02, root+UP*.05+BACK*.15+sx*.04]
        locks.append((ctrl, None, .055, .024, {8}, False, BRIGHT))
    for side in (-1, 1):
        sx = np.array([side, 0, 0])
        for k in range(3):
            root = np.array([side*(.035+.022*k), 2.125-.01*k, -1.705+.012*k])
            ctrl = [root-UP*.01, root+UP*.05+FWD*.05+sx*.02, root+FWD*.12+sx*(.06+.02*k)-UP*.02,
                    np.array([side*(.075+.025*k), 1.93-.03*k, -1.80])]
            locks.append((ctrl, X, .05, .015, {8}, True, BRIGHT))
    # Fuller tail over the original tail strip, gently waved.
    for _ in range(14):
        x0 = rng.uniform(-.04, .04)
        ctrl = [(x0, 1.665+rng.uniform(-.02, .02), .07+rng.uniform(0, .04)), (x0*1.4, 1.70, .28),
                (x0*2+rng.uniform(-.03, .03), 1.28, .44+rng.uniform(-.03, .03)),
                (x0*2.6+rng.uniform(-.05, .05), .62+rng.uniform(0, .14), .40+rng.uniform(-.02, .07))]
        locks.append((ctrl, X, .06, .02, TAIL_BONE_SET, False, BRIGHT))
    return locks


def add_hair(extra, surface, rect):
    rng = np.random.default_rng(5)
    locks = hair_locks()
    for ctrl, width, w0, th0, allowed, push, (t0, t1) in locks:
        centre = np.array([bezier(ctrl, s) for s in lock_s()])
        if push:
            centre[2:] = surface.push(centre[2:], th0+.014, allowed | FRONT_BODY)
        u0 = rng.uniform(t0, t1-.3)
        points, corners = lock_mesh(extra.next_index, centre, width, w0, th0, rect, u0, u0+.3)
        extra.add(points, corners, surface.bind(points, allowed))
    return dict(locks=len(locks), under_layer=20, outer_mane=40, crest=12, forelock=9, tail=14)


def horn_texture(w, h):
    a = (np.arange(w)[None, :]+.5)/w
    t = (np.arange(h)[:, None]+.5)/h
    theta = 2*math.pi*a
    phase = HORN_STARTS*theta+2*math.pi*HORN_TURNS*t
    crest = np.clip((np.cos(phase)+1)/2, 0, 1)  # 1 on the ridge, 0 in the groove
    light = .66+.34*np.clip(np.cos(theta-.35), 0, 1)**.8+.08*np.clip(np.cos(theta+2.4), 0, 1)
    light = light*(1-t**1.5)+1.08*t**1.5  # the tip reads self-lit from every side
    pearl = ramp([(0, '#7488b0'), (.35, '#b9cbe8'), (.7, '#eaf3ff'), (1, '#ffffff')], t)
    groove = ramp([(0, '#3b5282'), (.5, '#6f97cf'), (.85, '#bfe9ff'), (1, '#f4fdff')], t)
    mix = (crest**.7)[..., None]
    rgb = (pearl*mix+groove*(1-mix))*light[..., None]
    glow = ramp([(0, '#000000'), (.45, '#000000'), (1, '#7fd6ff')], t)*.7
    return np.clip(rgb+glow, 0, 255)


def hair_texture(w, h, seed=11):
    """Soft sky-blue strands along v (root to tip); the left half is the darker under-layer."""
    rng = np.random.default_rng(seed)
    x = np.arange(w)[None, :]+.0
    t = (np.arange(h)[:, None]+.5)/h
    xs = x+2.0*np.sin(t*2*math.pi*1.3+.7)
    fine = .5+.15*np.sin(xs*1.9+1.1)+.11*np.sin(xs*4.3+.3)+.08*np.sin(xs*7.7+2.2)
    fine = fine+np.interp(xs, np.arange(w), np.convolve(rng.uniform(-.2, .2, w), np.ones(2)/2, 'same'))
    sheen = .10*np.exp(-((t-.35)/.14)**2)
    colour = ramp([(0, '#1f5a96'), (.22, '#3b86c6'), (.55, '#69b8ec'), (.85, '#98d4f6'), (1, '#aee0fa')], t)
    shade = (.8+.55*(fine-.5)+sheen)*(.78+.22*np.clip(t/.2, 0, 1))*np.where(x < w//2, .68, 1.0)
    return np.clip(colour*shade[..., None], 0, 255)


def eye_glow(out):
    """Paint a luminous blue almond eye with a soft blue halo on the coat."""
    cx, cy, rx, ry, angle = EYE_GLOW
    yy, xx = np.mgrid[:SIZE, :SIZE]
    dx, dy = xx-cx, yy-cy
    u = (dx*math.cos(angle)+dy*math.sin(angle))/rx
    v = (-dx*math.sin(angle)+dy*math.cos(angle))/ry
    r = np.hypot(u, v)
    core = np.clip((1.08-r)/.22, 0, 1)
    iris = ramp([(0, '#1466d6'), (.45, '#2fa8ff'), (.8, '#aeeaff'), (1, '#ffffff')], np.clip(1-np.hypot(u+.18, v), 0, 1)**.9)
    halo = np.clip(1-np.hypot(u/2.3, v/3.0), 0, 1)**1.8*(1-core)
    out = out*(1-core[..., None])+iris*core[..., None]
    return out+ramp([(0, '#3aa6ff'), (1, '#3aa6ff')], halo)*(halo[..., None]*.85)


def build_textures(source, masks, rects):
    base = source.resize((SIZE, SIZE), Image.Resampling.LANCZOS)
    rgb = np.asarray(base, float)
    lum = rgb@[.299, .587, .114]
    body = np.asarray(masks['body'], float)/255
    tailuv = np.asarray(masks['tail'], float)/255
    tack = np.asarray(masks['tack'].filter(ImageFilter.MaxFilter(9)).filter(ImageFilter.GaussianBlur(2)), float)/255
    mane = np.clip(polygon_mask(FORELOCK, 5)+polygon_mask(NECK_MANE, 6)+polygon_mask(TAIL, 5), 0, 1)
    blanket = polygon_mask(BLANKET, 1.5)
    strap = np.clip((rgb[..., 2]-rgb[..., 0]-2)/8, 0, 1)*(1-blanket)
    strap = np.asarray(Image.fromarray((strap*255).astype(np.uint8)).filter(ImageFilter.GaussianBlur(.8)), float)/255
    yy, xx = np.mgrid[:SIZE, :SIZE]
    ex, ey, rx, ry = EYE
    eye = np.clip(1.4-np.hypot((xx-ex)/rx, (yy-ey)/ry)*1.4, 0, 1)

    coat_t = normalise(lum, body*(1-mane)*(1-strap))**.92
    mane_t = normalise(lum, mane*(tailuv+body > .5)*(1-strap), 3, 99)**.85
    hsv = np.asarray(base.convert('HSV'), float)
    hue, sat = hsv[..., 0]*360/255, hsv[..., 1]/255
    stripe = ((hue > 60) & (hue < 250) & (sat > .35)).astype(float)
    stripe = np.asarray(Image.fromarray((stripe*255).astype(np.uint8)).filter(ImageFilter.GaussianBlur(1)), float)/255
    cloth_t = normalise(lum, blanket)
    seat_t = normalise(lum, tack)

    outputs = {}
    for coat_name, stops in COATS.items():
        coat = ramp(stops, coat_t)
        hair = ramp(MANE, mane_t)
        out = coat*(1-mane[..., None])+hair*mane[..., None]
        out = out*(1-strap[..., None])+rgb*strap[..., None]
        out = out*(1-eye[..., None])+rgb*.9*eye[..., None]
        out = eye_glow(out)
        for saddle in ('original', 'bluemoon'):
            final = out.copy()
            if saddle == 'original':
                cloth, seat = rgb, rgb
            else:
                cloth = ramp(SADDLE_CLOTH, cloth_t)*(1-stripe[..., None])+ramp(SADDLE_STRIPE, cloth_t)*stripe[..., None]
                keep = np.clip((rgb[..., 2]-rgb[..., 0]-2)/8, 0, 1)[..., None]  # navy strap trims stay native
                cloth = cloth*(1-keep)+rgb*keep
                seat = ramp(SEAT, seat_t)*(1-keep)+rgb*keep
            final = final*(1-blanket[..., None])+cloth*blanket[..., None]
            final = final*(1-tack[..., None])+seat*tack[..., None]
            for key, paint in (('horn', horn_texture), ('hair', hair_texture)):
                (x0, y0), (w, h) = rects[key]
                final[y0:y0+h, x0:x0+w] = paint(w, h)
            outputs[(coat_name, saddle)] = Image.fromarray(np.clip(final+.5, 0, 255).astype(np.uint8))
    return outputs


def native_mod(source, chunks, extra, texture_name):
    """Rewrite horse_1.mod with the extra horn/hair geometry appended to chunk 0."""
    raw = bytearray(source)
    out = bytearray(raw[:36])
    at = 36
    for ci, chunk in enumerate(chunks):
        matrix = raw[at:at+64]
        at += 64
        n = struct.unpack_from('<i', raw, at)[0]
        at += 4
        pts = raw[at:at+n*12]
        at += n*12
        faces = struct.unpack_from('<i', raw, at)[0]
        at += 4
        corner_bytes = raw[at:at+faces*72]
        at += faces*72
        material = raw[at:at+16]
        at += 16
        at += 32
        kind = struct.unpack_from('<i', raw, at)[0]
        at += 4
        bones = raw[at:at+n] if kind == 2 else b''
        at += len(bones)
        if ci == 0:
            pts = pts+b''.join(struct.pack('<3f', *p) for p in extra.points)
            corner_bytes = corner_bytes+b''.join(struct.pack('<i5f', *c) for c in extra.corners)
            bones = bones+bytes(extra.bones)
            n += len(extra.points)
            faces += len(extra.corners)//3
        out += matrix+struct.pack('<i', n)+pts+struct.pack('<i', faces)+corner_bytes+material
        out += texture_name.encode().ljust(32, b'\0')+struct.pack('<i', kind)+bones
    assert at == len(raw)
    return bytes(out)


def wtm(image):
    stream = BytesIO()
    image.save(stream, format='BMP')
    bmp = stream.getvalue()
    return b'TEAMMAY\0\0'+struct.pack('<I', len(bmp))+zlib.compress(bmp, 9)


def main():
    source = (C/'Vehicle/horse_1.mod').read_bytes()
    original = model(source)
    atlas = Image.open(BytesIO(png_from_wtm((C/'Texture/Vehicle/horse21.wtm').read_bytes()))).convert('RGB')
    masks = uv_masks(original)
    used = Image.new('L', (SIZE, SIZE), 0)
    for m in masks.values():
        used.paste(255, mask=m)
    rects = {'horn': (free_rect(used, *HORN_RECT), HORN_RECT), 'hair': (free_rect(used, *HAIR_RECT), HAIR_RECT)}

    extra = Extra(len(original[0]['points']))
    horn_points, horn_corners = horn_mesh(extra.next_index, rects['horn'][0])
    extra.add(horn_points, horn_corners, [HEAD_BONE]*len(horn_points))
    horn_count = len(horn_points), len(horn_corners)//3
    hair = add_hair(extra, Surface(original[0]), rects['hair'])
    raw = native_mod(source, original, extra, 'mt_bluemoonhorse.bmp')
    bluemoon = model(raw)
    for a, b in zip(original, bluemoon):
        assert a['matrix'] == b['matrix']
        assert a['corners'] == b['corners'][:len(a['corners'])]
        assert a['bones'] == b['bones'][:len(a['bones'])]
    assert bluemoon[0]['bones'][extra.first:] == extra.bones
    assert max(extra.bones) < 32

    O.mkdir(parents=True, exist_ok=True)
    (O/'payload/Vehicle').mkdir(parents=True, exist_ok=True)
    (O/'payload/Vehicle/mt_bluemoonhorse.mod').write_bytes(raw)
    atlas.save(O/'horse21-original.png')
    textures = {'original': 'horse21-original.png'}
    report_textures = {}
    for (coat, saddle), image in build_textures(atlas, masks, rects).items():
        name = f'atlas-{coat}-{saddle}.png'
        image.save(O/name, optimize=True)
        textures[f'{coat}-{saddle}'] = name
        packed = wtm(image)
        assert Image.open(BytesIO(zlib.decompress(packed[13:]))).tobytes() == image.tobytes()
        report_textures[name] = hashlib.sha256(image.tobytes()).hexdigest()

    ani_raw = (C/'Vehicle/Animation/horse_1.ani').read_bytes()
    n = struct.unpack_from('<i', ani_raw, 32)[0]
    clip = animation(ani_raw, list(range(n)))
    assert len(clip['bones']) == 32 and clip['bones'][HEAD_BONE]['name'] == 'Bip02 Head'
    data = dict(original=original, bluemoon=bluemoon, animation=clip, textures=textures,
                motions={'idle': [1, 101], 'run': [151, 167]})
    (O/'model.json').write_text(json.dumps(data, separators=(',', ':')), encoding='utf-8')
    report = dict(
        passed=True, preview_only=True, registered_as_ride_item=False, installed=False,
        source='Vehicle/horse_1.mod', source_texture='Texture/Vehicle/horse21.wtm',
        source_sha256=hashlib.sha256(source).hexdigest(), model_sha256=hashlib.sha256(raw).hexdigest(),
        original_vertices=sum(len(c['points']) for c in original),
        original_triangles=sum(len(c['corners'])//3 for c in original),
        horn_vertices=horn_count[0], horn_triangles=horn_count[1], horn_bone='Bip02 Head',
        horn_length=HORN_LENGTH, horn_base_radius=HORN_RADIUS,
        hair_parts=hair, hair_vertices=len(extra.points)-horn_count[0],
        hair_triangles=len(extra.corners)//3-horn_count[1],
        hair_bones=sorted({clip['bones'][b]['name'] for b in extra.bones[horn_count[0]:]}),
        total_vertices=sum(len(c['points']) for c in bluemoon),
        total_triangles=sum(len(c['corners'])//3 for c in bluemoon),
        texture_rects={k: [*xy, *wh] for k, (xy, wh) in rects.items()},
        eye='painted luminous blue with a halo (texture only, no emissive material)',
        original_skin_weights_uvs_topology_preserved=True, texture_size=[SIZE, SIZE],
        textures=report_textures,
        note='Preview only. Horn and hair appended to chunk 0 with the native corner layout; not installed.')
    (O/'validation.json').write_text(json.dumps(report, ensure_ascii=False, indent=2)+'\n', encoding='utf-8')
    print(json.dumps({k: report[k] for k in ('hair_parts', 'hair_vertices', 'hair_triangles', 'total_vertices',
                                              'total_triangles', 'hair_bones', 'texture_rects')}, ensure_ascii=False))


if __name__ == '__main__':
    main()
