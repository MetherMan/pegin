"""3D Blue Moon ornaments added to the native armour meshes (rank-9 look).

Each ornament is a closed, thick crescent (horns up): front and back faces plus side walls, in a
separate chunk with its own small texture (`ornament_texture`). Vertices are skinned to the bone of
the armour vertex they sit on, so they follow the native animation:
- a crescent crest standing on each shoulder plate,
- a raised crescent medallion over the painted chest emblem,
- a large crescent floating just behind the back plate.
All positions come from the bind-pose armour geometry (front is -Z, up is +Y).
"""
import math
import numpy as np

C_IN, R_IN = .32, .8  # inner circle of the crescent (unit outer circle, horns up)
TIP = (math.sqrt(1-((1+C_IN**2-R_IN**2)/(2*C_IN))**2), (1+C_IN**2-R_IN**2)/(2*C_IN))
FACE_UV = (.02, .02, .46, .96)   # u0, v0, width, height of the crescent face area
SIDE_UV = (.52, .02, .46, .96)   # side-wall strip


def crescent_outline(n=24):
    a = math.atan2(TIP[1], TIP[0])
    b = math.atan2(TIP[1]-C_IN, TIP[0])
    outer = [(math.cos(t), math.sin(t)) for t in np.linspace(math.pi-a, 2*math.pi+a, n+1)]
    inner = [(R_IN*math.cos(t), C_IN+R_IN*math.sin(t)) for t in np.linspace(math.pi-b, 2*math.pi+b, n+1)]
    return np.array(outer), np.array(inner)


def crescent_mesh(origin, x_axis, y_axis, normal, radius, thickness, bone, first, n=24):
    """Closed thick crescent; returns points, native corners (index, normal, uv), bones."""
    outer, inner = crescent_outline(n)
    half = thickness/2
    points, corners = [], []

    def place(p2, z):
        return origin+radius*(p2[0]*x_axis+p2[1]*y_axis)+z*normal

    def face_uv(p2):
        u0, v0, w, h = FACE_UV
        return u0+w*(p2[0]+1)/2, v0+h*(1-(p2[1]+1)/2)

    rings = {}
    for side, z in (('front', half), ('back', -half)):
        rings[side] = ([first+len(points)+k for k in range(n+1)], None)
        points += [place(p, z) for p in outer]
        rings[side] = (rings[side][0], [first+len(points)+k for k in range(n+1)])
        points += [place(p, z) for p in inner]
    pos = lambda i: points[i-first]

    def tri(a, b, c, nrm, uvs):
        k = [(a, uvs[0]), (b, uvs[1]), (c, uvs[2])]
        pa, pb, pc = pos(a), pos(b), pos(c)
        if np.dot(np.cross(pb-pa, pc-pa), nrm) < 0:
            k[1], k[2] = k[2], k[1]
        for idx, uv in k:
            corners.append((idx, *map(float, nrm), float(uv[0]), float(uv[1])))

    for side, sign in (('front', 1), ('back', -1)):
        o, i = rings[side]
        nrm = normal*sign
        for k in range(n):
            # Tips are shared: skip the degenerate triangle at either end.
            if k > 0:
                tri(o[k], o[k+1], i[k], nrm, [face_uv(outer[k]), face_uv(outer[k+1]), face_uv(inner[k])])
            else:
                tri(o[k], o[k+1], i[k+1], nrm, [face_uv(outer[k]), face_uv(outer[k+1]), face_uv(inner[k+1])])
            if 0 < k < n-1:
                tri(o[k+1], i[k+1], i[k], nrm, [face_uv(outer[k+1]), face_uv(inner[k+1]), face_uv(inner[k])])
    su0, sv0, sw, sh = SIDE_UV
    for ring, arc, centre, outward in ((0, outer, (0, 0), 1), (1, inner, (0, C_IN), -1)):
        f, b = rings['front'][ring], rings['back'][ring]
        for k in range(n):
            mid = (arc[k]+arc[k+1])/2-np.array(centre)
            nrm2 = outward*mid/np.linalg.norm(mid)
            nrm = nrm2[0]*x_axis+nrm2[1]*y_axis
            nrm /= np.linalg.norm(nrm)
            u_a, u_b = su0+sw*k/n, su0+sw*(k+1)/n
            tri(f[k], f[k+1], b[k+1], nrm, [(u_a, sv0), (u_b, sv0), (u_b, sv0+sh)])
            tri(f[k], b[k+1], b[k], nrm, [(u_a, sv0), (u_b, sv0+sh), (u_a, sv0+sh)])
    return [list(map(float, p)) for p in points], corners, [int(bone)]*len(points)


def frame(normal, up=(0, 1.0, 0)):
    n = np.asarray(normal, float)
    n /= np.linalg.norm(n)
    y = np.asarray(up, float)-n*np.dot(up, n)
    y /= np.linalg.norm(y)
    return np.cross(y, n), y, n


