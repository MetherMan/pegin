"""New plate armour pieces that change the Elf set's silhouette (Blue Moon rank-9 redesign).

Every piece is a closed shell (outer face, inner face and edge walls) shaped on the bind-pose body
and skinned to one native bone, built from bone rest positions and measured body sizes:
- pauldrons: three overlapping lames on each shoulder; the top lame's front and back ends sweep up
  and flare out like the horns of a crescent,
- gorget (male): a raised collar with a crescent notch at the front,
- tassets: two overlapping lames at the front and sides of the hips, pointed lower edge, following
  the thighs,
- knee cops: a cap whose outer side extends up and down into a crescent wing,
- cuffs: flared wrist guards rising to a point on the outer forearm.
Front is -Z, up is +Y. The plate texture has an outer design area, a dark underside and an edge band.
"""
import math
import numpy as np

OUTER_UV = (.02, .02, .70, .96)
INNER_UV = (.76, .02, .10, .96)
EDGE_UV = (.88, .02, .10, .96)


def unit(v):
    v = np.asarray(v, float)
    return v/np.linalg.norm(v)


def shell(surface, nu, nv, thickness, bone, first):
    """surface(u, v) -> (point, outward normal). Returns points, native corners, bones."""
    us, vs = np.linspace(0, 1, nu+1), np.linspace(0, 1, nv+1)
    grid = [[surface(u, v) for v in vs] for u in us]
    outer = np.array([[p for p, n in row] for row in grid])
    normal = np.array([[unit(n) for p, n in row] for row in grid])
    inner = outer-normal*thickness
    points = list(outer.reshape(-1, 3))+list(inner.reshape(-1, 3))
    idx_o = lambda i, j: first+i*(nv+1)+j
    idx_i = lambda i, j: first+(nu+1)*(nv+1)+i*(nv+1)+j
    corners = []

    def tri(ks, nrm):
        pa, pb, pc = (points[k[0]-first] for k in ks)
        if np.dot(np.cross(pb-pa, pc-pa), nrm) < 0:
            ks = [ks[0], ks[2], ks[1]]
        for k, (u, v) in ks:
            corners.append((k, *map(float, unit(nrm)), float(u), float(v)))

    def area(region, u, v):
        u0, v0, w, h = region
        return u0+w*u, v0+h*v
    for i in range(nu):
        for j in range(nv):
            n = normal[i:i+2, j:j+2].reshape(-1, 3).mean(0)
            q = [(i, j), (i+1, j), (i+1, j+1), (i, j+1)]
            o = [(idx_o(a, b), area(OUTER_UV, us[a], vs[b])) for a, b in q]
            tri([o[0], o[1], o[2]], n)
            tri([o[0], o[2], o[3]], n)
            r = [(idx_i(a, b), area(INNER_UV, us[a], vs[b])) for a, b in q]
            tri([r[0], r[1], r[2]], -n)
            tri([r[0], r[2], r[3]], -n)
    # Edge walls around the border, facing away from the plate.
    border = [((i, 0), (i+1, 0), -1, 'v') for i in range(nu)]+[((i, nv), (i+1, nv), 1, 'v') for i in range(nu)]
    border += [((0, j), (0, j+1), -1, 'u') for j in range(nv)]+[((nu, j), (nu, j+1), 1, 'u') for j in range(nv)]
    for (a, b), (c, d), sign, along in border:
        if along == 'v':
            t = outer[a, b]-outer[a, min(b+1, nv) if sign < 0 else max(b-1, 0)]
        else:
            t = outer[a, b]-outer[min(a+1, nu) if sign < 0 else max(a-1, 0), b]
        n = unit(t) if np.linalg.norm(t) > 1e-9 else normal[a, b]
        s = (a+b)/(nu+nv)
        ks = [(idx_o(a, b), area(EDGE_UV, 0, s)), (idx_o(c, d), area(EDGE_UV, 0, s+.02)),
              (idx_i(c, d), area(EDGE_UV, 1, s+.02)), (idx_i(a, b), area(EDGE_UV, 1, s))]
        tri([ks[0], ks[1], ks[2]], n)
        tri([ks[0], ks[2], ks[3]], n)
    return [list(map(float, p)) for p in points], corners, [int(bone)]*len(points)


def sphere_surface(centre, e_out, e_fwd, e_up, theta, phi_top, phi_bottom, radius):
    """theta in degrees around e_up from e_out towards e_fwd; phi = elevation (degrees)."""
    def surface(u, v):
        t = theta[0]+u*(theta[1]-theta[0])
        p = phi_top(t)+v*(phi_bottom(t)-phi_top(t))
        tr, pr = math.radians(t), math.radians(p)
        d = math.cos(pr)*(math.cos(tr)*e_out+math.sin(tr)*e_fwd)+math.sin(pr)*e_up
        return centre+radius(t, p, v)*d, d
    return surface


