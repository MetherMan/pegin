import * as T from './vendor/three.module.js';
import fs from 'node:fs';
import path from 'node:path';
import {fileURLToPath} from 'node:url';
const OUT=path.dirname(fileURLToPath(import.meta.url));
const root=new T.Group();root.name='Blue_Moon_Twilight_Greatsword';
const design=JSON.parse(fs.readFileSync(path.join(OUT,'design04_geometry.json'),'utf8'));
const volumes=JSON.parse(fs.readFileSync(path.join(OUT,'volume_geometry.json'),'utf8'));
const [W,H]=design.image_size,S=.00165,CX=design.center_x,CY=1112;
function p(x,y,z=0){return new T.Vector3((x-CX)*S,(CY-y)*S,z);}
function pix(x,y){return new T.Vector2(x/S+CX,CY-y/S);}
const materials={
 blade:{name:'Midnight cobalt lunar inlay',color:'#ffffff',metal:.22,rough:.48,texture:true},
 edge:{name:'Moonlit honed silver',color:'#b8c0cd',metal:.62,rough:.58,texture:'steel'},
 dark:{name:'Blued steel spine',color:'#142035',metal:.72,rough:.4},
 cloth:{name:'Weathered ivory woven bandage',color:'#aaa798',metal:0,rough:.94,texture:'cloth'},
 seam:{name:'Linen edge stitching',color:'#655f53',metal:0,rough:.98},
 leather:{name:'Midnight wrapped grip',color:'#172033',metal:0,rough:.74},
 sapphire:{name:'Blue moonstone',color:'#245bc4',metal:.46,rough:.17},
 silver:{name:'Lunar relief silver',color:'#ffffff',metal:.62,rough:.38,texture:'steel'},
};
const tmat={};for(const [k,m] of Object.entries(materials))tmat[k]=new T.MeshStandardMaterial({name:k,color:m.color,metalness:m.metal,roughness:m.rough,side:T.DoubleSide});
const components=[];
function add(g,key,name){if(!g.getAttribute('normal'))g.computeVertexNormals();const mesh=new T.Mesh(g,tmat[key]);mesh.name=name;root.add(mesh);components.push(mesh);return mesh;}
function curve(points,closed=false){return new T.CatmullRomCurve3(points,closed,'centripetal');}
function tube(points,radius,key,name,segments=40,sides=6){return add(new T.TubeGeometry(curve(points),segments,radius,sides,false),key,name);}
function cylinderBetween(a,b,r1,r2,key,name,sides=20){const d=b.clone().sub(a);const mesh=add(new T.CylinderGeometry(r2,r1,d.length(),sides,1,false),key,name);mesh.position.copy(a).add(b).multiplyScalar(.5);mesh.quaternion.setFromUnitVectors(new T.Vector3(0,1,0),d.normalize());return mesh;}
function shapeOf(points,holes=[]){let q=points.map(a=>p(...a));let s=new T.Shape(q.map(v=>new T.Vector2(v.x,v.y)));for(const ring of holes){let h=ring.map(a=>p(...a));s.holes.push(new T.Path(h.map(v=>new T.Vector2(v.x,v.y))));}return s;}
function project(g){const pos=g.attributes.position,uv=g.attributes.uv;for(let i=0;i<pos.count;i++){const a=pix(pos.getX(i),pos.getY(i));uv.setXY(i,a.x/W,1-a.y/H);}return g;}
function unwrapSidewalls(g,points,holes,z,depth){
 const loops=[points,...holes].map(ring=>{const pts=ring.map(q=>p(...q));let total=0;const segments=pts.map((a,i)=>{const b=pts[(i+1)%pts.length],d=b.clone().sub(a),length=d.length(),s={a,d,length,start:total};total+=length;return s;});return {segments,total};});
 const pos=g.attributes.position,uv=g.attributes.uv,perimeters=[];
 for(let i=0;i<pos.count;i++){const q=new T.Vector3(pos.getX(i),pos.getY(i),0);let best;
  for(const loop of loops)for(const s of loop.segments){const t=T.MathUtils.clamp(q.clone().sub(s.a).dot(s.d)/(s.length*s.length),0,1),distance=q.distanceToSquared(s.a.clone().addScaledVector(s.d,t));if(!best||distance<best.distance)best={distance,along:s.start+t*s.length,total:loop.total};}
  // Unwrap the thickness independently of the front projection. Every side
  // triangle now samples real 2D texture area instead of a collapsed UV line.
  uv.setXY(i,(pos.getZ(i)-z)/depth,best.along/.22);perimeters.push(best.total/.22);
 }
 for(let i=0;i<pos.count;i+=3){const vs=[uv.getY(i),uv.getY(i+1),uv.getY(i+2)],total=perimeters[i];if(Math.max(...vs)-Math.min(...vs)>total*.5)for(let k=0;k<3;k++)if(vs[k]<total*.5)uv.setY(i+k,vs[k]+total);}
}
function solidShape(points,holes,depth,z,key,name,bevel=.002){
 const geo=new T.ExtrudeGeometry(shapeOf(points,holes),{depth,bevelEnabled:bevel>0,bevelThickness:bevel,bevelSize:bevel,bevelSegments:2,steps:1,curveSegments:1});geo.translate(0,0,z);project(geo);
 // Planar faces retain their reference projection. Sidewalls and bevels use
 // a dedicated metal tile with perimeter/thickness UVs.
 let face;
 for(const group of geo.groups){const g=new T.BufferGeometry();for(const [attr,a] of Object.entries(geo.attributes)){g.setAttribute(attr,new T.BufferAttribute(a.array.slice(group.start*a.itemSize,(group.start+group.count)*a.itemSize),a.itemSize));}if(group.materialIndex!==0)unwrapSidewalls(g,points,holes,z,depth);const m=add(g,group.materialIndex===0?key:'edge',name+(group.materialIndex===0?' | face':' | solid metal bevel'));if(group.materialIndex===0)face=m;}
 return face;
}

