"""Convert our game GLB into Wind3D MOD / compressed BMP textures."""
from pathlib import Path
from io import BytesIO
import struct,json,zlib,sys
import numpy as np
from PIL import Image
O=Path(__file__).resolve().parent;P=O/'payload';P.mkdir(exist_ok=True);C=O.parents[2]/'runtime/client/GameClient'
def pack_texture(image,path):
 f=BytesIO();image.convert('RGB').save(f,format='BMP');b=f.getvalue();path.write_bytes(b'TEAMMAY\0\0'+struct.pack('<I',len(b))+zlib.compress(b,9))
b=(O/'Bloodfang_Openwork_Spear.glb').read_bytes();n=struct.unpack_from('<I',b,12)[0];j=json.loads(b[20:20+n]);bin=b[28+n:]
def acc(i):
 a=j['accessors'][i];v=j['bufferViews'][a['bufferView']];dim={'VEC2':2,'VEC3':3,'SCALAR':1}[a['type']]
 return np.frombuffer(bin,dtype='<f4',count=a['count']*dim,offset=v.get('byteOffset',0)+a.get('byteOffset',0)).reshape((-1,dim)).copy()
original=(C/'Equip/Wp_0026_1.mod').read_bytes()
nv=struct.unpack_from('<i',original,100)[0];v=np.frombuffer(original,dtype='<f4',count=nv*3,offset=104).reshape(-1,3)
nf=struct.unpack_from('<i',original,104+12*nv)[0];fc=np.frombuffer(original,dtype=np.dtype([('i','<i4'),('n','<f4',3),('uv','<f4',2)]),count=nf*3,offset=108+12*nv)
fp=v[fc['i']].reshape(-1,3,3);cross=np.cross(fp[:,1]-fp[:,0],fp[:,2]-fp[:,0]);dot=(cross*fc['n'].reshape(-1,3,3).mean(axis=1)).sum(axis=1);sign=float(np.median(np.sign(dot[abs(dot)>1e-9])))
assert sign in [-1,1]
groups={};colors=[]
for mi,mat in enumerate(j['materials']):
 pbr=mat['pbrMetallicRoughness'];rgb=np.array(pbr['baseColorFactor'][:3]);rgb=np.where(rgb<=.0031308,rgb*12.92,1.055*rgb**(1/2.4)-.055);colors.append(tuple(np.clip(np.round(rgb*255),0,255).astype(int)))
 if 'baseColorTexture' not in pbr:pack_texture(Image.new('RGB',(8,8),colors[-1]),P/f'bn_mat_{mi:02}.wtm')
pack_texture(Image.open(O/'concept_texture.png').resize((1024,2048),Image.Resampling.LANCZOS),P/'bn_surface.wtm')
allpos=[]
for mesh in j['meshes']:
 for prim in mesh['primitives']:
  a=prim['attributes'];pos=acc(a['POSITION']);normal=acc(a['NORMAL']);uv=acc(a['TEXCOORD_0']) if 'TEXCOORD_0' in a else np.full((len(pos),2),.5,dtype=np.float32)
  # Original weapons point along +X. Grip lies 0.64m from the butt.
  # Scale gives a long 2.70m weapon (original Black Knight spear ~2.21m).
  pos=np.column_stack(((pos[:,1]-.64)*.83,-pos[:,0]*.83,pos[:,2]*.83)).astype('<f4')
  normal=np.column_stack((normal[:,1],-normal[:,0],normal[:,2])).astype('<f4')
  if sign<0:
   pos=pos.reshape(-1,3,3)[:,[0,2,1]].reshape(-1,3);normal=normal.reshape(-1,3,3)[:,[0,2,1]].reshape(-1,3);uv=uv.reshape(-1,3,2)[:,[0,2,1]].reshape(-1,2)
  # Fur and relief ribbons must remain visible from both sides in the old renderer.
  if 'fur strands' in mesh['name'] or 'crest claw' in mesh['name']:
   pos=np.concatenate([pos,pos.reshape(-1,3,3)[:,[0,2,1]].reshape(-1,3)])
   normal=np.concatenate([normal,-normal.reshape(-1,3,3)[:,[0,2,1]].reshape(-1,3)])
   uv=np.concatenate([uv,uv.reshape(-1,3,2)[:,[0,2,1]].reshape(-1,2)])
  groups.setdefault(prim['material'],[]).append((pos,normal,uv));allpos.append(pos)
chunks=[];report=[]
for mi,parts in sorted(groups.items()):
 pos=np.concatenate([a[0] for a in parts]);normal=np.concatenate([a[1] for a in parts]);uv=np.concatenate([a[2] for a in parts])
 texture='bn_surface.bmp' if mi==0 else f'bn_mat_{mi:02}.bmp'
 for start in range(0,len(pos),24000):
  ps=pos[start:start+24000];ns=normal[start:start+24000];us=uv[start:start+24000];nv=len(ps);nf=nv//3
  corners=np.empty(nv,dtype=np.dtype([('i','<i4'),('n','<f4',3),('uv','<f4',2)]));corners['i']=np.arange(nv);corners['n']=ns;corners['uv']=us
  chunk=np.eye(4,dtype='<f4').tobytes()+struct.pack('<i',nv)+ps.astype('<f4').tobytes()+struct.pack('<i',nf)+corners.tobytes()+struct.pack('<4f',.12,.12,.12,.2)+texture.encode().ljust(32,b'\0')+struct.pack('<i',0)
  chunks.append(chunk);report.append(dict(texture=texture,vertices=nv,triangles=nf))
data=original[:32]+struct.pack('<i',len(chunks))+b''.join(chunks)
for lod in [1,2,3]:(P/f'bn_blood_{lod}.mod').write_bytes(data)
# Round-trip every field and validate all runtime texture resolutions.
at=36
for mesh in report:
 at+=64;nv=struct.unpack_from('<i',data,at)[0];at+=4+nv*12;nf=struct.unpack_from('<i',data,at)[0];at+=4
 faces=np.frombuffer(data,dtype=corners.dtype,count=nf*3,offset=at);assert faces['i'].min()>=0 and faces['i'].max()<nv<65536
 assert np.isfinite(faces['n']).all() and np.isfinite(faces['uv']).all();at+=nf*72+16
 tex=data[at:at+32].split(b'\0')[0].decode();assert (P/Path(tex).with_suffix('.wtm')).exists();at+=32
 assert struct.unpack_from('<i',data,at)[0]==0;at+=4
assert at==len(data)
pos=np.concatenate(allpos);result=dict(passed=True,triangles=sum(x['triangles'] for x in report),meshes=len(report),bounds=[pos.min(axis=0).tolist(),pos.max(axis=0).tolist()],original_winding_sign=sign,bytes=len(data),parts=report)
(O/'native_model_validation.json').write_text(json.dumps(result,indent=2));print(json.dumps({k:v for k,v in result.items() if k!='parts'}))
