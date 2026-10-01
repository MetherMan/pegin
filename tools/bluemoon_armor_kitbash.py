"""Take artist-made shoulder armour from higher-tier sets and fit it onto the Blue Moon Elf set.

All warrior armour of one sex shares the native character skeleton, so the donor triangles whose
three vertices ride the upper-arm/clavicle bones (pauldrons and upper sleeves) keep their skinning
as they are. They are pushed 4% outward from their own joint so they sit over the Elf plates
instead of fighting with them.
"""
import numpy as np


def shoulder_parts(donor_chunks, bone_list, scale=1.04):
    names = [b['name'] for b in bone_list]
    rest = [np.array(b['rest'][12:15], float) for b in bone_list]
    keep = {i for i, n in enumerate(names) if 'UpperArm' in n or 'Clavicle' in n}
    out = []
    for c in donor_chunks:
        cs = c['corners']
        faces = [f for f in range(0, len(cs), 3) if all(c['bones'][cs[f+k][0]] in keep for k in range(3))]
        if not faces:
            continue
        used = sorted({cs[f+k][0] for f in faces for k in range(3)})
        index = {old: new for new, old in enumerate(used)}
        points = []
        for old in used:
            centre = rest[c['bones'][old]]
            points.append(list(map(float, centre+(np.array(c['points'][old])-centre)*scale)))
        corners = [[index[k[0]], *k[1:]] for f in faces for k in cs[f:f+3]]
        out.append(dict(points=points, corners=corners, bones=[c['bones'][o] for o in used], texture=c['texture']))
    return out
