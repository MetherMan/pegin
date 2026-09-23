"""Closed sculpted surfaces, native MOD export, and independent GLB preview.

Reference images are read as material sources; segmentation produces geometry,
not repainted image deliverables. Every native surface belongs to mesh zero.
"""
from pathlib import Path
from io import BytesIO
import sys, struct, json, zlib, math
ROOT=Path(__file__).resolve().parents[2]
sys.path[:0]=[str(ROOT/'.cache/weapon-build-deps'),str(ROOT/'runtime/pylibs'),str(ROOT/'assets/twilight')]
import numpy as np
import cv2
import shapely
from shapely.geometry import Polygon
from PIL import Image
from native_atlas import single_mesh_atlas
from verify_native_animation import read_mod

def normals(p):
    t=np.asarray(p).reshape(-1,3,3)
    n=np.cross(t[:,1]-t[:,0],t[:,2]-t[:,0]);n/=np.maximum(np.linalg.norm(n,axis=1)[:,None],1e-12)
    return np.repeat(n,3,axis=0)

def refine(tris,step=22):
    # Split the same geometric edge in every adjacent face: no T junctions.
    def edge(a,b):return tuple(sorted((tuple(a),tuple(b))))
    tris=np.asarray(tris,dtype=float)
    for _ in range(12):
        cuts={edge(a,b) for t in tris for a,b in zip(t,np.roll(t,-1,axis=0)) if np.linalg.norm(a-b)>step}
        if not cuts:return tris
        out=[]
        for t in tris:
            flags=[edge(t[i],t[(i+1)%3]) in cuts for i in range(3)]
            n=sum(flags)
            if n==0:out.append(t);continue
            if n==3:
                a,b,c=t;ab=(a+b)/2;bc=(b+c)/2;ca=(c+a)/2
                out.extend([[a,ab,ca],[ab,b,bc],[ca,bc,c],[ab,bc,ca]])
            elif n==1:
                k=flags.index(True);a,b,c=np.roll(t,-k,axis=0);m=(a+b)/2
                out.extend([[a,m,c],[m,b,c]])
            else:
                k=next(i for i in range(3) if flags[i] and flags[(i+1)%3])
                a,b,c=np.roll(t,-k,axis=0);ab=(a+b)/2;bc=(b+c)/2
                out.extend([[ab,b,bc],[a,ab,c],[ab,bc,c]])
        tris=np.asarray(out)
    raise RuntimeError('Refinement did not converge')

def sculpt(poly,xy_map,height,image_size,step=22):
    poly=shapely.orient_polygons(poly)
    triangles=shapely.constrained_delaunay_triangles(poly)
    tri=refine([np.asarray(g.exterior.coords)[:3] for g in triangles.geoms],step)
    # GEOS does not inherit the polygon's orientation for its triangles.
    # Orient each face in the actual 3D XY plane: the +Z skin faces outward.
    mapped_tri=xy_map(tri.reshape(-1,2)).reshape(-1,3,2)
    ab=mapped_tri[:,1]-mapped_tri[:,0];ac=mapped_tri[:,2]-mapped_tri[:,0]
    reverse=ab[:,0]*ac[:,1]-ab[:,1]*ac[:,0]<0
    tri[reverse]=tri[reverse][:,[0,2,1]]
    xy=tri.reshape(-1,2);distance=shapely.distance(shapely.points(xy),poly.boundary)
    half=np.asarray(height(xy,distance));mapped=xy_map(xy)
    front=np.column_stack((mapped,half));back=front.reshape(-1,3,3)[:,[0,2,1]].reshape(-1,3).copy();back[:,2]*=-1
    uv=xy/np.asarray(image_size);uvback=uv.reshape(-1,3,2)[:,[0,2,1]].reshape(-1,2)
    face=np.concatenate((front,back));fuv=np.concatenate((uv,uvback))
    side=[];suv=[]
    # Use the *refined* face boundary, so sidewalls share every vertex and edge
    # with the skins. Using the original contour leaves open T junctions.
    boundary={}
    for t in tri:
        for a,b in zip(t,np.roll(t,-1,axis=0)):
            key=tuple(sorted((tuple(a),tuple(b))))
            if key in boundary:del boundary[key]
            else:boundary[key]=(a,b)
    along=0
    for a,b in boundary.values():
        distance=float(np.linalg.norm(b-a));h=height(np.array([a,b]),np.zeros(2));q=xy_map(np.array([a,b]))
        corners=np.array([[*q[0],-h[0]],[*q[1],-h[1]],[*q[1],h[1]],[*q[0],h[0]]])
        start=(along/220)%1
        if start+distance/220>1:start=0
        end=start+distance/220
        tex=np.array([[0,start],[0,end],[1,end],[1,start]])
        for inds in [[0,1,2],[0,2,3]]:side.extend(corners[inds]);suv.extend(tex[inds])
        along+=distance
    side=np.asarray(side)
    suv=np.asarray(suv)
    return (face,normals(face),fuv),(side,normals(side),suv)

