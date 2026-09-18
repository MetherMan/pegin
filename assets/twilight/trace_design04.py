"""Extract vector construction loops from the approved ImageGen projection.

This reads the image to build geometry; it never paints or edits its pixels.
The cylindrical grip is modeled separately from the silhouette extrusions.
"""
from pathlib import Path
import json
import numpy as np
from PIL import Image
O=Path(__file__).resolve().parent

def area(points):
    p=np.asarray(points);return float((p[:,0]*np.roll(p[:,1],-1)-p[:,1]*np.roll(p[:,0],-1)).sum()/2)

def simplify(points,epsilon=1.2):
    def rdp(p):
        if len(p)<3:return p
        d=p[-1]-p[0];length=np.linalg.norm(d)
        distances=np.abs(d[0]*(p[:,1]-p[0,1])-d[1]*(p[:,0]-p[0,0]))/length if length else np.linalg.norm(p-p[0],axis=1)
        i=int(distances.argmax())
        if distances[i]<=epsilon:return np.stack([p[0],p[-1]])
        return np.concatenate([rdp(p[:i+1])[:-1],rdp(p[i:])])
    p=np.asarray(points,dtype=float);cut=np.argmax(np.linalg.norm(p-p[0],axis=1))
    a=rdp(p[:cut+1]);b=rdp(np.concatenate([p[cut:],p[:1]]))
    return np.concatenate([a[:-1],b[:-1]]).tolist()

def loops(mask):
    padded=np.pad(mask,1);edges={}
    # Clockwise exterior in image coordinates; holes have opposite winding.
    for dx,dy,ox1,oy1,ox2,oy2 in [(0,-1,0,0,1,0),(1,0,1,0,1,1),(0,1,1,1,0,1),(-1,0,0,1,0,0)]:
        neighbors=padded[1+dy:1+dy+mask.shape[0],1+dx:1+dx+mask.shape[1]]
        ys,xs=np.where(mask & ~neighbors)
        for x,y in zip(xs.tolist(),ys.tolist()):edges.setdefault((x+ox1,y+oy1),[]).append((x+ox2,y+oy2))
    result=[]
    while edges:
        start=next(iter(edges));current=start;points=[]
        while True:
            points.append(current);choices=edges[current];nxt=choices.pop()
            if not choices:del edges[current]
            current=nxt
            if current==start:break
        if len(points)>8 and abs(area(points))>50:result.append(points)
    return result

image=np.asarray(Image.open(O/'concept_texture.png').convert('RGB'));mask=image.max(axis=2)>22
parts=[]
for name,lower,upper in [('main-blade-and-auxiliary-cutters',0,1039),('crescent-pommel',1264,len(mask))]:
    region=mask.copy();region[:lower]=False;region[upper:]=False
    contours=loops(region);outer=max(contours,key=area);holes=[q for q in contours if area(q)<-500]
    reduced=simplify(outer);holes=[simplify(q,.9) for q in holes]
    parts.append(dict(name=name,outline=reduced,holes=holes,source_area=area(outer),outline_points=len(reduced)))
assert len(parts[0]['holes'])>=2,'Expected upper openwork and the lower round aperture'
data=dict(revision='approved-04-irregular-auxiliary-blades',image_size=[image.shape[1],image.shape[0]],center_x=487,grip_top=1034,grip_bottom=1270,grip_radius_pixels=22,parts=parts)
(O/'design04_geometry.json').write_text(json.dumps(data,indent=2)+'\n')
print(json.dumps(dict(parts=[dict(name=p['name'],vertices=len(p['outline']),holes=len(p['holes'])) for p in parts])))