def nearest_bone(chunks, point, allowed=None):
    best = None
    for c in chunks:
        P = np.array(c['points'])
        d = np.linalg.norm(P-point, axis=1)
        for i in np.argsort(d)[:8]:
            if allowed is None or c['bones'][i] in allowed:
                if best is None or d[i] < best[0]:
                    best = (d[i], c['bones'][i])
                break
    return best[1]


def surface_point(chunks, facing, height):
    """Point on the x=0 line of the facing (-1 front, +1 back) side of the main armour chunk."""
    main = max(chunks, key=lambda c: len(c['points']))
    P = np.array(main['points'])
    cs = main['corners']
    tris = []
    for f in range(0, len(cs), 3):
        k = cs[f:f+3]
        n = np.mean([x[1:4] for x in k], 0)
        if facing*n[2] > .45:
            tris.append((P[[x[0] for x in k]], n/np.linalg.norm(n)))
    ys = np.concatenate([t[0][:, 1] for t in tris])
    y = ys.min()+height*(ys.max()-ys.min())
    best = None
    for pts, n in tris:
        a, b, c = pts[:, :2]
        m = np.array([b-a, c-a]).T
        if abs(np.linalg.det(m)) < 1e-9:
            continue
        l1, l2 = np.linalg.solve(m, np.array([0, y])-a)
        inside = min(l1, l2, 1-l1-l2)
        if best is None or inside > best[0]:
            best = (inside, pts[0]+l1*(pts[1]-pts[0])+l2*(pts[2]-pts[0]), n)
    return best[1], best[2]


def shoulder_tops(chunks, bone_names):
    # Outer top of each shoulder plate: upper-arm vertices well away from the neck.
    arm = {i for i, name in enumerate(bone_names) if 'UpperArm' in name}
    reach = max(abs(p[0]) for c in chunks for p, b in zip(c['points'], c['bones']) if b in arm)
    tops = {}
    for c in chunks:
        P = np.array(c['points'])
        for i, (p, b) in enumerate(zip(P, c['bones'])):
            if b in arm and abs(p[0]) > .55*reach:
                side = 1 if p[0] > 0 else -1
                if side not in tops or p[1] > tops[side][0][1]:
                    tops[side] = (p, b)
    return tops


def ornaments(chunks, bone_names, sizes, include=('shoulder', 'chest', 'back')):
    """Build one ornament chunk (bind-pose points, corners, bones) for these armour chunks."""
    points, corners, bones = [], [], []

    def add(mesh):
        p, c, b = mesh
        points.extend(p)
        corners.extend(c)
        bones.extend(b)
    for side, (top, bone) in (shoulder_tops(chunks, bone_names).items() if 'shoulder' in include else []):
        # A crescent blade standing on the plate, its face turned outward and a little forward,
        # leaning away from the head.
        x, y, n = frame((side, 0, -.35), up=(side*.4, 1.0, 0))
        r = sizes['shoulder']
        add(crescent_mesh(top+y*r*.55, x, y, n, r, sizes['thickness'], bone, len(points)))
    point, normal = surface_point(chunks, -1, sizes['chest_height'])
    x, y, n = frame(normal)
    if 'chest' in include:
        add(crescent_mesh(point+n*.012, x, y, n, sizes['chest'], sizes['thickness']*.8,
                      nearest_bone(chunks, point), len(points)))
    point, normal = surface_point(chunks, 1, sizes['back_height'])
    x, y, n = frame((0, 0, 1.0))
    if 'back' in include:
        add(crescent_mesh(point+n*.07+y*.03, x, y, n, sizes['back'], sizes['thickness'],
                      nearest_bone(chunks, point), len(points)))
    return dict(points=points, corners=corners, bones=bones)


def ornament_texture(size=128):
    """Left half: crescent face (bright rim, vivid blue body, luminous inner edge). Right: side wall."""
    from PIL import Image
    h = w = size
    img = np.zeros((h, w, 3))
    yy, xx = np.mgrid[:h, :w]
    u, v = (xx+.5)/w, (yy+.5)/h
    u0, v0, fw, fh = FACE_UV
    x = (u-u0)/fw*2-1
    y = 1-(v-v0)/fh*2
    outer = np.hypot(x, y)-1
    inner = np.hypot(x, y-C_IN)-R_IN
    depth = np.clip(-np.maximum(outer, -inner), 0, 1)  # distance inside the crescent
    rim = np.exp(-depth/.05)
    inner_glow = np.exp(-np.clip(-inner, 0, 1)/.09)
    body = np.stack([np.interp(depth, [0, .15, .4], c) for c in ([90, 40, 22], [150, 95, 70], [255, 220, 200])], -1)
    face = body+(np.array([235, 247, 255])-body)*np.clip(rim*.9+inner_glow*.55, 0, 1)[..., None]
    side_t = np.abs((v-.5)/.48)
    side = np.stack([np.interp(side_t, [0, .5, 1], c) for c in ([220, 120, 70], [240, 190, 160], [255, 255, 255])], -1)
    img = np.where((u < .5)[..., None], face, side)
    return Image.fromarray(np.clip(img+.5, 0, 255).astype(np.uint8))