def cylinder(a,b,r1,r2,uv_box=(0,0,1,1),sides=24,rings=1):
    a=np.array(a,float);b=np.array(b,float);d=b-a;d/=np.linalg.norm(d)
    e=np.cross(d,[0,0,1]);e/=np.linalg.norm(e);f=np.cross(d,e)
    pts=[];uv=[]
    def v(t,k):return a+(b-a)*t+(e*math.cos(k/sides*2*math.pi)+f*math.sin(k/sides*2*math.pi))*(r1+(r2-r1)*t)
    for j in range(rings):
        for k in range(sides):
            ts=[j/rings,j/rings,(j+1)/rings,(j+1)/rings];ks=[k,k+1,k+1,k]
            q=[v(t,kk) for t,kk in zip(ts,ks)];u=[(kk/sides,t) for t,kk in zip(ts,ks)]
            for inds in [[0,1,2],[0,2,3]]:pts.extend([q[i] for i in inds]);uv.extend([u[i] for i in inds])
    for t,inds in [(0,[0,2,1]),(1,[0,1,2])]:
        for k in range(sides):
            q=[a+(b-a)*t,v(t,k),v(t,k+1)];u=[(.5,t),(k/sides,t),((k+1)/sides,t)]
            pts.extend([q[i] for i in inds]);uv.extend([u[i] for i in inds])
    uv=np.asarray(uv);uv=uv*np.array([uv_box[2]-uv_box[0],uv_box[3]-uv_box[1]])+uv_box[:2]
    pts=np.asarray(pts);return pts,normals(pts),uv

def pack_texture(im,path):
    b=BytesIO();im.convert('RGB').save(b,format='BMP');raw=b.getvalue();path.parent.mkdir(parents=True,exist_ok=True)
    path.write_bytes(b'TEAMMAY\0\0'+struct.pack('<I',len(raw))+zlib.compress(raw,9))

def write_glb(path,parts,images):
    blobs=[];views=[];accessors=[];meshes=[];gimages=[];cursor=0
    def chunk(data):
        nonlocal cursor
        pad=(-cursor)%4;blobs.append(bytes(pad));cursor+=pad
        i=len(views);views.append(dict(buffer=0,byteOffset=cursor,byteLength=len(data)));blobs.append(data);cursor+=len(data);return i
    def acc(a,typ):
        a=np.asarray(a,dtype='<f4');i=len(accessors);accessors.append(dict(bufferView=chunk(a.tobytes()),componentType=5126,count=len(a),type=typ,min=a.min(0).tolist(),max=a.max(0).tolist()));return i
    for name,material,(p,n,u) in parts:
        meshes.append(dict(name=name,primitives=[dict(material=material,attributes=dict(POSITION=acc(p,'VEC3'),NORMAL=acc(n,'VEC3'),TEXCOORD_0=acc(u,'VEC2')))]))
    for im in images:
        b=BytesIO();im.save(b,format='PNG');gimages.append(dict(bufferView=chunk(b.getvalue()),mimeType='image/png'))
    j=dict(asset=dict(version='2.0',generator='Twilight volume and UV builder'),scene=0,scenes=[dict(nodes=list(range(len(meshes))))],nodes=[dict(mesh=i) for i in range(len(meshes))],meshes=meshes,buffers=[dict(byteLength=cursor)],bufferViews=views,accessors=accessors,images=gimages,textures=[dict(source=i,sampler=0) for i in range(len(images))],samplers=[dict(wrapS=10497,wrapT=10497)],materials=[dict(doubleSided=False,pbrMetallicRoughness=dict(baseColorTexture=dict(index=i),metallicFactor=0,roughnessFactor=.9)) for i in range(len(images))])
    jb=json.dumps(j).encode();jb+=b' '*((-len(jb))%4);bb=b''.join(blobs);bb+=bytes((-len(bb))%4)
    path.write_bytes(struct.pack('<3I',0x46546c67,2,28+len(jb)+len(bb))+struct.pack('<I4s',len(jb),b'JSON')+jb+struct.pack('<I4s',len(bb),b'BIN\0')+bb)

def export_native(out,prefix,parts,images,reference,atlas_packer=single_mesh_atlas):
    out.mkdir(parents=True,exist_ok=True);groups={}
    ref=read_mod(reference)[0];tri=ref['points'][ref['corners']['index']].reshape(-1,3,3)
    dots=(np.cross(tri[:,1]-tri[:,0],tri[:,2]-tri[:,0])*ref['corners']['normal'].reshape(-1,3,3).mean(1)).sum(1)
    sign=float(np.median(np.sign(dots[np.abs(dots)>1e-9])))
    assert sign in [-1,1]
    for name,mat,(p,n,uv) in parts:
        # +Y in preview -> +X in the original weapon animation.
        p=np.column_stack((p[:,1],-p[:,0],p[:,2]));n=np.column_stack((n[:,1],-n[:,0],n[:,2]))
        if sign<0:
            p=p.reshape(-1,3,3)[:,[0,2,1]].reshape(-1,3);n=n.reshape(-1,3,3)[:,[0,2,1]].reshape(-1,3);uv=uv.reshape(-1,3,2)[:,[0,2,1]].reshape(-1,2)
        groups.setdefault(mat,[]).append((p,n,uv))
    p,n,u,atlas=atlas_packer(groups,dict(enumerate(images)),pack_texture,out/(prefix+'_atlas.wtm'))
    vertices,index=np.unique(p,axis=0,return_inverse=True);nf=len(p)//3
    assert len(vertices)<65536 and nf<65536
    c=np.empty(len(p),dtype=[('i','<i4'),('n','<f4',3),('u','<f4',2)]);c['i']=index;c['n']=n;c['u']=u
    chunk=np.eye(4,dtype='<f4').tobytes()+struct.pack('<i',len(vertices))+vertices.astype('<f4').tobytes()+struct.pack('<i',nf)+c.tobytes()+struct.pack('<4f',0,0,0,0)+(prefix+'_atlas.bmp').encode().ljust(32,b'\0')+bytes(4)
    data=reference.read_bytes()[:32]+struct.pack('<i',1)+chunk
    for lod in [1,2,3]:(out/f'{prefix}_{lod}.mod').write_bytes(data)
    actual=read_mod(out/f'{prefix}_1.mod');assert len(actual)==1
    return dict(triangles=nf,vertices=len(vertices),meshes=1,bounds=[vertices.min(0).tolist(),vertices.max(0).tolist()],atlas=atlas,reference=reference.name,reference_winding=sign)
