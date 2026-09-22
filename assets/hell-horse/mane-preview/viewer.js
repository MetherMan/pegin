import * as T from 'three';
import {OrbitControls} from 'controls';
const scene=new T.Scene();scene.background=new T.Color('#161313');
const renderer=new T.WebGLRenderer({antialias:true,preserveDrawingBuffer:true});renderer.setPixelRatio(Math.min(devicePixelRatio,2));renderer.setSize(innerWidth,innerHeight);renderer.outputColorSpace=T.SRGBColorSpace;document.body.appendChild(renderer.domElement);
const camera=new T.PerspectiveCamera(34,innerWidth/innerHeight,.01,100),controls=new OrbitControls(camera,renderer.domElement);controls.enableDamping=true;
const group=new T.Group();scene.add(group);
const floor=new T.Mesh(new T.CircleGeometry(3.3,80),new T.MeshBasicMaterial({color:0x231e1a}));floor.rotation.x=-Math.PI/2;floor.position.y=-.015;scene.add(floor);
const data=await (await fetch('../model.json')).json(),loader=new T.TextureLoader(),textures={};
for(const [key,url] of Object.entries(data.textures)){const t=await loader.loadAsync(key==='hellhorse'?'before-atlas.png':'../'+url);t.flipY=false;t.colorSpace=T.SRGBColorSpace;textures[key]=t;}
textures.muted=await loader.loadAsync('../hellhorse-atlas.png?v=muted-mane-tail');textures.muted.flipY=false;textures.muted.colorSpace=T.SRGBColorSpace;
let entries=[],selected='muted',motion='idle',paused=false,time=0;
function pose(p){return new T.Matrix4().compose(new T.Vector3(...p.slice(0,3)),new T.Quaternion(-p[3],-p[4],-p[5],p[6]),new T.Vector3(1,1,1));}
const inverse=data.animation.bones.map(b=>new T.Matrix4().fromArray(b.rest).invert()),point=new T.Vector3();
function update(){
 const [first,last]=data.motions[motion],index=first+Math.floor(time*30)%(last-first+1);
 const matrices=data.animation.bones.map((b,i)=>pose(b.poses[index]).multiply(inverse[i]));
 for(const {chunk,mesh} of entries){const positions=chunk.points.map((p,i)=>point.fromArray(p).applyMatrix4(matrices[chunk.bones[i]]).toArray()),a=mesh.geometry.attributes.position;chunk.corners.forEach((c,i)=>a.setXYZ(i,...positions[c[0]]));a.needsUpdate=true;}
 window.previewState={selected,motion,paused,frame:index};
}
function load(key){
 for(const {mesh} of entries){group.remove(mesh);mesh.geometry.dispose();mesh.material.dispose();}entries=[];selected=key;
 for(const chunk of data[key==='muted'?'hellhorse':key]){const g=new T.BufferGeometry();g.setAttribute('position',new T.Float32BufferAttribute(chunk.corners.flatMap(c=>chunk.points[c[0]]),3));g.setAttribute('uv',new T.Float32BufferAttribute(chunk.corners.flatMap(c=>c.slice(4,6)),2));const mesh=new T.Mesh(g,new T.MeshBasicMaterial({map:textures[key],side:T.DoubleSide}));mesh.frustumCulled=false;group.add(mesh);entries.push({chunk,mesh});}
 document.querySelector('#hell').classList.toggle('active',key==='hellhorse');document.querySelector('#muted').classList.toggle('active',key==='muted');update();
}
function view(name){
 for(const {mesh} of entries)mesh.geometry.computeBoundingBox();
 const box=new T.Box3().setFromObject(group),center=box.getCenter(new T.Vector3()),size=box.getSize(new T.Vector3());
 const dist=Math.max(size.y,size.z/camera.aspect)/2/Math.tan(T.MathUtils.degToRad(17))*1.35;
 let target=center.clone(),direction=new T.Vector3(1,.17,-.9),distance=dist;
 if(name==='side')direction.set(1,.10,0);
 if(name==='front')direction.set(.015,.06,-1);
 if(name==='eyes'){target.set(center.x,box.max.y-.30,box.min.z+.25);direction.set(1,.12,-.55);distance=1.20;}
 if(name==='feet'){target.set(center.x,box.min.y+.24,box.min.z+.70);direction.set(1,.36,-.8);distance=1.85;}
 camera.position.copy(target).addScaledVector(direction.normalize(),distance);controls.target.copy(target);controls.update();
}
document.querySelector('#hell').onclick=()=>load('hellhorse');document.querySelector('#muted').onclick=()=>load('muted');
for(const key of ['idle','run'])document.querySelector('#'+key).onclick=()=>{motion=key;time=0;document.querySelector('#idle').classList.toggle('active',key==='idle');document.querySelector('#run').classList.toggle('active',key==='run');update();};
for(const key of ['quarter','side','front','eyes','feet'])document.querySelector('#'+key).onclick=()=>view(key);
document.querySelector('#pause').onclick=()=>{paused=!paused;document.querySelector('#pause').textContent=paused?'재생':'일시정지';};
window.addEventListener('resize',()=>{camera.aspect=innerWidth/innerHeight;camera.updateProjectionMatrix();renderer.setSize(innerWidth,innerHeight);});
load(selected);view('side');document.querySelector('#status').textContent='718 삼각형 · 원본 32본 · 원본 대기 / 달리기';
window.viewerReady=true;const clock=new T.Clock();renderer.setAnimationLoop(()=>{const dt=clock.getDelta();if(!paused){time+=dt;update();}controls.update();renderer.render(scene,camera);});