// All auxiliary blades share a continuous root with the primary blade.
// Image-derived holes are genuine topology, including the round lunar aperture.
for(const part of design.parts){
 const title=part.name.startsWith('main')?'01 | sculpted blade, spine and guard':'12 | solid crescent pommel';
 for(const [surface,data] of Object.entries(volumes[part.name])){
  const g=new T.BufferGeometry();
  for(const [key,size] of [['position',3],['normal',3],['uv',2]])g.setAttribute(key,new T.Float32BufferAttribute(data[key],size));
  // The geometry tool uses image-space V; GLB export below performs its own
  // flip, so keep the builder's usual bottom-up UV convention here.
  const uv=g.attributes.uv;for(let i=0;i<uv.count;i++)uv.setY(i,1-uv.getY(i));
  add(g,surface==='face'?'blade':'edge',title+' | '+surface);
 }
}
// A circular grip gives the edge-on view real volume instead of a flat card.
const top=p(CX,design.grip_top),bottom=p(CX,design.grip_bottom);
const grip=cylinderBetween(bottom,top,design.grip_radius_pixels*S,design.grip_radius_pixels*S,'blade','09 | cylindrical blue leather two-hand grip',32);
const guv=grip.geometry.attributes.uv;
for(let i=0;i<guv.count;i++){
 const u=guv.getX(i),v=guv.getY(i);
 guv.setXY(i,(466+42*u)/W,1-(1043+(1-v)*216)/H);
}
// Preserve the exact previous total length and the Black Knight grip-to-tip limit.
root.updateMatrixWorld(true);
const before=new T.Box3().setFromObject(root);
const sizeSpec=JSON.parse(fs.readFileSync(path.join(OUT,'size_spec.json'),'utf8'));
const scale=sizeSpec.new_length/(before.max.y-before.min.y);
root.scale.setScalar(scale);
root.position.y=.5+sizeSpec.reference_grip_to_tip-before.max.y*scale;
root.updateMatrixWorld(true);
const finalBounds=new T.Box3().setFromObject(root);
if(Math.abs(finalBounds.max.y-.5-sizeSpec.reference_grip_to_tip)>1e-6)throw Error('Grip-to-tip mismatch');
if(Math.abs(finalBounds.max.y-finalBounds.min.y-sizeSpec.new_length)>1e-6)throw Error('Length mismatch');
function bakedGeometry(mesh){let g=mesh.geometry.clone();if(g.index)g=g.toNonIndexed();g.applyMatrix4(mesh.matrixWorld);return g;}
const blobs=[],views=[],accessors=[],meshes=[],nodes=[],matkeys=Object.keys(materials);let cursor=0;
function chunk(buffer,target){const pad=(4-cursor%4)%4;if(pad){blobs.push(Buffer.alloc(pad));cursor+=pad;}const index=views.length;views.push({buffer:0,byteOffset:cursor,byteLength:buffer.length,...(target?{target}:{})});blobs.push(buffer);cursor+=buffer.length;return index;}
function acc(array,size,type,range=false){const a=Float32Array.from(array),v=chunk(Buffer.from(a.buffer),34962),o={bufferView:v,componentType:5126,count:a.length/size,type};if(range){o.min=Array(size).fill(Infinity);o.max=Array(size).fill(-Infinity);for(let i=0;i<a.length;i++){o.min[i%size]=Math.min(o.min[i%size],a[i]);o.max[i%size]=Math.max(o.max[i%size],a[i]);}}accessors.push(o);return accessors.length-1;}
const stats=[];
for(const mesh of components){
 let g=bakedGeometry(mesh);const ps=g.attributes.position.array,ns=g.attributes.normal.array;
 if(!Array.from(ps).every(Number.isFinite)||!Array.from(ns).every(Number.isFinite))throw Error('Nonfinite geometry '+mesh.name);
 const attrs={POSITION:acc(ps,3,'VEC3',true),NORMAL:acc(ns,3,'VEC3')};
 if(g.attributes.uv){let uv=Array.from(g.attributes.uv.array);for(let i=1;i<uv.length;i+=2)uv[i]=1-uv[i];attrs.TEXCOORD_0=acc(uv,2,'VEC2');}
 meshes.push({name:mesh.name,primitives:[{attributes:attrs,material:matkeys.indexOf(mesh.material.name)}]});nodes.push({name:mesh.name,mesh:meshes.length-1});stats.push({name:mesh.name,triangles:ps.length/9});
}
const texture=fs.readFileSync(path.join(OUT,'concept_texture.png')),iv=chunk(texture);
const clothImage=chunk(fs.readFileSync(path.join(OUT,'bandage_linen_albedo.png')));
const steelImage=chunk(fs.readFileSync(path.join(OUT,'moonsteel_side_albedo.png')));
const gltf={asset:{version:'2.0',generator:'Twilight reference-guided geometry builder'},scene:0,scenes:[{nodes:nodes.map((_,i)=>i)}],nodes,meshes,buffers:[{byteLength:cursor}],bufferViews:views,accessors,
 materials:matkeys.map(k=>{const m=materials[k];return {name:m.name,doubleSided:true,pbrMetallicRoughness:{baseColorFactor:[...new T.Color(m.color).toArray(),1],metallicFactor:m.metal,roughnessFactor:m.rough,...(m.texture?{baseColorTexture:{index:m.texture==='cloth'?1:m.texture==='steel'?2:0}}:{})}};}),textures:[{source:0,sampler:0},{source:1,sampler:1},{source:2,sampler:1}],samplers:[{magFilter:9729,minFilter:9987,wrapS:33071,wrapT:33071},{magFilter:9729,minFilter:9987,wrapS:10497,wrapT:10497}],images:[{bufferView:iv,mimeType:'image/png'},{bufferView:clothImage,mimeType:'image/png'},{bufferView:steelImage,mimeType:'image/png'}]};
