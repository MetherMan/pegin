import * as T from './vendor/three.module.js';
import fs from 'node:fs';
import path from 'node:path';
import {fileURLToPath} from 'node:url';
const OUT=path.dirname(fileURLToPath(import.meta.url));
const root=new T.Group();root.name='Blue_Moon_Twilight_Greatsword';
const S=.00165, AX=new T.Vector2(.808,-.589).normalize(), RX=new T.Vector2(-AX.y,AX.x), OR=new T.Vector2(100,874);
function p(x,y,z=0){const d=new T.Vector2(x,y).sub(OR);return new T.Vector3(d.dot(RX)*S,d.dot(AX)*S,z);}
function pix(x,y){return OR.clone().addScaledVector(RX,x/S).addScaledVector(AX,y/S);}
const materials={
 blade:{name:'Midnight cobalt lunar inlay',color:'#ffffff',metal:.22,rough:.48,texture:true},
 edge:{name:'Moonlit honed silver',color:'#ffffff',metal:.62,rough:.38,texture:'steel'},
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
function project(g){const pos=g.attributes.position,uv=g.attributes.uv;for(let i=0;i<pos.count;i++){const a=pix(pos.getX(i),pos.getY(i));uv.setXY(i,a.x/1536,1-a.y/1024);}return g;}
function unwrapSidewalls(g,points,holes,z){
 const loops=[points,...holes].map(ring=>{const pts=ring.map(q=>p(...q));let total=0;const segments=pts.map((a,i)=>{const b=pts[(i+1)%pts.length],d=b.clone().sub(a),length=d.length(),s={a,d,length,start:total};total+=length;return s;});return {segments,total};});
 const pos=g.attributes.position,uv=g.attributes.uv,perimeters=[];
 for(let i=0;i<pos.count;i++){const q=new T.Vector3(pos.getX(i),pos.getY(i),0);let best;
  for(const loop of loops)for(const s of loop.segments){const t=T.MathUtils.clamp(q.clone().sub(s.a).dot(s.d)/(s.length*s.length),0,1),distance=q.distanceToSquared(s.a.clone().addScaledVector(s.d,t));if(!best||distance<best.distance)best={distance,along:s.start+t*s.length,total:loop.total};}
  // Unwrap the thickness independently of the front projection. Every side
  // triangle now samples real 2D texture area instead of a collapsed UV line.
  uv.setXY(i,(pos.getZ(i)-z)/.08,best.along/.22);perimeters.push(best.total/.22);
 }
 for(let i=0;i<pos.count;i+=3){const vs=[uv.getY(i),uv.getY(i+1),uv.getY(i+2)],total=perimeters[i];if(Math.max(...vs)-Math.min(...vs)>total*.5)for(let k=0;k<3;k++)if(vs[k]<total*.5)uv.setY(i+k,vs[k]+total);}
}
function solidShape(points,holes,depth,z,key,name,bevel=.002){
 const geo=new T.ExtrudeGeometry(shapeOf(points,holes),{depth,bevelEnabled:bevel>0,bevelThickness:bevel,bevelSize:bevel,bevelSegments:2,steps:1,curveSegments:1});geo.translate(0,0,z);project(geo);
 // Planar faces retain their reference projection. Sidewalls and bevels use
 // a dedicated metal tile with perimeter/thickness UVs.
 let face;
 for(const group of geo.groups){const g=new T.BufferGeometry();for(const [attr,a] of Object.entries(geo.attributes)){g.setAttribute(attr,new T.BufferAttribute(a.array.slice(group.start*a.itemSize,(group.start+group.count)*a.itemSize),a.itemSize));}if(group.materialIndex!==0)unwrapSidewalls(g,points,holes,z);const m=add(g,group.materialIndex===0?key:'edge',name+(group.materialIndex===0?' | face':' | solid metal bevel'));if(group.materialIndex===0)face=m;}
 return face;
}
const main=[[479,635],[521,578],[563,538],[628,500],[702,465],[789,420],[883,379],[986,327],[1094,268],[1205,204],[1317,136],[1438,52],[1498,15],[1468,97],[1433,163],[1380,242],[1318,322],[1240,413],[1172,482],[1096,546],[1016,608],[916,665],[963,599],[1006,534],[1039,477],[1053,440],[1051,424],[1043,415],[1027,409],[1004,406],[978,409],[918,421],[867,436],[809,461],[752,493],[691,532],[630,574],[559,627],[495,647]];
solidShape(main,[],.035,-.0175,'blade','01 | long pierced moon blade',.0025);
// Replace the reference's dark tip wedge with a raised, faceted steel end cap.
// Its distinct normals give the tip a continuous metallic finish on both faces.
for(const side of [-1,1]){
 const corners=[[1438,52],[1498,15],[1390,191]],center=p(1442,91,side*.028),ps=[],us=[],samples=[[1360,258],[1400,213],[1340,290],[1368,248]];
 for(let i=0;i<3;i++){const a=p(...corners[i],side*.020),b=p(...corners[(i+1)%3],side*.020),vs=[a,b,center],ids=[i,(i+1)%3,3];for(const k of (side>0?[0,1,2]:[1,0,2])){ps.push(...vs[k].toArray());us.push(samples[ids[k]][0]/1536,1-samples[ids[k]][1]/1024);}}
 const g=new T.BufferGeometry();g.setAttribute('position',new T.Float32BufferAttribute(ps,3));g.setAttribute('uv',new T.Float32BufferAttribute(us,2));g.computeVertexNormals();add(g,'blade','01b | textured steel tip facets');
}
// Separate blade fins leave genuine open space, including three small pierced windows.
const left=[[245,389],[282,419],[341,442],[418,451],[493,453],[561,447],[605,447],[655,431],[693,408],[719,376],[736,339],[744,298],[751,256],[758,223],[752,181],[742,123],[763,157],[782,208],[796,271],[799,330],[791,385],[811,408],[777,442],[726,474],[667,496],[599,511],[529,517],[455,511],[388,498],[328,476],[282,444]];
solidShape(left,[],.025,-.0125,'blade','02 | swept crescent crown and upper fin',.002);
const bottom=[[486,623],[515,594],[548,568],[587,550],[627,544],[665,551],[704,570],[736,600],[762,639],[783,688],[801,745],[802,793],[795,824],[784,772],[765,722],[737,682],[704,651],[665,629],[627,613],[589,609],[551,613],[514,628]];
solidShape(bottom,[],.029,-.0145,'blade','03 | lower crescent cutting guard',.002);
// Structural swept ribs, individually extruded, connect the crown to the handle.
solidShape([[487,622],[535,571],[598,529],[667,486],[746,440],[827,401],[898,377],[813,420],[734,467],[655,511],[589,551],[537,592],[501,635]],[],.026,-.013,'blade','04 | diagonal pierced guard rib',.0015);
// Remove the marked inner branch rather than filling the surrounding space.
// Connect only the cyan-marked narrow cutting line, using the original steel
// texture from its adjacent ridge. This is a 10–12 pixel edge, not a cover plate.
const junction=JSON.parse(fs.readFileSync(path.join(OUT,'junction_steel_uv.json'),'utf8'));
const junctionPoints=junction.path.map(q=>new T.Vector2(...q)),sides=[[],[]];
for(let i=0;i<junctionPoints.length;i++){const tangent=junctionPoints[Math.min(i+1,junctionPoints.length-1)].clone().sub(junctionPoints[Math.max(0,i-1)]).normalize(),n=new T.Vector2(-tangent.y,tangent.x),w=(i===0||i===junctionPoints.length-1)?1.5:4.5;for(let k=0;k<2;k++)sides[k].push(junctionPoints[i].clone().addScaledVector(n,k===0?w:-w).toArray());}
const junctionFace=solidShape([...sides[0],...sides[1].reverse()],[],.040,-.020,'blade','05 | narrow continuous textured cutting line',.0006);
const junctionPointsos=junctionFace.geometry.attributes.position,juv=junctionFace.geometry.attributes.uv;
for(let i=0;i<junctionPointsos.count;i++){const q=pix(junctionPointsos.getX(i),junctionPointsos.getY(i));let best;
 for(let j=0;j<junctionPoints.length-1;j++){const delta=junctionPoints[j+1].clone().sub(junctionPoints[j]),t=T.MathUtils.clamp(q.clone().sub(junctionPoints[j]).dot(delta)/delta.lengthSq(),0,1),near=junctionPoints[j].clone().addScaledVector(delta,t),dist=q.distanceToSquared(near);if(!best||dist<best.dist)best={j,t,dist,offset:q.clone().sub(near).dot(new T.Vector2(-delta.y,delta.x).normalize())};}
 const {j,t,offset}=best,s0=new T.Vector2(...junction.samples[j]),s1=new T.Vector2(...junction.samples[j+1]),sample=s0.lerp(s1,t).add(new T.Vector2(.55,.83).multiplyScalar(offset*.12));juv.setXY(i,sample.x/1536,1-sample.y/1024);
}
// Ground edges have real thickness and a narrow bright bevel ridge on both sides.
for(const [name,pts] of [['outer tip',main.slice(13,24)],['crescent guard',bottom.slice(0,15)],['left fin',left.slice(0,11)]]){
 for(const z of [-.019,.019])tube(pts.map(q=>p(...q,z)),.0015,'edge','06 | cutting bevel '+name,Math.max(24,pts.length*3),5);
}
// Raised crescent crest (the image also carries fine engraved ornament).
const crescent=[];for(let i=0;i<=25;i++){let a=(70+i*270/25)*Math.PI/180;crescent.push([1224+54*Math.cos(a),270+54*Math.sin(a)]);}for(let i=0;i<=25;i++){let a=(343-i*255/25)*Math.PI/180;crescent.push([1238+43*Math.cos(a),257+43*Math.sin(a)]);}
for(const z of [.020,-.027])solidShape(crescent,[],.007,z,'blade','07 | raised crescent moon insignia',.0007);
for(const sign of [-1,1])for(const [x,y,size] of [[1287,218,6],[968,406,4],[1100,352,3]]){const gem=add(new T.SphereGeometry(size*S,12,8),'sapphire','08 | moonstone inlay');gem.position.copy(p(x,y,sign*.027));gem.scale.z=.48;}
// Round sword hilt, not a flat painted rectangle.
const a=p(220,792),b=p(480,645),d=b.clone().sub(a).normalize(),len=a.distanceTo(b),side=new T.Vector3(d.y,-d.x,0),front=new T.Vector3(0,0,1);
cylinderBetween(a,b,.025,.024,'leather','09 | two-handed dark leather grip',24);
for(let i=0;i<15;i++){
 const pts=[];for(let j=0;j<=28;j++){const t=j/28,angle=t*Math.PI*2;pts.push(a.clone().addScaledVector(d,(i+t)/15*len).addScaledVector(side,.027*Math.cos(angle)).addScaledVector(front,.027*Math.sin(angle)));}
 // A tight leather wrap stays flush against the grip; no loose bandages.
 tube(pts,.0042,'leather','10 | tight leather grip binding '+i,28,5);
}
for(const t of [0,.04,.96,1]){const c=a.clone().addScaledVector(d,t*len);cylinderBetween(c.clone().addScaledVector(d,-.01),c.clone().addScaledVector(d,.01),.033,.033,'silver','11 | silver grip ferrule');}
const pommelStart=components.length;
solidShape([[225,776],[236,798],[219,820],[188,833],[165,856],[163,888],[144,908],[119,916],[94,916],[59,929],[23,953],[50,915],[63,880],[72,849],[85,829],[132,812],[115,833],[145,831],[168,815],[190,803]],[],.035,-.0175,'blade','12 | crescent pommel',.002);
// Use the reference's own gem surface for both domes: a second, plain blue
// sphere over the painted stone gave the pommel a doubled, off-center jewel.
for(const sign of [-1,1]){
 const positions=[],indices=[],uvs=[],rings=6,segs=32;
 for(let i=0;i<=rings;i++)for(let j=0;j<=segs;j++){const r=i/rings,angle=j/segs*Math.PI*2,q=p(111+22*r*Math.cos(angle),861+24*r*Math.sin(angle),sign*(.018+.009*Math.sqrt(1-r*r)));positions.push(...q.toArray());uvs.push(0,0);}
 for(let i=0;i<rings;i++)for(let j=0;j<segs;j++){const n=i*(segs+1)+j;const tri=[n,n+1,n+segs+1,n+1,n+segs+2,n+segs+1];indices.push(...(sign>0?tri:tri.reverse()));}
 const g=new T.BufferGeometry();g.setAttribute('position',new T.Float32BufferAttribute(positions,3));g.setAttribute('uv',new T.Float32BufferAttribute(uvs,2));g.setIndex(indices);project(g);g.computeVertexNormals();add(g,'blade','13 | reference moonstone dome');
}
// Straighten the traced centerline without moving its attachment or changing
// the UVs. Root, jewel center and terminal point all lie on the grip axis.
const pommelKnots=[[23,953],[111,861],[220,792]].map(q=>{const v=p(...q).sub(a);return {t:v.dot(d),offset:v.dot(side)};});
function pommelOffset(t){if(t<=pommelKnots[0].t)return pommelKnots[0].offset;if(t>=0)return 0;for(let i=0;i<2;i++){const lo=pommelKnots[i],hi=pommelKnots[i+1];if(t<=hi.t)return T.MathUtils.lerp(lo.offset,hi.offset,(t-lo.t)/(hi.t-lo.t));}return 0;}
for(const mesh of components.slice(pommelStart)){const pos=mesh.geometry.attributes.position;for(let i=0;i<pos.count;i++){const q=new T.Vector3().fromBufferAttribute(pos,i),t=q.clone().sub(a).dot(d);q.addScaledVector(side,-pommelOffset(t));pos.setXYZ(i,...q.toArray());}pos.needsUpdate=true;mesh.geometry.computeVertexNormals();}
for(const q of [[111,861],[23,953]]){const v=p(...q).sub(a),aligned=v.clone().addScaledVector(side,-pommelOffset(v.dot(d)));if(Math.abs(aligned.dot(side))>1e-7)throw Error('Pommel is off the grip axis');}
// Tiny engraved concentric moon tracks on the broad face, kept on the blade face.
for(const sign of [-1,1]){const ring=[];for(let i=0;i<=70;i++){const a=i/70*Math.PI*2;ring.push(p(1227+66*Math.cos(a),267+66*Math.sin(a),sign*.024));}tube(ring,.00065,'silver','18 | lunar orbit engraving',70,4);}
// Match the original spear's limits around the native hand/grip origin.
// Extend the broad cutting blade toward the hilt while preserving the tip,
// grip, crown fins, circular moon crest and overall Black Knight size limit.
// The old texture coordinates move with the surface; no untextured filler.
function splitBladeAtY(source,cut){
 const geo=source.index?source.toNonIndexed():source,keys=Object.keys(geo.attributes),out=Object.fromEntries(keys.map(k=>[k,[]]));
 function mixVertex(a,b,t){return Object.fromEntries(keys.map(k=>[k,a[k].map((v,i)=>v+(b[k][i]-v)*t)]));}
 function clip(poly,sign){const result=[];for(let i=0;i<poly.length;i++){const a=poly[i],b=poly[(i+1)%poly.length],da=sign*(a.position[1]-cut),db=sign*(b.position[1]-cut);if(da>=0)result.push(a);if((da<0&&db>0)||(da>0&&db<0))result.push(mixVertex(a,b,da/(da-db)));}return result;}
 for(let i=0;i<geo.attributes.position.count;i+=3){
  const tri=[0,1,2].map(n=>Object.fromEntries(keys.map(k=>{const a=geo.attributes[k];return [k,Array.from(a.array.slice((i+n)*a.itemSize,(i+n+1)*a.itemSize))];})));
  const ys=tri.map(v=>v.position[1]),polys=Math.min(...ys)<cut&&Math.max(...ys)>cut?[clip(tri,1),clip(tri,-1)]:[tri];
  for(const poly of polys)for(let j=1;j<poly.length-1;j++){
   const face=[poly[0],poly[j],poly[j+1]],a=new T.Vector3(...face[0].position),b=new T.Vector3(...face[1].position),c=new T.Vector3(...face[2].position);
   if(b.sub(a).cross(c.sub(a)).lengthSq()<1e-20)continue;
   for(const v of face)for(const k of keys)out[k].push(...v[k]);
  }
 }
 const result=new T.BufferGeometry();for(const k of keys)result.setAttribute(k,new T.Float32BufferAttribute(out[k],geo.attributes[k].itemSize));return result;
}
function bladeLongitudinalY(y){
 const rootY=.73,shoulderY=1.50,unchangedY=1.965,extension=.30;
 if(y<=rootY||y>=unchangedY)return y;
 const weight=y<=shoulderY?(y-rootY)/(shoulderY-rootY):(unchangedY-y)/(unchangedY-shoulderY);
 return y-extension*weight;
}
for(const mesh of components){
 if(!mesh.name.startsWith('01 |')&&!mesh.name.startsWith('06 | cutting bevel outer tip'))continue;
 // Split across deformation boundaries so long face triangles cannot drag
 // the painted moon away from its unchanged raised crest and engraved ring.
 for(const cut of [.73,1.50,1.965])mesh.geometry=splitBladeAtY(mesh.geometry,cut);
 const attr=mesh.geometry.attributes.position;
 for(let i=0;i<attr.count;i++)attr.setY(i,bladeLongitudinalY(attr.getY(i)));
 attr.needsUpdate=true;mesh.geometry.computeVertexNormals();
}
const sizeSpec=JSON.parse(fs.readFileSync(path.join(OUT,'size_spec.json'),'utf8'));
root.scale.setScalar(sizeSpec.scale);root.position.y=.5*(1-sizeSpec.scale);
root.updateMatrixWorld(true);
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
const info={name:'푸른달의 트와일라잇 양손검',rank:9,name_color:'#72CFFF',rank_color:'#348BFF',type:2,triangles:stats.reduce((a,b)=>a+b.triangles,0),mesh_objects:stats.length,cloth_strips:0,source:'concept_texture.png',not_in_game:false,construction:'Real extruded blade and separate crescent fins, open negative spaces, round wrapped hilt, tight dark leather grip. Both blade faces use the concept projection; some painted lighting remains in the texture.',parts:stats};
fs.writeFileSync(path.join(OUT,'model_info.json'),JSON.stringify(info,null,2));
// Readable editable OBJ companion with the same geometry and material assignments.
let obj=['mtllib Blue_Moon_Twilight_Greatsword.mtl'];let idx=1;
for(const mesh of components){let g=bakedGeometry(mesh);const a=g.attributes.position,n=g.attributes.normal,u=g.attributes.uv;obj.push('o '+mesh.name.replaceAll(' ','_').replaceAll('|',''),'usemtl '+mesh.material.name);for(let i=0;i<a.count;i++)obj.push(`v ${a.getX(i)} ${a.getY(i)} ${a.getZ(i)}`);for(let i=0;i<a.count;i++)obj.push(`vt ${u?u.getX(i):0} ${u?u.getY(i):0}`);for(let i=0;i<a.count;i++)obj.push(`vn ${n.getX(i)} ${n.getY(i)} ${n.getZ(i)}`);for(let i=0;i<a.count;i+=3)obj.push('f '+[idx+i,idx+i+1,idx+i+2].map(j=>`${j}/${j}/${j}`).join(' '));idx+=a.count;}
fs.writeFileSync(path.join(OUT,'Blue_Moon_Twilight_Greatsword.obj'),obj.join('\n'));
fs.writeFileSync(path.join(OUT,'Blue_Moon_Twilight_Greatsword.mtl'),matkeys.map(k=>`newmtl ${k}\nKd ${new T.Color(materials[k].color).toArray().join(' ')}\nNs 50\n${materials[k].texture?'map_Kd '+(materials[k].texture==='cloth'?'bandage_linen_albedo.png':materials[k].texture==='steel'?'moonsteel_side_albedo.png':'concept_texture.png')+'\n':''}`).join('\n'));
console.log(JSON.stringify({triangles:info.triangles,meshes:info.mesh_objects,cloth_strips:0,glb_bytes:28+json.length+bin.length}));
