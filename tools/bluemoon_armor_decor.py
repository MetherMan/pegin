"""Geometry-aware decoration for the Blue Moon armour textures (rank-9 look).

Uses the native armour meshes to find where each texel lands on the body:
- back mask: texels on back-facing torso triangles (their own tone pass, so a darker painted
  back plate reaches the same blue as the front),
- piece outline: the UV island border of every armour piece (luminous light-blue rim),
- chest/back anchors: the surface point on the body centre line, where a horns-up crescent and
  small star are painted in the surface's own scale and orientation. The crescent is left-right
  symmetric, so mirrored halves (the male back plate) show it whole.
"""
import math
import numpy as np
from PIL import Image, ImageDraw, ImageFilter


def triangles(parts, name):
    out = []
    for chunks in parts.values():
        for c in chunks:
            if c['texture'] != name:
                continue
            P = np.array(c['points'], float)
            cs = c['corners']
            for f in range(0, len(cs), 3):
                k = cs[f:f+3]
                out.append(dict(pos=np.array([P[x[0]] for x in k]), uv=np.array([x[4:6] for x in k], float),
                                n=np.mean([x[1:4] for x in k], 0), ns=np.array([x[1:4] for x in k], float),
                                verts=len(P)))
    return out


def raster(tris, size, keep=lambda t: True):
    w, h = size
    img = Image.new('L', size, 0)
    draw = ImageDraw.Draw(img)
    for t in tris:
        if keep(t):
            draw.polygon([(u*w, v*h) for u, v in t['uv']], fill=255)
    return np.asarray(img, float)/255


def back_mask(tris, size):
    m = raster(tris, size, lambda t: t['n'][2] > .3)
    return np.asarray(Image.fromarray((m*255).astype(np.uint8)).filter(ImageFilter.GaussianBlur(2)), float)/255


def anchor(tris, facing, yfrac):
    """Surface point on x=0 at the given height of the facing (-1 front, +1 back) torso triangles."""
    cand = [t for t in tris if facing*t['n'][2] > .45 and t['verts'] == max(x['verts'] for x in tris)]
    if not cand:
        return None
    ys = np.concatenate([t['pos'][:, 1] for t in cand])
    y = ys.min()+yfrac*(ys.max()-ys.min())
    best = None
    for t in cand:
        a, b, c = t['pos'][:, :2]
        m = np.array([b-a, c-a]).T
        if abs(np.linalg.det(m)) < 1e-9:
            continue
        l1, l2 = np.linalg.solve(m, np.array([0, y])-a)
        inside = min(l1, l2, 1-l1-l2)
        if best is None or inside > best[0]:
            best = (inside, t, l1, l2)
    _, t, l1, l2 = best
    uv = t['uv'][0]+l1*(t['uv'][1]-t['uv'][0])+l2*(t['uv'][2]-t['uv'][0])
    e = np.array([t['pos'][1]-t['pos'][0], t['pos'][2]-t['pos'][0]]).T
    duv = np.array([t['uv'][1]-t['uv'][0], t['uv'][2]-t['uv'][0]]).T

    def direction(d):
        coeff = np.linalg.lstsq(e, d, rcond=None)[0]
        return duv@coeff
    return uv, direction(np.array([1.0, 0, 0])), direction(np.array([0, 1.0, 0]))


def ramp(stops, t):
    t = np.clip(t, 0, 1)
    xs = [s[0] for s in stops]
    cols = np.array([[int(s[1][i:i+2], 16) for i in (1, 3, 5)] for s in stops], float)
    return np.stack([np.interp(t, xs, cols[:, k]) for k in range(3)], -1)


