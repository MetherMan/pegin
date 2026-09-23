"""Skin-rooted curved antlers and the approved antler cleaver in native geometry.

Adds geometry to the existing two skinned chunks. Original bones/animations,
monster IDs and server stats remain unchanged; textures use dedicated atlases.
"""
from pathlib import Path
import copy,json,math,struct,sys,io,zlib,hashlib
R=Path(__file__).resolve().parents[1]
sys.path[:0]=[str(R/'runtime/pylibs'),str(R/'client-overlay/Tools/SkillColors')]
from native_actor import model
from PIL import Image
from primordial_geometry import BASELINE_SCALE,TARGET_SCALE,scale_chunks,rebuild_motions
D=R/'client-overlay';O=R/'assets/primordial-baphomet'

def cross(a,b):return [a[1]*b[2]-a[2]*b[1],a[2]*b[0]-a[0]*b[2],a[0]*b[1]-a[1]*b[0]]
def norm(a):
    n=math.sqrt(sum(x*x for x in a));return [x/n for x in a] if n>1e-9 else [0,1,0]
def face(c,points,bone,uv):
    n=norm(cross([points[1][k]-points[0][k] for k in range(3)],[points[2][k]-points[0][k] for k in range(3)]))
    for i,p in enumerate(points):
        ident=len(c['points']);c['points'].append(list(p));c['bones'].append(bone if isinstance(bone,int) else bone[i])
        c['corners'].append([ident,*n,*uv[i]])

def tube(c,path,radii,bone,uv,sides=12):
    rings=[]
    distances=[0.0]
    for a,b in zip(path,path[1:]):distances.append(distances[-1]+math.sqrt(sum((x-y)**2 for x,y in zip(a,b))))
    for j,p in enumerate(path):
        a=path[max(0,j-1)];b=path[min(len(path)-1,j+1)];t=norm([b[k]-a[k] for k in range(3)])
        axis=[0,0,1] if abs(t[2])<.9 else [1,0,0];u=norm(cross(t,axis));v=cross(t,u)
        rings.append([[p[k]+radii[j]*(u[k]*math.cos(i*math.tau/sides)+v[k]*math.sin(i*math.tau/sides)) for k in range(3)] for i in range(sides)])
    for j in range(len(rings)-1):
        for i in range(sides):
            k=(i+1)%sides;u0,v0,u1,v1=uv;tex=lambda a,b:(u0+(u1-u0)*a/sides,v0+(v1-v0)*distances[b]/max(distances[-1],1e-12))
            face(c,[rings[j][i],rings[j][k],rings[j+1][k]],bone,[tex(i,j),tex(i+1,j),tex(i+1,j+1)])
            face(c,[rings[j][i],rings[j+1][k],rings[j+1][i]],bone,[tex(i,j),tex(i+1,j+1),tex(i,j+1)])
    for ring,center,rev in [(rings[0],path[0],True),(rings[-1],path[-1],False)]:
        for i in range(sides):
            pts=[center,ring[i],ring[(i+1)%sides]]
            if rev:pts.reverse()
            face(c,pts,bone,[(uv[0],uv[1])]*3)

def split(poly,a,b):
    def signed(p):return (b[0]-a[0])*(p[1]-a[1])-(b[1]-a[1])*(p[0]-a[0])
    inside=[];outside=[]
    for p,q in zip(poly,poly[1:]+poly[:1]):
        x,y=signed(p),signed(q);(inside if x>=0 else outside).append(p)
        if (x>=0)!=(y>=0):
            t=x/(x-y);mid=[p[k]+t*(q[k]-p[k]) for k in range(len(p))]
            inside.append(mid);outside.append(mid)
    return inside,outside