def ring_surface(centre, e_up, e_side, e_front, theta, h_top, h_bottom, rx, rz):
    """Elliptic band around e_up; theta in degrees from e_front towards e_side; heights along e_up."""
    def surface(u, v):
        t = theta[0]+u*(theta[1]-theta[0])
        h = h_top(t)+v*(h_bottom(t)-h_top(t))
        tr = math.radians(t)
        a, b = rx(h, v), rz(h, v)
        p = centre+h*e_up+a*math.sin(tr)*e_side+b*math.cos(tr)*e_front
        n = math.sin(tr)/a*e_side+math.cos(tr)/b*e_front
        return p, unit(n)
    return surface


class Builder:
    def __init__(self):
        self.points, self.corners, self.bones = [], [], []

    def add(self, mesh):
        p, c, b = mesh
        self.points += p
        self.corners += c
        self.bones += b

    def shell(self, surface, nu, nv, thickness, bone):
        self.add(shell(surface, nu, nv, thickness, bone, len(self.points)))


def plates(bone_list, sex, measure):
    """bone_list: [{'name','rest'}]; measure: dict of body sizes for this sex (see SIZES)."""
    names = [b['name'] for b in bone_list]
    rest = {b['name']: np.array(b['rest'][12:15], float) for b in bone_list}
    bone = names.index
    m = measure
    up, front = np.array([0, 1.0, 0]), np.array([0, 0, -1.0])
    out = Builder()
    for side, tag in ((1, 'L'), (-1, 'R')):
        joint = rest[f'Bip01 {tag} UpperArm']
        elbow = rest[f'Bip01 {tag} Forearm']
        hand = rest[f'Bip01 {tag} Hand']
        e_out = np.array([side, 0, 0.0])
        e_fwd = front
        # Pauldron: pole tilted outward so the lames step down the outer arm.
        pole = unit(up*.75+e_out*.66)
        e_o2 = unit(e_out-pole*np.dot(e_out, pole))
        centre = joint+up*m['pauldron_lift']
        r0 = m['pauldron']
        lames = [(r0*1.0, (-6, 62), 76), (r0*.95, (-30, 2), 82), (r0*.9, (-52, -24), 86)]
        for k, (r, (p_lo, p_hi), tmax) in enumerate(lames):
            def phi_top(t, p_hi=p_hi, tmax=tmax, k=k):
                horn = 20*(abs(t)/tmax)**2.4 if k == 0 else 0
                return p_hi+horn
            def radius(t, p, v, r=r, tmax=tmax, k=k):
                flare = .12*(abs(t)/tmax)**3*(1-v) if k == 0 else .04*(1-v)
                return r*(1+flare)
            out.shell(sphere_surface(centre, e_o2, e_fwd, pole, (-tmax, tmax), phi_top, lambda t, p_lo=p_lo: p_lo, radius),
                      14, 4, m['thickness'], bone(f'Bip01 {tag} UpperArm'))
        # Cuff: flared band near the wrist, rising to a point on the outer forearm.
        axis = unit(hand-elbow)
        e_front_arm = unit(front-axis*np.dot(front, axis))
        e_side_arm = e_out-axis*np.dot(e_out, axis)
        e_side_arm = unit(e_side_arm-e_front_arm*np.dot(e_side_arm, e_front_arm))  # away from the body
        length = np.linalg.norm(hand-elbow)
        wrist = hand-axis*length*.05
        r_arm = m['forearm']
        out.shell(ring_surface(wrist, -axis, e_side_arm, e_front_arm, (-180, 180),
                               lambda t: .2*length+.12*length*math.exp(-((abs(t)-90)/35)**2) if t > 0 else .2*length,
                               lambda t: 0.0,
                               lambda h, v: r_arm*(1+.18*v), lambda h, v: r_arm*(1+.18*v)),
                  24, 3, m['thickness']*.8, bone(f'Bip01 {tag} Forearm'))
        # Knee cop with a crescent wing on the outer side.
        knee = rest[f'Bip01 {tag} Calf']
        e_k_out = e_out
        out.shell(sphere_surface(knee+front*m['knee']*.25, front, e_k_out, up, (-70, 115),
                                 lambda t: 38+30*max(0, (t-50)/65)**1.5,
                                 lambda t: -38-34*max(0, (t-50)/65)**1.5,
                                 lambda t, p, v: m['knee']),
                  12, 6, m['thickness'], bone(f'Bip01 {tag} Calf'))
        # Tassets: two lames at the front and two at the side, following this thigh.
        pelvis = rest['Bip01 Pelvis']
        belt = m['belt']
        for t_range in ((12*side, 70*side), (75*side, 125*side)):
            lo, hi = sorted(t_range)
            for k, (top, bottom, grow) in enumerate(((0, -.13, 0), (-.11, -.25, .02))):
                def h_bottom(t, bottom=bottom, lo=lo, hi=hi, k=k):
                    mid = (lo+hi)/2
                    point = .05*(1-abs(t-mid)/((hi-lo)/2)) if k == 1 else 0
                    return bottom*m['tasset']-point
                out.shell(ring_surface(np.array([0, belt, pelvis[2]]), up, np.array([1.0, 0, 0]), front, (lo, hi),
                                       lambda t, top=top: top*m['tasset'], h_bottom,
                                       lambda h, v, grow=grow: m['hip'][0]+grow+.06*v*m['tasset'],
                                       lambda h, v, grow=grow: m['hip'][1]+grow+.05*v*m['tasset']),
                          8, 3, m['thickness'], bone(f'Bip01 {tag} Thigh'))
    if m.get('gorget'):
        neck = rest['Bip01 Neck']
        base = np.array([0, m['gorget'][0], neck[2]-.02])
        height = m['gorget'][1]
        out.shell(ring_surface(base, up, np.array([1.0, 0, 0]), front, (-180, 180),
                               lambda t: height-.05*math.exp(-(t/28)**2), lambda t: 0.0,
                               lambda h, v: m['gorget'][2]+.02*v, lambda h, v: m['gorget'][3]+.02*v),
                  28, 3, m['thickness'], bone('Bip01 Spine1'))
    return dict(points=out.points, corners=out.corners, bones=out.bones)


