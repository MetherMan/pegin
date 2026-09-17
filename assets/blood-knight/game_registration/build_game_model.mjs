import * as T from '../openwork_3d/vendor/three.module.js';
import fs from 'node:fs';
import path from 'node:path';
import {fileURLToPath} from 'node:url';
const OUT=path.dirname(fileURLToPath(import.meta.url));
const root=new T.Group();root.name='Bloodfang_Openwork_Spear_Game';
const S=.0019, AX=new T.Vector2(.329,-.944).normalize(), RX=new T.Vector2(-AX.y,AX.x), OR=new T.Vector2(188,1640);
function p(x,y,z=0){const d=new T.Vector2(x,y).sub(OR);return new T.Vector3(d.dot(RX)*S,d.dot(AX)*S,z);}
function pix(x,y){return OR.clone().addScaledVector(RX,x/S).addScaledVector(AX,y/S);}
let seed=94133;function rnd(){seed=(Math.imul(seed,1664525)+1013904223)>>>0;return seed/4294967296;}
const materials={
 blade:{name:'Concept-painted oxblood steel',color:'#ffffff',metal:.18,rough:.52,texture:true},
 edge:{name:'Honed silver bevel',color:'#c0c8cd',metal:.84,rough:.31},
 dark:{name:'Forged dark steel',color:'#333135',metal:.83,rough:.39},
 chain:{name:'Worn iron chain',color:'#68625b',metal:.86,rough:.32},
 bronze:{name:'Aged bronze fittings',color:'#655140',metal:.8,rough:.43},
 leather:{name:'Oxblood leather',color:'#321819',metal:0,rough:.85},
 seam:{name:'Leather seams',color:'#100e10',metal:0,rough:.94},
 ruby:{name:'Blood-fang ruby',color:'#580710',metal:.34,rough:.23},
 ivory:{name:'Worn engraving silver',color:'#b1a393',metal:.7,rough:.4},
 fur0:{name:'Beast fur shadow',color:'#170f0e',metal:0,rough:.96},
 fur1:{name:'Beast fur brown',color:'#3a271d',metal:0,rough:.91},
 fur2:{name:'Beast fur ash tips',color:'#6c4c34',metal:0,rough:.9},
 fur3:{name:'Blood matted fur',color:'#33070d',metal:0,rough:.57},
 fur4:{name:'Crimson fur tips',color:'#5c0b14',metal:0,rough:.49},
};
const tmat={};for(const [k,m] of Object.entries(materials))tmat[k]=new T.MeshStandardMaterial({name:k,color:m.color,metalness:m.metal,roughness:m.rough,side:T.DoubleSide});
const components=[];
function add(g,key,name){if(!g.getAttribute('normal'))g.computeVertexNormals();const mesh=new T.Mesh(g,tmat[key]);mesh.name=name;root.add(mesh);components.push(mesh);return mesh;}
function curve(points,closed=false){return new T.CatmullRomCurve3(points,closed,'centripetal');}
function tube(points,radius,key,name,segments=40,sides=8){return add(new T.TubeGeometry(curve(points),(name.includes('spiral')?segments:Math.min(segments,48)),radius,Math.min(sides,6),false),key,name);}
function cylinderBetween(a,b,r1,r2,key,name,sides=24){const d=b.clone().sub(a);const mesh=add(new T.CylinderGeometry(r2,r1,d.length(),sides,1,false),key,name);mesh.position.copy(a).add(b).multiplyScalar(.5);mesh.quaternion.setFromUnitVectors(new T.Vector3(0,1,0),d.normalize());return mesh;}
const outline=[[757,18],[759,120],[758,220],[752,320],[741,420],[726,519],[703,614],[678,696],[650,750],[650,798],[641,826],[620,854],[594,883],[606,847],[616,812],[615,789],[604,776],[590,778],[576,790],[563,811],[554,839],[555,865],[564,895],[578,913],[562,913],[550,932],[540,953],[535,980],[540,1020],[550,1058],[520,1025],[496,1007],[475,992],[454,971],[440,956],[444,932],[453,906],[463,880],[472,851],[480,822],[486,793],[490,767],[476,741],[449,722],[408,709],[432,694],[454,675],[475,647],[491,618],[508,581],[521,549],[529,525],[499,489],[526,459],[552,423],[578,384],[604,341],[628,297],[650,255],[680,196],[712,128],[739,58]];
const hole=[[720,161],[717,234],[707,321],[691,410],[670,492],[646,556],[617,605],[586,633],[588,600],[601,554],[618,501],[637,448],[656,393],[675,336],[695,268],[710,207]];
function shapeOf(points,holes=[]){let q=points.map(a=>p(...a));let s=new T.Shape(q.map(v=>new T.Vector2(v.x,v.y)));for(const ring of holes){let h=ring.map(a=>p(...a));s.holes.push(new T.Path(h.map(v=>new T.Vector2(v.x,v.y))));}return s;}
function solidShape(points,holes,depth,z,key,name,bevel=.002){
 const geo=new T.ExtrudeGeometry(shapeOf(points,holes),{depth,bevelEnabled:bevel>0,bevelThickness:bevel,bevelSize:bevel,bevelSegments:2,steps:1,curveSegments:1});
 geo.translate(0,0,z);
 // Project the concept onto the entire blade so its original metal design survives.
 const pos=geo.attributes.position,uv=geo.attributes.uv;for(let i=0;i<pos.count;i++){const a=pix(pos.getX(i),pos.getY(i));uv.setXY(i,a.x/887,1-a.y/1774);}
 return add(geo,key,name);
}
solidShape(outline,[hole],.024,-.012,'blade','01 | pierced main blade');
// Separate silver lining down the physical inside edge makes the hole legible side-on.
const inner=hole.map(x=>p(...x,.012));inner.push(inner[0].clone());
tube(inner,.0024,'edge','02 | inner silver bevel',95,6);
// A polished outer ridge follows the right cutting edge only, not the decorative back.
tube(outline.slice(0,15).map(x=>p(...x,.012)),.0023,'edge','03 | outer cutting edge',90,6);

