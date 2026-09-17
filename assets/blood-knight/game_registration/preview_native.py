"""Independent MOD/WTM readback for visual QA (not the original GLB)."""
from pathlib import Path
from io import BytesIO
import struct,json,zlib,base64
import numpy as np
from PIL import Image
O=Path(__file__).resolve().parent;P=O/'payload';P.mkdir(exist_ok=True);b=(P/'bn_blood_1.mod').read_bytes();at=36
views=[];accessors=[];parts=[];meshes=[];materials=[];images=[];textures=[];cursor=0
def chunk(data):
 global cursor
 pad=(-cursor)%4;parts.append(b'\0'*pad);cursor+=pad;idx=len(views);views.append(dict(buffer=0,byteOffset=cursor,byteLength=len(data)));parts.append(data);cursor+=len(data);return idx
def acc(a,typ):
 v=chunk(np.ascontiguousarray(a,dtype='<f4').tobytes());accessors.append(dict(bufferView=v,componentType=5126,count=len(a),type=typ,min=a.min(axis=0).tolist(),max=a.max(axis=0).tolist()));return len(accessors)-1
for m in range(struct.unpack_from('<i',b,32)[0]):
 at+=64;nv=struct.unpack_from('<i',b,at)[0];at+=4;pos=np.frombuffer(b,dtype='<f4',count=nv*3,offset=at).reshape(-1,3);at+=nv*12;nf=struct.unpack_from('<i',b,at)[0];at+=4
 faces=np.frombuffer(b,dtype=np.dtype([('i','<i4'),('n','<f4',3),('uv','<f4',2)]),count=nf*3,offset=at);at+=nf*72+16;tex=b[at:at+32].split(b'\0')[0].decode();at+=36
 pos=pos[faces['i']];pos=np.column_stack((-pos[:,1],pos[:,0],pos[:,2]));normal=faces['n'];normal=np.column_stack((-normal[:,1],normal[:,0],normal[:,2]))
 attrs=dict(POSITION=acc(pos,'VEC3'),NORMAL=acc(normal,'VEC3'),TEXCOORD_0=acc(faces['uv'],'VEC2'))
 raw=(P/Path(tex).with_suffix('.wtm')).read_bytes();im=Image.open(BytesIO(zlib.decompress(raw[13:])));f=BytesIO();im.save(f,format='PNG');images.append(dict(bufferView=chunk(f.getvalue()),mimeType='image/png'));textures.append(dict(source=m))
 materials.append(dict(name=tex,doubleSided=False,pbrMetallicRoughness=dict(baseColorTexture=dict(index=m),metallicFactor=0,roughnessFactor=.8)))
 meshes.append(dict(name=tex,primitives=[dict(attributes=attrs,material=m)]))
j=dict(asset=dict(version='2.0'),scene=0,scenes=[dict(nodes=list(range(len(meshes))))],nodes=[dict(mesh=i) for i in range(len(meshes))],meshes=meshes,materials=materials,textures=textures,images=images,buffers=[dict(byteLength=cursor)],bufferViews=views,accessors=accessors)
jb=json.dumps(j).encode();jb+=b' '*((-len(jb))%4);bb=b''.join(parts);bb+=b'\0'*((-len(bb))%4)
glb=struct.pack('<3I',0x46546c67,2,28+len(jb)+len(bb))+struct.pack('<I4s',len(jb),b'JSON')+jb+struct.pack('<I4s',len(bb),b'BIN\0')+bb
(O/'native_readback.glb').write_bytes(glb)
V=O.parent/'openwork_3d';enc=lambda s:'data:text/javascript;base64,'+base64.b64encode(s.encode()).decode()
imports={key:enc((V/'vendor'/name).read_text(encoding='utf-8').replace('../utils/BufferGeometryUtils.js','bufferutils')) for key,name in [('three','three.module.js'),('controls','OrbitControls.js'),('loader','GLTFLoader.js'),('room','RoomEnvironment.js'),('bufferutils','BufferGeometryUtils.js')]}
html=(V/'viewer_template.html').read_text(encoding='utf-8').replace('__IMPORTMAP__',json.dumps(dict(imports=imports))).replace('__GLB__',base64.b64encode(glb).decode()).replace('Bloodfang_Openwork_Spear.glb','native_readback.glb')
(O/'게임형식_모델미리보기.html').write_text(html,encoding='utf-8')