def carve(c):
    cx,cy,r=0,3.64,.235;n=32
    circle=[(cx+r*math.cos(i*math.tau/n),cy+r*math.sin(i*math.tau/n)) for i in range(n)]
    old=copy.deepcopy(c);c['corners']=[];cut=0
    for k in range(0,len(old['corners']),3):
        corners=old['corners'][k:k+3];pts=[old['points'][v[0]] for v in corners]
        if (min(p[0] for p in pts)>r or max(p[0] for p in pts)<-r or min(p[1] for p in pts)>cy+r or max(p[1] for p in pts)<cy-r or any(old['bones'][v[0]] not in (3,6,11) for v in corners)):
            c['corners'].extend(corners);continue
        poly=[list(p)+list(v[4:6]) for p,v in zip(pts,corners)];pieces=[]
        for a,b in zip(circle,circle[1:]+circle[:1]):
            if not poly:break
            poly,out=split(poly,a,b)
            if len(out)>=3:pieces.append(out)
        if len(poly)<3:
            c['corners'].extend(corners);continue
        cut+=1
        for part in pieces:
            for i in range(1,len(part)-1):
                tri=[part[0],part[i],part[i+1]]
                face(c,[v[:3] for v in tri],3,[v[3:5] for v in tri])
    def surface(x,y,front=True):
        hits=[]
        for k in range(0,len(old['corners']),3):
            cc=old['corners'][k:k+3]
            if any(old['bones'][v[0]] not in (3,6,11) for v in cc):continue
            a,b,d=[old['points'][v[0]] for v in cc]
            den=(b[1]-d[1])*(a[0]-d[0])+(d[0]-b[0])*(a[1]-d[1])
            if abs(den)<1e-9:continue
            u=((b[1]-d[1])*(x-d[0])+(d[0]-b[0])*(y-d[1]))/den
            v=((d[1]-a[1])*(x-d[0])+(a[0]-d[0])*(y-d[1]))/den
            if min(u,v,1-u-v)>-1e-5:hits.append(u*a[2]+v*b[2]+(1-u-v)*d[2])
        return (min(hits) if front else max(hits)) if hits else (-.33 if front else .6)
    image=Image.open(O/'body-atlas.png').convert('RGB');w,h=image.size
    # Sample the actual existing atlas for dark interior and wet dark-red seams.
    black=min(((sum(image.getpixel((x,y))),x,y) for y in range(0,h,4) for x in range(0,w,4)))
    dark=((black[1]+.5)/w,(black[2]+.5)/h)
    blood=min(((abs(rr-42)+abs(g-9)+abs(b-12),x,y) for y in range(0,h,4) for x in range(0,w,4) for rr,g,b in [image.getpixel((x,y))]))
    red=((blood[1]+.5)/w,(blood[2]+.5)/h)
    rings=[]
    for radius,depth in [(r*1.13,0),(r,0),(r*.86,.15),(r*.83,.8),(r,1)]:
        ring=[]
        for i in range(n):
            a=i*math.tau/n;x=cx+radius*math.cos(a);y=cy+radius*math.sin(a)
            near=surface(x,y);far=surface(x,y,False);z=near+(far-near)*depth
            if depth==0:z-=.012
            ring.append([x,y,z])
        rings.append(ring)
    for j in range(len(rings)-1):
        for i in range(n):
            k=(i+1)%n;uv=red if j==0 else dark
            for tri in ([rings[j][i],rings[j+1][i],rings[j+1][k]],[rings[j][i],rings[j+1][k],rings[j][k]]):face(c,tri,3,[uv]*3)
    for i,(x,length) in enumerate([(-.15,.55),(-.06,.86),(.05,.67),(.16,.43)]):
        y=cy-math.sqrt(max(0,r*r-x*x));path=[]
        for j in range(6):
            xx=x+.024*math.sin(j*1.7+i);yy=y-length*j/5
            path.append([xx,yy,surface(xx,max(3.08,yy))-.028])
        tube(c,path,[.025*(1-j/6) for j in range(6)],3,(*red,*red),8)
    return dict(chest_faces_cut=cut,cavity_radius=r,cavity_is_through_hole=True,dripping_strands=4)