// Shaft follows the original concept's attachment line; circular from every angle.
const shaftBottom=p(204,1582),shaftTop=p(466,887);
const haft=cylinderBetween(shaftBottom,shaftTop,.021,.018,'blade','04 | round leather-wrapped haft',32);haft.userData.projected=true;
const shaftAxis=shaftTop.clone().sub(shaftBottom).normalize(), shaftLength=shaftTop.distanceTo(shaftBottom);
const side=new T.Vector3(shaftAxis.y,-shaftAxis.x,0).normalize(),front=new T.Vector3(0,0,1);
const helix=[];for(let i=0;i<=800;i++){const t=i/800,a=t*Math.PI*2*39;helix.push(shaftBottom.clone().addScaledVector(shaftAxis,t*shaftLength).addScaledVector(side,Math.cos(a)*.022).addScaledVector(front,Math.sin(a)*.022));}
tube(helix,.00135,'seam','05 | spiral leather binding',440,5);
function ringAt(t,width=.034,r=.027){const c=shaftBottom.clone().addScaledVector(shaftAxis,t*shaftLength);cylinderBetween(c.clone().addScaledVector(shaftAxis,-width/2),c.clone().addScaledVector(shaftAxis,width/2),r,r,'bronze','06 | bronze shaft collar');for(const s of [-1,1]){const pts=[];for(let k=0;k<=32;k++){const a=k/32*Math.PI*2;pts.push(c.clone().addScaledVector(shaftAxis,s*width*.46).addScaledVector(side,Math.cos(a)*r).addScaledVector(front,Math.sin(a)*r));}tube(pts,.002,'ivory','collar border',32,6);}}
for(const t of [.02,.16,.46,.77,.91,.985])ringAt(t);
// Butt blade: real beveled asymmetric metal piece with the source appearance.
solidShape([[216,1548],[242,1551],[243,1574],[229,1598],[211,1626],[191,1672],[159,1747],[181,1659],[184,1626],[181,1596],[198,1587],[207,1569]],[],.027,-.0135,'blade','07 | butt spike',.003);

// Raised fangs and crossed claw-slashes. These are relief meshes, not just paint.
const fangs=[[[557,642],[548,675],[552,694],[541,719],[550,705],[559,679],[572,650]],[[583,641],[592,664],[582,705],[552,752],[562,716],[575,681]]];
for(const z of [-.027,.016])for(let i=0;i<fangs.length;i++)solidShape(fangs[i],[],.008,z,'blade',`08 | raised fang crest ${i}`, .0012);
for(const [i,line] of [[[519,703],[583,676]],[[520,720],[582,695]],[[527,736],[576,714]]].entries()){
 const a=p(...line[0],.043),b=p(...line[1],.043),d=b.clone().sub(a).normalize(),s=new T.Vector3(-d.y,d.x,0).multiplyScalar(.003);
 const g=new T.BufferGeometry();const points=[a.clone().add(s),a.clone().sub(s),b.clone().add(s),b.clone().sub(s)];g.setAttribute('position',new T.Float32BufferAttribute(points.flatMap(v=>v.toArray()),3));g.setIndex([0,1,2,2,1,3]);add(g,'edge',`09 | crest claw slash ${i}`);
}
const diamondShape=[[568,663],[578,682],[550,726],[541,706]];solidShape(diamondShape,[],.007,.017,'blade','10 | dark blood-drop crest core',.0015);