SIZES = {
    '0': dict(pauldron=.165, pauldron_lift=.015, thickness=.018, forearm=.046, knee=.085,
              belt=1.0, hip=(.215, .155), tasset=1.0, gorget=(1.48, .11, .145, .13)),
    '1': dict(pauldron=.12, pauldron_lift=.01, thickness=.014, forearm=.037, knee=.065,
              belt=.99, hip=(.185, .14), tasset=.75, gorget=None),
}


def plate_texture(size=256):
    """Outer face: painted plate with baked light (lighter top, darker bottom), a specular band,
    brushed streaks, a soft lighter rim with an engraved groove inside it and corner rivets.
    Underside: dark navy. Edge band: steel blue."""
    from PIL import Image
    h = w = size
    yy, xx = np.mgrid[:h, :w]
    u, v = (xx+.5)/w, (yy+.5)/h
    img = np.zeros((h, w, 3))
    u0, v0, fw, fh = OUTER_UV
    pu, pv = np.clip((u-u0)/fw, 0, 1), np.clip((v-v0)/fh, 0, 1)
    edge = np.minimum(np.minimum(pu, 1-pu)*1.3, np.minimum(pv, 1-pv))
    rng = np.random.default_rng(3)
    streak = np.interp(np.arange(w), np.arange(0, w, 3), rng.uniform(-1, 1, len(range(0, w, 3))))[None, :]
    base = np.stack([np.interp(pv, [0, .35, 1], c) for c in ([58, 34, 14], [126, 84, 40], [236, 214, 150])], -1)
    base = base+streak[..., None]*np.array([5, 8, 10])
    spec = np.exp(-((pv-.27)/.07)**2)*.6+np.exp(-((pv-.64)/.05)**2)*.18
    face = base+(np.array([214, 236, 255])-base)*spec[..., None]
    rim = np.clip(1-edge/.08, 0, 1)**1.4
    face = face+(np.array([150, 200, 250])-face)*rim[..., None]*.7
    groove = np.exp(-((edge-.11)/.011)**2)
    face = face*(1-.5*groove[..., None])
    for cu, cv in ((.09, .1), (.91, .1), (.09, .9), (.91, .9)):
        d = np.hypot((pu-cu)*1.3, pv-cv)
        face = np.where((d < .03)[..., None], np.array([226, 242, 255.0])*(1-.4*d[..., None]/.03), face)
        ring = (d >= .03) & (d < .042)
        face = np.where(ring[..., None], face*.45, face)
    img = np.where((u < .74)[..., None], face, img)
    img = np.where(((u >= .74) & (u < .87))[..., None], np.array([12, 24, 66.0]), img)
    img = np.where((u >= .87)[..., None], np.array([120, 172, 236.0]), img)
    return Image.fromarray(np.clip(img+.5, 0, 255).astype(np.uint8))