let json=Buffer.from(JSON.stringify(gltf)),jp=Buffer.alloc((4-json.length%4)%4,32),bin=Buffer.concat(blobs),bp=Buffer.alloc((4-bin.length%4)%4);json=Buffer.concat([json,jp]);bin=Buffer.concat([bin,bp]);
const head=Buffer.alloc(12);head.writeUInt32LE(0x46546c67);head.writeUInt32LE(2,4);head.writeUInt32LE(28+json.length+bin.length,8);
const jhead=Buffer.alloc(8);jhead.writeUInt32LE(json.length);jhead.write('JSON',4);const bhead=Buffer.alloc(8);bhead.writeUInt32LE(bin.length);bhead.write('BIN\0',4);
fs.writeFileSync(path.join(OUT,'Blue_Moon_Twilight_Greatsword.glb'),Buffer.concat([head,jhead,json,bhead,bin]));
const info={name:'푸른달의 트와일라잇 양손검',rank:9,name_color:'#72CFFF',rank_color:'#72CFFF',type:2,triangles:stats.reduce((a,b)=>a+b.triangles,0),mesh_objects:stats.length,cloth_strips:0,source:'concept_texture.png',revision:'approved design 04 - sculpted body and thin cutting rim',construction_transform:{pixel_scale:S*scale,x_origin:CX,y_offset:CY*S*scale+root.position.y},not_in_game:false,construction:'Sculpted 04 silhouette: thin cutting rims, thicker spine, guard and crescent pommel, true apertures, round grip and independently mapped metal sidewalls. One rigid animation mesh.',parts:stats};
fs.writeFileSync(path.join(OUT,'model_info.json'),JSON.stringify(info,null,2));
// Readable editable OBJ companion with the same geometry and material assignments.
let obj=['mtllib Blue_Moon_Twilight_Greatsword.mtl'];let idx=1;
for(const mesh of components){let g=bakedGeometry(mesh);const a=g.attributes.position,n=g.attributes.normal,u=g.attributes.uv;obj.push('o '+mesh.name.replaceAll(' ','_').replaceAll('|',''),'usemtl '+mesh.material.name);for(let i=0;i<a.count;i++)obj.push(`v ${a.getX(i)} ${a.getY(i)} ${a.getZ(i)}`);for(let i=0;i<a.count;i++)obj.push(`vt ${u?u.getX(i):0} ${u?u.getY(i):0}`);for(let i=0;i<a.count;i++)obj.push(`vn ${n.getX(i)} ${n.getY(i)} ${n.getZ(i)}`);for(let i=0;i<a.count;i+=3)obj.push('f '+[idx+i,idx+i+1,idx+i+2].map(j=>`${j}/${j}/${j}`).join(' '));idx+=a.count;}
fs.writeFileSync(path.join(OUT,'Blue_Moon_Twilight_Greatsword.obj'),obj.join('\n'));
fs.writeFileSync(path.join(OUT,'Blue_Moon_Twilight_Greatsword.mtl'),matkeys.map(k=>`newmtl ${k}\nKd ${new T.Color(materials[k].color).toArray().join(' ')}\nNs 50\n${materials[k].texture?'map_Kd '+(materials[k].texture==='cloth'?'bandage_linen_albedo.png':materials[k].texture==='steel'?'moonsteel_side_albedo.png':'concept_texture.png')+'\n':''}`).join('\n'));
console.log(JSON.stringify({triangles:info.triangles,meshes:info.mesh_objects,cloth_strips:0,glb_bytes:28+json.length+bin.length}));