// Chain: interlocking oval toroidal links with alternating planes.
const chainPath=curve([[655,195],[634,250],[601,345],[569,440],[542,526],[513,619],[490,704],[458,797],[425,888],[401,944]].map(([x,y])=>p(x,y,.047)));
const chainLen=chainPath.getLength(),count=Math.round(chainLen/.050);
for(let i=0;i<count;i++){
 const t=(i+.2)/(count-.6),c=chainPath.getPointAt(Math.min(t,1)),axis=chainPath.getTangentAt(Math.min(t,1));
 const crossSide=new T.Vector3(axis.y,-axis.x,0).normalize();const theta=i%2?Math.PI*.40:-Math.PI*.06;
 const across=crossSide.multiplyScalar(Math.cos(theta)).addScaledVector(front,Math.sin(theta));
 const pts=[];for(let j=0;j<=32;j++){const a=j/32*Math.PI*2;pts.push(c.clone().addScaledVector(axis,.031*Math.cos(a)).addScaledVector(across,.014*Math.sin(a)));}
 tube(pts,.0044,'chain',`11 | chain link ${String(i+1).padStart(2,'0')}`,32,8);
}
for(const point of [[655,195],[401,944]]){const c=p(...point,.036),pts=[];for(let i=0;i<=40;i++){const a=i/40*Math.PI*2;pts.push(c.clone().add(new T.Vector3(Math.cos(a)*.030,Math.sin(a)*.030,0)));}tube(pts,.005,'bronze','12 | chain anchor ring',40,8);}
cylinderBetween(p(657,195,.03),p(685,200,.018),.007,.009,'dark','12 | riveted upper chain eyelet',12);

// Dimensional beast-pelt tuft: tapered curved strand meshes, never a flat image card.
const furRoot=p(405,954,.025),furTip=p(290,1117,.027),flow=furTip.clone().sub(furRoot),fside=new T.Vector3(flow.y,-flow.x,0).normalize();
const furArrays=Array.from({length:5},()=>({p:[],n:[],c:[]}));
for(let h=0;h<500;h++){
 const spread=(rnd()-.5)*2,length=.36+rnd()*.9,zspread=(rnd()-.5)*.21;
 const start=furRoot.clone().addScaledVector(fside,(rnd()-.5)*.045).add(new T.Vector3(0,0,(rnd()-.5)*.085));
 const end=furRoot.clone().addScaledVector(flow,length).addScaledVector(fside,spread*(.11+rnd()*.07)).add(new T.Vector3(0,0,zspread));
 const bend=(rnd()-.5)*.048;let points=[];
 for(let j=0;j<=5;j++){const t=j/5;points.push(start.clone().lerp(end,t).addScaledVector(fside,Math.sin(t*Math.PI)*bend).add(new T.Vector3(0,0,Math.sin(t*Math.PI)*(.025+rnd()*.01))));}
 const width=.0012+rnd()*.0020,baseMat=rnd()<.18?2:rnd()<.60?1:0,bloody=rnd()<.68;
 for(let j=0;j<5;j++){
   const tangent=points[j+1].clone().sub(points[j]).normalize();const sw=new T.Vector3().crossVectors(tangent,new T.Vector3(.2,.1,1)).normalize();
   const w0=width*(1-j/5),w1=width*(1-(j+1)/5)*.4;
   const a=points[j].clone().addScaledVector(sw,w0),b=points[j].clone().addScaledVector(sw,-w0),c=points[j+1].clone().addScaledVector(sw,w1),d=points[j+1].clone().addScaledVector(sw,-w1);
   const mat=bloody&&j>=3?(rnd()<.75?3:4):baseMat;
   for(const q of [a,b,c,c,b,d])furArrays[mat].p.push(...q.toArray());
 }
}
furArrays.forEach((a,i)=>{const g=new T.BufferGeometry();g.setAttribute('position',new T.Float32BufferAttribute(a.p,3));g.computeVertexNormals();add(g,'fur'+i,`13 | dimensional fur strands ${i}`);});
// Leather binding around the fur root with a few bronze rivets.
cylinderBetween(furRoot.clone().addScaledVector(flow.clone().normalize(),-.024),furRoot.clone().addScaledVector(flow.clone().normalize(),.035),.038,.033,'leather','14 | fur leather binding',24);
for(let k=0;k<5;k++){const c=furRoot.clone().add(new T.Vector3((k-2)*.01,.013,.047));const mesh=add(new T.SphereGeometry(.004,8,6),'bronze','fur binding rivet');mesh.position.copy(c);}