def encode(raw,chunks):
    at=36;records=[]
    for c in chunks:
        # Preserve each chunk's original 16-byte material block.
        n=struct.unpack_from('<i',raw,at+64)[0];at+=68+n*12
        nf=struct.unpack_from('<i',raw,at)[0];at+=4+nf*72
        material=raw[at:at+16];at+=16+32
        kind=struct.unpack_from('<i',raw,at)[0];at+=4+n
        assert kind==2
        rec=struct.pack('<16f',*c['matrix'])+struct.pack('<i',len(c['points']))
        rec+=b''.join(struct.pack('<3f',*p) for p in c['points'])+struct.pack('<i',len(c['corners'])//3)
        rec+=b''.join(struct.pack('<i5f',*p) for p in c['corners'])+material
        rec+=Path(c['texture']).with_suffix('.bmp').name.encode().ljust(32,b'\0')+struct.pack('<i',2)+bytes(c['bones']);records.append(rec)
    return raw[:36]+b''.join(records)

def compact_points(chunk,keep=0):
    # Normals and UVs live on face corners. Share identical skinned positions
    # without smoothing hard edges or merging texture seams.
    points=chunk['points'][:keep];bones=chunk['bones'][:keep]
    ids={(tuple(p),b):i for i,(p,b) in enumerate(zip(points,bones))}
    remap={i:i for i in range(keep)}
    for i in range(keep,len(chunk['points'])):
        p,b=chunk['points'][i],chunk['bones'][i];key=(tuple(p),b)
        if key not in ids:
            ids[key]=len(points);points.append(p);bones.append(b)
        remap[i]=ids[key]
    for corner in chunk['corners']:corner[0]=remap[corner[0]]
    chunk['points']=points;chunk['bones']=bones

def main(target_scale=TARGET_SCALE):
    baseline=O/'sculpt-baseline.mod'
    # This immutable geometry is authored at 1.4. Never capture an already
    # enlarged output as the next baseline, which would accumulate scaling.
    assert baseline.exists(),'The original 1.4 sculpt baseline is required'
    raw=baseline.read_bytes();chunks=model(raw);body,blade=chunks
    original=model((R/'runtime/client/GameClient/Monster/p-warrior.mod').read_bytes())
    assert len(body['points'])==len(original[0]['points'])==809
    assert all(abs(q[k]-p[k]*BASELINE_SCALE)<1e-5 for p,q in zip(original[0]['points'],body['points']) for k in range(3))
    from antler_revision import sculpt_body,build_cleaver
    report=sculpt_body(body,tube)
    report.update(build_cleaver(blade,face,tube))
    compact_points(body,keep=809);compact_points(blade)
    factor=target_scale/BASELINE_SCALE
    scale_chunks(chunks,factor)
    result=encode(raw,chunks);parsed=model(result)
    assert len(parsed)==2 and all(max(c['bones'])<29 for c in parsed)
    assert all(abs(q[k]-p[k]*target_scale)<1e-5 for p,q in zip(original[0]['points'],parsed[0]['points'][:809]) for k in range(3))
    (D/'Monster/mt_prime_baphomet.mod').write_bytes(result)
    data=json.loads((O/'model.json').read_text(encoding='utf-8'));data['primordial']=parsed;data['scale']=target_scale
    motion_report=rebuild_motions(data,R/'runtime/client/GameClient/Monster/Animation',D/'Monster/Animation',target_scale)
    packed=[]
    from horn_strength import apply_strength,load_strength
    horn_strength=load_strength()
    body_source=O/'body-matte-v4-imagegen.png'
    if not body_source.exists():body_source=O/'body-cyclops-imagegen.png'
    horn_source=O/'horn-satin-keratin-v5-imagegen.png'
    if not horn_source.exists():horn_source=O/'horn-material-matte-v4-imagegen.png'
    if not horn_source.exists():horn_source=O/'horn-material-imagegen.png'
    for source,preview,native,size in [(body_source,'body-cyclops-atlas.png','mt_prime_body.wtm',(1024,1024)),(O.parent/'visual-refresh-20260923/baphomet-antler-cleaver-concept.png','cleaver-atlas.png','mt_prime_cleaver.wtm',(1536,1024))]:
        # Native atlas packing only: original image occupies the left region;
        # the separate ImageGen horn material occupies the remaining region.
        base=Image.open(source).convert('RGB').resize(size,Image.Resampling.LANCZOS)
        material=Image.open(horn_source).convert('RGB').resize((2048-size[0],1024),Image.Resampling.LANCZOS)
        material=apply_strength(material,horn_strength)
        im=Image.new('RGB',(2048,1024));im.paste(base,(0,0));im.paste(material,(size[0],0))
        im.save(O/preview);buf=io.BytesIO();im.save(buf,format='BMP');bmp=buf.getvalue()
        native_bytes=b'TEAMMAY\0\0'+struct.pack('<I',len(bmp))+zlib.compress(bmp,9)
        (D/'Texture/Monster'/native).write_bytes(native_bytes)
        assert Image.open(io.BytesIO(zlib.decompress(native_bytes[13:]))).tobytes()==im.tobytes()
        packed.append(dict(texture=native,preview=preview,sha256=hashlib.sha256(native_bytes).hexdigest()))
    data['textures']['primordial']=['body-cyclops-atlas.png','cleaver-atlas.png']
    (O/'model.json').write_text(json.dumps(data,separators=(',',':')),encoding='utf-8')
    assert all(len(c['points'])<65535 for c in parsed)
    report.update(passed=True,revision='black-keratin-v5',original_bones=29,original_animation_rotations_unchanged=True,
                  scale=target_scale,sculpt_baseline_scale=BASELINE_SCALE,sculpt_scale_factor=factor,
                  baseline_sha256=hashlib.sha256(raw).hexdigest(),animations=motion_report,
                  body_source=body_source.name,horn_source=horn_source.name,horn_strength=horn_strength,
                  vertices=sum(len(c['points']) for c in parsed),triangles=sum(len(c['corners'])//3 for c in parsed),texture_readback=packed,in_game_visual_test=False)
    (O/'sculpt-validation.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps(report))

if __name__=='__main__':main()
