import * as T from 'three';
import {OrbitControls} from 'controls';
import {installHornControls} from './horn-controls.js?v=1';
const scene=new T.Scene();scene.background=new T.Color('#282c32');
function viewport(){return innerWidth<=650?{w:innerWidth,h:Math.max(220,innerHeight-270)}:{w:innerWidth-250,h:Math.max(260,innerHeight-135)};}
const initial=viewport(),renderer=new T.WebGLRenderer({antialias:true,preserveDrawingBuffer:true});renderer.setPixelRatio(Math.min(devicePixelRatio,2));renderer.setSize(initial.w,initial.h);renderer.outputColorSpace=T.SRGBColorSpace;document.body.appendChild(renderer.domElement);
const camera=new T.PerspectiveCamera(34,initial.w/initial.h,.01,100),controls=new OrbitControls(camera,renderer.domElement);controls.enableDamping=true;controls.autoRotateSpeed=.55;
const stage=new T.Group();scene.add(stage);
// The native renderer shades the MOD corner normals. Keep the same surface
// information here; an unlit texture makes the dark horns look like cutouts.
scene.add(new T.AmbientLight(0xffffff,.8));
const keyLight=new T.DirectionalLight(0xffffff,2.8);keyLight.position.set(-3,7,-5);scene.add(keyLight);
const fillLight=new T.DirectionalLight(0xffffff,1.2);fillLight.position.set(4,3,2);scene.add(fillLight);
const floor=new T.Mesh(new T.CircleGeometry(10,100),new T.MeshBasicMaterial({color:0x24272c}));floor.rotation.x=-Math.PI/2;floor.position.y=-.03;scene.add(floor);
const data=await (await fetch('model.json?sculpt=satin-keratin-v5',{cache:'no-store'})).json(),loader=new T.TextureLoader(),textures={};
for(const [key,urls] of Object.entries(data.textures)){textures[key]=[];for(const url of urls){const t=await loader.loadAsync(url+'?v=satin-keratin-v5');t.flipY=false;t.colorSpace=T.SRGBColorSpace;textures[key].push(t);}}
try{await installHornControls(textures);}catch(error){document.querySelector('#horn-save-status').textContent=error.message;}
let entries=[],selected='primordial',motion='idle',paused=false,time=0;
const inverse={};for(const [name,clips] of Object.entries(data.clips)){inverse[name]={};for(const [key,clip] of Object.entries(clips))inverse[name][key]=clip.bones.map(b=>new T.Matrix4().fromArray(b.rest).invert());}
const point=new T.Vector3(),normal=new T.Vector3();
function pose(p){return new T.Matrix4().compose(new T.Vector3(...p.slice(0,3)),new T.Quaternion(-p[3],-p[4],-p[5],p[6]),new T.Vector3(1,1,1));}
function update(){
 const m=data.motions[motion],frame=m.first+Math.floor(time*30)%(m.last-m.first+1),matrices={};
 for(const key of ['original','primordial'])matrices[key]=data.clips[m.clip][key].bones.map((b,i)=>pose(b.poses[frame]).multiply(inverse[m.clip][key][i]));
 for(const {chunk,mesh,key} of entries){const positions=chunk.points.map((p,i)=>point.fromArray(p).applyMatrix4(matrices[key][chunk.bones[i]]).toArray()),a=mesh.geometry.attributes.position,n=mesh.geometry.attributes.normal;chunk.corners.forEach((c,i)=>{a.setXYZ(i,...positions[c[0]]);normal.set(c[1],c[2],c[3]).transformDirection(matrices[key][chunk.bones[c[0]]]);n.setXYZ(i,normal.x,normal.y,normal.z);});a.needsUpdate=true;n.needsUpdate=true;mesh.geometry.computeBoundingBox();}
 window.previewState={selected,motion,paused,frame,scale:1.7,meshes:entries.length};
 document.querySelector('#status').textContent=`${{idle:'대기',walk:'걷기',run:'달리기',attack:'공격'}[motion]} · ${paused?'일시정지':'재생 중'}`;
}
function load(key){
 for(const e of entries){stage.remove(e.group);e.mesh.geometry.dispose();e.mesh.material.dispose();}entries=[];selected=key;
 const keys=key==='compare'?['original','primordial']:[key];
 for(const kind of keys){const group=new T.Group();group.position.x=key==='compare'?(kind==='original'?2.0:-2.0):0;stage.add(group);
 data[kind].forEach((chunk,i)=>{const g=new T.BufferGeometry();g.setAttribute('position',new T.Float32BufferAttribute(chunk.corners.flatMap(c=>chunk.points[c[0]]),3));g.setAttribute('normal',new T.Float32BufferAttribute(chunk.corners.flatMap(c=>c.slice(1,4)),3));g.setAttribute('uv',new T.Float32BufferAttribute(chunk.corners.flatMap(c=>c.slice(4,6)),2));const mesh=new T.Mesh(g,new T.MeshLambertMaterial({map:textures[kind][i],side:T.FrontSide}));mesh.frustumCulled=false;group.add(mesh);entries.push({group,chunk,mesh,key:kind});});}
 for(const id of ['original','primordial','compare'])document.querySelector('#'+id).classList.toggle('active',key===id);
 document.body.classList.toggle('compare',key==='compare');update();view(key==='compare'?'front':'quarter');
}
function view(name){
 const box=new T.Box3().setFromObject(stage),center=box.getCenter(new T.Vector3()),size=box.getSize(new T.Vector3());
 const dist=Math.max(size.y,size.x/camera.aspect)/2/Math.tan(T.MathUtils.degToRad(17))*1.45;
 let target=center.clone(),direction=new T.Vector3(.7,.10,-1),distance=dist;
 if(name==='front')direction.set(0,.04,-1);
 if(name==='back')direction.set(0,.07,1);
 if(name==='detail'){target.y=box.min.y+size.y*.73;direction.set(.4,.02,-1);distance=dist*.52;}
 if(name==='feet'){target.y=box.min.y+size.y*.13;direction.set(.5,.25,-1);distance=dist*.40;}
 camera.position.copy(target).addScaledVector(direction.normalize(),distance);controls.target.copy(target);controls.update();
}
for(const key of ['original','primordial','compare'])document.querySelector('#'+key).onclick=()=>load(key);
for(const key of ['idle','walk','run','attack'])document.querySelector('#'+key).onclick=()=>{motion=key;time=0;for(const id of ['idle','walk','run','attack'])document.querySelector('#'+id).classList.toggle('active',id===key);update();};
for(const key of ['quarter','front','back','detail','feet'])document.querySelector('#'+key).onclick=()=>view(key);
document.querySelector('#pause').onclick=()=>{paused=!paused;document.querySelector('#pause').textContent=paused?'재생':'일시정지';update();};
document.querySelector('#rotate').onclick=()=>{controls.autoRotate=!controls.autoRotate;document.querySelector('#rotate').classList.toggle('active',controls.autoRotate);};
window.addEventListener('resize',()=>{const v=viewport();camera.aspect=v.w/v.h;camera.updateProjectionMatrix();renderer.setSize(v.w,v.h);});
load(selected);window.viewerReady=true;const clock=new T.Clock();renderer.setAnimationLoop(()=>{const dt=clock.getDelta();if(!paused){time+=dt;update();}controls.update();renderer.render(scene,camera);});