// Bake transforms and export to a portable GLB with the concept texture embedded.
root.updateMatrixWorld(true);
function bakedGeometry(mesh){let g=mesh.geometry.clone();if(g.index)g=g.toNonIndexed();g.applyMatrix4(mesh.matrixWorld);if(mesh.userData.projected){const pos=g.attributes.position,u=new Float32Array(pos.count*2);for(let i=0;i<pos.count;i++){const q=pix(pos.getX(i),pos.getY(i));u[i*2]=q.x/887;u[i*2+1]=1-q.y/1774;}g.setAttribute('uv',new T.BufferAttribute(u,2));}return g;}
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
const gltf={asset:{version:'2.0',generator:'Bloodfang reference-guided geometry builder'},scene:0,scenes:[{nodes:nodes.map((_,i)=>i)}],nodes,meshes,buffers:[{byteLength:cursor}],bufferViews:views,accessors,
 materials:matkeys.map(k=>{const m=materials[k];return {name:m.name,doubleSided:true,pbrMetallicRoughness:{baseColorFactor:[...new T.Color(m.color).toArray(),1],metallicFactor:m.metal,roughnessFactor:m.rough,...(m.texture?{baseColorTexture:{index:0}}:{})}};}),textures:[{source:0,sampler:0}],samplers:[{magFilter:9729,minFilter:9987,wrapS:33071,wrapT:33071}],images:[{bufferView:iv,mimeType:'image/png'}]};
let json=Buffer.from(JSON.stringify(gltf)),jp=Buffer.alloc((4-json.length%4)%4,32),bin=Buffer.concat(blobs),bp=Buffer.alloc((4-bin.length%4)%4);json=Buffer.concat([json,jp]);bin=Buffer.concat([bin,bp]);
const head=Buffer.alloc(12);head.writeUInt32LE(0x46546c67);head.writeUInt32LE(2,4);head.writeUInt32LE(28+json.length+bin.length,8);
const jhead=Buffer.alloc(8);jhead.writeUInt32LE(json.length);jhead.write('JSON',4);const bhead=Buffer.alloc(8);bhead.writeUInt32LE(bin.length);bhead.write('BIN\0',4);
fs.writeFileSync(path.join(OUT,'Bloodfang_Openwork_Spear.glb'),Buffer.concat([head,jhead,json,bhead,bin]));
const info={name:'Bloodfang Openwork Spear',triangles:stats.reduce((a,b)=>a+b.triangles,0),mesh_objects:stats.length,chain_links:count,fur_strands:500,source:'blade_texture.png',construction:'New reference-guided 3D geometry. Pierced extruded blade with thickness and bevels, raised heraldry, toroidal chain links, circular haft and dimensional tapered fur strands.',surface:'Blade uses projected concept texture; some original lighting/detail is baked into this texture. Back uses mirrored front appearance. Other components have PBR materials.',not_in_game:false,parts:stats};
fs.writeFileSync(path.join(OUT,'model_info.json'),JSON.stringify(info,null,2));
// Readable editable OBJ companion with the same geometry and material assignments.
let obj=['mtllib Bloodfang_Openwork_Spear.mtl'];let idx=1;
for(const mesh of components){let g=bakedGeometry(mesh);const a=g.attributes.position,n=g.attributes.normal,u=g.attributes.uv;obj.push('o '+mesh.name.replaceAll(' ','_').replaceAll('|',''),'usemtl '+mesh.material.name);for(let i=0;i<a.count;i++)obj.push(`v ${a.getX(i)} ${a.getY(i)} ${a.getZ(i)}`);for(let i=0;i<a.count;i++)obj.push(`vt ${u?u.getX(i):0} ${u?u.getY(i):0}`);for(let i=0;i<a.count;i++)obj.push(`vn ${n.getX(i)} ${n.getY(i)} ${n.getZ(i)}`);for(let i=0;i<a.count;i+=3)obj.push('f '+[idx+i,idx+i+1,idx+i+2].map(j=>`${j}/${j}/${j}`).join(' '));idx+=a.count;}
fs.writeFileSync(path.join(OUT,'Bloodfang_Openwork_Spear.obj'),obj.join('\n'));
fs.writeFileSync(path.join(OUT,'Bloodfang_Openwork_Spear.mtl'),matkeys.map(k=>`newmtl ${k}\nKd ${new T.Color(materials[k].color).toArray().join(' ')}\nNs 50\n${materials[k].texture?'map_Kd concept_texture.png\n':''}`).join('\n'));
console.log(JSON.stringify({triangles:info.triangles,meshes:info.mesh_objects,chain_links:count,glb_bytes:28+json.length+bin.length}));