def emblem(rgb, where, radius):
    """Horns-up crescent with a small four-point star between the horns, plus a soft glow."""
    h, w, _ = rgb.shape
    uv, ur, uu = where
    m = np.array([ur*w, uu*h]).T  # pixels per model unit, columns: right, up
    if abs(np.linalg.det(m)) < 1e-9:
        return rgb
    inv = np.linalg.inv(m)
    yy, xx = np.mgrid[:h, :w]
    d = np.stack([xx+.5-uv[0]*w, yy+.5-uv[1]*h], -1)@inv.T  # model-unit offsets (x right, y up)
    x, y = d[..., 0]/radius, d[..., 1]/radius
    outer = np.hypot(x, y)-1
    inner = np.hypot(x, y-.32)-.8
    moon = np.maximum(outer, -inner)
    sx, sy = np.abs(x), np.abs(y-.62)
    star = np.sqrt(sx)+np.sqrt(sy)-np.sqrt(.26)
    shape = np.minimum(moon, star)
    px = 1.5/np.linalg.norm(m, axis=0).mean()/radius  # about one texel in emblem units
    fill = np.clip(.5-shape/px, 0, 1)
    # A dark navy keyline just outside the shape keeps it readable on near-white plates too.
    edge = np.clip(.5-(shape-.13)/px, 0, 1)*(1-fill)
    core = ramp([(0, '#2f9cff'), (.45, '#8fdcff'), (1, '#f4fcff')], np.clip(-shape/.22, 0, 1))
    glow = np.exp(-np.maximum(shape-.13, 0)/.3)*(shape > .13)*.55
    out = rgb+(255-rgb)*(glow[..., None]*np.array([.2, .55, 1.0]))
    out = out*(1-.85*edge[..., None])+np.array([8, 22, 70])*.85*edge[..., None]
    out = out*(1-fill[..., None])+core*fill[..., None]
    return out


def outline(rgb, island):
    """Luminous light-blue rim just inside every armour piece border."""
    solid = Image.fromarray((island > .5).astype(np.uint8)*255)
    inner = np.asarray(solid.filter(ImageFilter.MinFilter(3)), float)/255
    rim = np.clip(island-inner, 0, 1)
    rim = np.asarray(Image.fromarray((rim*255).astype(np.uint8)).filter(ImageFilter.MaxFilter(3)), float)/255*(island > .5)
    soft = np.asarray(Image.fromarray((rim*255).astype(np.uint8)).filter(ImageFilter.GaussianBlur(1.2)), float)/255
    colour = ramp([(0, '#3aa8ff'), (1, '#dff6ff')], rim)
    a = np.clip(rim*.85+soft*.35, 0, .9)[..., None]
    return rgb*(1-a)+colour*a


def normal_map(tris, size):
    """Per-texel surface normal (interpolated corner normals) and coverage for this texture."""
    w, h = size
    normals = np.zeros((h, w, 3))
    covered = np.zeros((h, w), bool)
    for t in tris:
        px = t['uv']*np.array([w, h])
        lo = np.floor(px.min(0)).astype(int).clip(0, [w-1, h-1])
        hi = np.ceil(px.max(0)).astype(int).clip(0, [w-1, h-1])
        if (hi < lo).any():
            continue
        yy, xx = np.mgrid[lo[1]:hi[1]+1, lo[0]:hi[0]+1]
        a, b, c = px
        m = np.array([b-a, c-a]).T
        if abs(np.linalg.det(m)) < 1e-9:
            continue
        rel = np.stack([xx+.5-a[0], yy+.5-a[1]], -1)@np.linalg.inv(m).T
        l1, l2 = rel[..., 0], rel[..., 1]
        inside = (l1 >= -.02) & (l2 >= -.02) & (l1+l2 <= 1.02)
        n = (1-l1-l2)[..., None]*t['ns'][0]+l1[..., None]*t['ns'][1]+l2[..., None]*t['ns'][2]
        n /= np.maximum(np.linalg.norm(n, axis=-1, keepdims=True), 1e-9)
        region = normals[lo[1]:hi[1]+1, lo[0]:hi[0]+1]
        region[inside] = n[inside]
        covered[lo[1]:hi[1]+1, lo[0]:hi[0]+1] |= inside
    return normals, covered


def polish(rgb, normals, covered):
    """Even sky light from above (front and back alike, so the back never darkens) plus a soft sheen
    seen from the front or the back. Left-right symmetric, so mirrored UVs agree."""
    sky = normals[..., 1]*.5+.5
    factor = np.where(covered, .86+.3*sky, 1)[..., None]
    out = rgb*factor
    spec = 0
    for view in (np.array([0, .25, -.97]), np.array([0, .25, .97])):
        light = np.array([0, .8, view[2]*.6])
        half = (light/np.linalg.norm(light)+view)
        half /= np.linalg.norm(half)
        spec = np.maximum(spec, np.clip(normals@half, 0, 1)**18)
    sheen = np.where(covered, spec*.34, 0)[..., None]
    return out+(np.array([215, 235, 255.0])-out)*sheen


