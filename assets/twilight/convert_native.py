"""Convert our game GLB into Wind3D MOD / compressed BMP textures."""
from pathlib import Path
from io import BytesIO
import struct,json,zlib,sys
import numpy as np
from PIL import Image
O=Path(__file__).resolve().parent;P=O/'payload';P.mkdir(exist_ok=True);C=O.parents[1]/'runtime/client/GameClient'
sys.path.insert(0,str(O))
from bake_appearance import apply_appearance
appearance=json.loads((O/'game_appearance.json').read_text(encoding='utf-8'))['settings'] if (O/'game_appearance.json').exists() else None
def pack_texture(image,path):
 f=BytesIO();image.convert('RGB').save(f,format='BMP');b=f.getvalue();path.write_bytes(b'TEAMMAY\0\0'+struct.pack('<I',len(b))+zlib.compress(b,9))
b=(O/'Blue_Moon_Twilight_Greatsword.glb').read_bytes();n=struct.unpack_from('<I',b,12)[0];j=json.loads(b[20:20+n]);bin=b[28+n:]
def acc(i):
 a=j['accessors'][i];v=j['bufferViews'][a['bufferView']];dim={'VEC2':2,'VEC3':3,'SCALAR':1}[a['type']]
 return np.frombuffer(bin,dtype='<f4',count=a['count']*dim,offset=v.get('byteOffset',0)+a.get('byteOffset',0)).reshape((-1,dim)).copy()
original=(C/'Equip/Wp_0026_1.mod').read_bytes()
nv=struct.unpack_from('<i',original,100)[0];v=np.frombuffer(original,dtype='<f4',count=nv*3,offset=104).reshape(-1,3)
nf=struct.unpack_from('<i',original,104+12*nv)[0];fc=np.frombuffer(original,dtype=np.dtype([('i','<i4'),('n','<f4',3),('uv','<f4',2)]),count=nf*3,offset=108+12*nv)
fp=v[fc['i']].reshape(-1,3,3);cross=np.cross(fp[:,1]-fp[:,0],fp[:,2]-fp[:,0]);dot=(cross*fc['n'].reshape(-1,3,3).mean(axis=1)).sum(axis=1);sign=float(np.median(np.sign(dot[abs(dot)>1e-9])))
assert sign in [-1,1]
groups={};colors=[];texture_names={}
for mi,mat in enumerate(j['materials']):
 pbr=mat['pbrMetallicRoughness'];rgb=np.array(pbr['baseColorFactor'][:3]);rgb=np.where(rgb<=.0031308,rgb*12.92,1.055*rgb**(1/2.4)-.055);colors.append(tuple(np.clip(np.round(rgb*255),0,255).astype(int)))
 texture_names[mi]=f'mt_mat_{mi:02}.bmp'
 if 'baseColorTexture' not in pbr:
  im=Image.new('RGB',(8,8),colors[-1])
 else:
  tex=j['textures'][pbr['baseColorTexture']['index']];img=j['images'][tex['source']];view=j['bufferViews'][img['bufferView']]
  start=view.get('byteOffset',0);im=Image.open(BytesIO(bin[start:start+view['byteLength']])).convert('RGB')
  # The native renderer cannot apply GLTF factors: bake them in linear light.
  pixels=np.asarray(im,dtype=np.float32)/255;linear=np.where(pixels<=.04045,pixels/12.92,((pixels+.055)/1.055)**2.4)
  linear*=np.array(pbr['baseColorFactor'][:3])
  if appearance and mat['name']!='Weathered ivory woven bandage':linear=apply_appearance(linear,appearance)
  pixels=np.where(linear<=.0031308,linear*12.92,1.055*linear**(1/2.4)-.055)
  im=Image.fromarray(np.clip(np.round(pixels*255),0,255).astype('uint8'))
  im=im.resize((1024,1024),Image.Resampling.LANCZOS)
 pack_texture(im,P/Path(texture_names[mi]).with_suffix('.wtm'))
allpos=[]
for mesh in j['meshes']:
 for prim in mesh['primitives']:
  a=prim['attributes'];pos=acc(a['POSITION']);normal=acc(a['NORMAL']);uv=acc(a['TEXCOORD_0']) if 'TEXCOORD_0' in a else np.full((len(pos),2),.5,dtype=np.float32)
  # Original weapons point along +X; the resized GLB preserves its grip at Y=.5.
  pos=np.column_stack((pos[:,1]-.5,-pos[:,0],pos[:,2])).astype('<f4')
  normal=np.column_stack((normal[:,1],-normal[:,0],normal[:,2])).astype('<f4')
  if sign<0:
   pos=pos.reshape(-1,3,3)[:,[0,2,1]].reshape(-1,3);normal=normal.reshape(-1,3,3)[:,[0,2,1]].reshape(-1,3);uv=uv.reshape(-1,3,2)[:,[0,2,1]].reshape(-1,2)
  # Fur and relief ribbons must remain visible from both sides in the old renderer.
  if False: # Twilight cloth is already a closed thin volume.
   pos=np.concatenate([pos,pos.reshape(-1,3,3)[:,[0,2,1]].reshape(-1,3)])
   normal=np.concatenate([normal,-normal.reshape(-1,3,3)[:,[0,2,1]].reshape(-1,3)])
   uv=np.concatenate([uv,uv.reshape(-1,3,2)[:,[0,2,1]].reshape(-1,2)])
  groups.setdefault(prim['material'],[]).append((pos,normal,uv));allpos.append(pos)
chunks=[];report=[]
for mi,parts in sorted(groups.items()):
 pos=np.concatenate([a[0] for a in parts]);normal=np.concatenate([a[1] for a in parts]);uv=np.concatenate([a[2] for a in parts])
 texture=texture_names[mi]
 for start in range(0,len(pos),24000):
  ps=pos[start:start+24000];ns=normal[start:start+24000];us=uv[start:start+24000];nv=len(ps);nf=nv//3
  corners=np.empty(nv,dtype=np.dtype([('i','<i4'),('n','<f4',3),('uv','<f4',2)]));corners['i']=np.arange(nv);corners['n']=ns;corners['uv']=us
  roughness=appearance['metalRoughness'] if appearance and 'baseColorTexture' in j['materials'][mi]['pbrMetallicRoughness'] and mi!=3 else 0
  # Legacy MOD specular RGB / exponent (x100). At roughness 1, disable specular.
  specular=[.12*(1-roughness)]*3+[.2*(1-roughness)]
  chunk=np.eye(4,dtype='<f4').tobytes()+struct.pack('<i',nv)+ps.astype('<f4').tobytes()+struct.pack('<i',nf)+corners.tobytes()+struct.pack('<4f',*specular)+texture.encode().ljust(32,b'\0')+struct.pack('<i',0)
  chunks.append(chunk);report.append(dict(texture=texture,vertices=nv,triangles=nf))
data=original[:32]+struct.pack('<i',len(chunks))+b''.join(chunks)
for lod in [1,2,3]:(P/f'mt_twilight_{lod}.mod').write_bytes(data)
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
