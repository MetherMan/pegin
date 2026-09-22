"""Audit shipped surfaces, including winding and watertight seams after atlas export."""
from mesh_tools import *
from collections import defaultdict
import hashlib

def audit(path):
    parts=read_mod(path);triangles=[]
    for part in parts:
        t=part['points'][part['corners']['index']].reshape(-1,3,3).astype(float)
        n=part['corners']['normal'].reshape(-1,3,3).mean(1)
        dot=(np.cross(t[:,1]-t[:,0],t[:,2]-t[:,0])*n).sum(1)
        if np.median(dot)<0:t=t[:,[0,2,1]]
        triangles.append(t)
    t=np.concatenate(triangles)
    # Native float32 serialization and cylinder seam trigonometry may differ
    # below a microunit. Normals and UV seams do not separate the solid surface.
    _,ids=np.unique(np.round(t.reshape(-1,3),6),axis=0,return_inverse=True)
    ids=ids.reshape(-1,3);edges=defaultdict(list)
    for face,verts in enumerate(ids):
        for a,b in zip(verts,np.roll(verts,-1)):
            edges[tuple(sorted((int(a),int(b))))].append((face,1 if a<b else -1))
    open_edges=sum(len(v)==1 for v in edges.values())
    nonmanifold=sum(len(v)>2 for v in edges.values())
    reversed_edges=sum(len(v)==2 and v[0][1]==v[1][1] for v in edges.values())
    neighbors=[[] for _ in t]
    for values in edges.values():
        if len(values)==2:
            a,b=values[0][0],values[1][0];neighbors[a].append(b);neighbors[b].append(a)
    unseen=set(range(len(t)));volumes=[]
    while unseen:
        stack=[unseen.pop()];component=[]
        while stack:
            f=stack.pop();component.append(f)
            for n in neighbors[f]:
                if n in unseen:unseen.remove(n);stack.append(n)
        q=t[component].copy();q-=q.reshape(-1,3).mean(0)
        volumes.append(float(np.einsum('ij,ij->i',q[:,0],np.cross(q[:,1],q[:,2])).sum()/6))
    result=dict(model=path.name,triangles=len(t),open_edges=open_edges,nonmanifold_edges=nonmanifold,
                reversed_seams=reversed_edges,closed_components=len(volumes),component_volumes=volumes,
                sha256=hashlib.sha256(path.read_bytes()).hexdigest())
    result['passed']=open_edges==nonmanifold==reversed_edges==0 and all(v>1e-9 for v in volumes)
    return result

if __name__=='__main__':
    results=[audit(ROOT/f'client-overlay/Equip/{prefix}_1.mod') for prefix in ['mt_longbow','mt_staff','mt_twilight']]
    output=dict(passed=all(r['passed'] for r in results),models=results)
    (Path(__file__).parent/'closed-surface-validation.json').write_text(json.dumps(output,indent=2)+'\n')
    for r in results:print(json.dumps(r))
    assert output['passed'], 'Open, inward-facing, or inconsistent native surface'