def position_map(tris, size):
    """Per-texel bind-pose position and face normal (interpolated) for this texture."""
    w, h = size
    pos = np.zeros((h, w, 3))
    nrm = np.zeros((h, w, 3))
    covered = np.zeros((h, w), bool)
    for t in tris:
        px = t['uv']*np.array([w, h])
        lo = np.floor(px.min(0)).astype(int).clip(0, [w-1, h-1])
        hi = np.ceil(px.max(0)).astype(int).clip(0, [w-1, h-1])
        a, b, c = px
        m = np.array([b-a, c-a]).T
        if abs(np.linalg.det(m)) < 1e-9:
            continue
        yy, xx = np.mgrid[lo[1]:hi[1]+1, lo[0]:hi[0]+1]
        rel = np.stack([xx+.5-a[0], yy+.5-a[1]], -1)@np.linalg.inv(m).T
        l1, l2 = rel[..., 0], rel[..., 1]
        inside = (l1 >= -.02) & (l2 >= -.02) & (l1+l2 <= 1.02)
        w0 = (1-l1-l2)[..., None]
        p = w0*t['pos'][0]+l1[..., None]*t['pos'][1]+l2[..., None]*t['pos'][2]
        n = w0*t['ns'][0]+l1[..., None]*t['ns'][1]+l2[..., None]*t['ns'][2]
        pos[lo[1]:hi[1]+1, lo[0]:hi[0]+1][inside] = p[inside]
        nrm[lo[1]:hi[1]+1, lo[0]:hi[0]+1][inside] = n[inside]
        covered[lo[1]:hi[1]+1, lo[0]:hi[0]+1] |= inside
    return pos, nrm, covered


def emblem3d(rgb, tris, facing, height, radius):
    """Crescent + star painted from the body's own surface: every texel is placed by its 3D position
    projected onto the front (or back) view, so the emblem keeps its round shape however the UVs are
    stretched. Only texels on the facing side of the main torso piece near the anchor are painted."""
    main = [t for t in tris if t['verts'] == max(x['verts'] for x in tris)]
    where = anchor(main, facing, height)
    if where is None:
        return rgb
    # Anchor point in 3D: the x=0 surface point at the requested height on the facing side.
    cand = [t for t in main if facing*t['n'][2] > .45]
    ys = np.concatenate([t['pos'][:, 1] for t in cand])
    y0 = ys.min()+height*(ys.max()-ys.min())
    h, w, _ = rgb.shape
    pos, nrm, covered = position_map(main, (w, h))
    x = pos[..., 0]*(-facing)/radius  # mirrored the same way on both sides; the shape is symmetric
    y = (pos[..., 1]-y0)/radius
    on_side = covered & (facing*nrm[..., 2] > .2)
    outer = np.hypot(x, y)-1
    inner = np.hypot(x, y-.32)-.8
    moon = np.maximum(outer, -inner)
    star = np.sqrt(np.abs(x))+np.sqrt(np.abs(y-.62))-np.sqrt(.26)
    shape = np.minimum(moon, star)
    texel = 1.2/(radius*max(w, h))*.9  # roughly one texel in emblem units for this atlas
    px = max(texel, .035)
    fill = np.clip(.5-shape/px, 0, 1)*on_side
    edge = np.clip(.5-(shape-.13)/px, 0, 1)*(1-np.clip(.5-shape/px, 0, 1))*on_side
    core = ramp([(0, '#2f9cff'), (.45, '#8fdcff'), (1, '#f4fcff')], np.clip(-shape/.22, 0, 1))
    glow = np.exp(-np.maximum(shape-.13, 0)/.3)*(shape > .13)*.55*on_side
    # Medallion backdrop: a deep navy disc with a thin light-blue ring clears the plate's own
    # engraving from behind the crescent so it reads cleanly.
    r = np.hypot(x, y)
    disc = np.clip(.5-(r-1.28)/px, 0, 1)*on_side
    backdrop = ramp([(0, '#0d2a6e'), (1, '#050c26')], np.clip(r/1.28, 0, 1))
    ring = np.clip(1-np.abs(r-1.2)/(px*1.2), 0, 1)*on_side
    out = rgb*(1-disc[..., None])+backdrop*disc[..., None]
    out = out*(1-ring[..., None])+np.array([111, 184, 255.0])*ring[..., None]
    glow = glow*(r < 1.15)
    out = out+(255-out)*(glow[..., None]*np.array([.2, .55, 1.0]))
    out = out*(1-.85*edge[..., None])+np.array([8, 22, 70])*.85*edge[..., None]
    out = out*(1-fill[..., None])+core*fill[..., None]
    return out
