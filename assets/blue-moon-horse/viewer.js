import * as T from 'three';
import {OrbitControls} from 'controls';
const scene=new T.Scene();scene.background=new T.Color('#0d1322');
const renderer=new T.WebGLRenderer({antialias:true,preserveDrawingBuffer:true});renderer.setPixelRatio(Math.min(devicePixelRatio,2));renderer.setSize(innerWidth,innerHeight);renderer.outputColorSpace=T.SRGBColorSpace;document.body.appendChild(renderer.domElement);
const camera=new T.PerspectiveCamera(34,innerWidth/innerHeight,.01,100),controls=new OrbitControls(camera,renderer.domElement);controls.enableDamping=true;
const group=new T.Group();scene.add(group);
const floor=new T.Mesh(new T.CircleGeometry(3.3,80),new T.MeshBasicMaterial({color:0x161f33}));floor.rotation.x=-Math.PI/2;floor.position.y=-.015;scene.add(floor);
const data=await (await fetch('model.json')).json(),loader=new T.TextureLoader(),textures={};
async function texture(key){if(!textures[key]){const t=await loader.loadAsync(data.textures[key]);t.flipY=false;t.colorSpace=T.SRGBColorSpace;textures[key]=t;}return textures[key];}
let entries=[],coat='midnight',saddle='original',motion='idle',paused=false,time=0;
function pose(p){return new T.Matrix4().compose(new T.Vector3(...p.slice(0,3)),new T.Quaternion(-p[3],-p[4],-p[5],p[6]),new T.Vector3(1,1,1));}
const inverse=data.animation.bones.map(b=>new T.Matrix4().fromArray(b.rest).invert()),point=new T.Vector3();
function update(){
 const [first,last]=data.motions[motion],index=first+Math.floor(time*30)%(last-first+1);
 const matrices=data.animation.bones.map((b,i)=>pose(b.poses[index]).multiply(inverse[i]));
 for(const {chunk,mesh} of entries){const positions=chunk.points.map((p,i)=>point.fromArray(p).applyMatrix4(matrices[chunk.bones[i]]).toArray()),a=mesh.geometry.attributes.position;chunk.corners.forEach((c,i)=>a.setXYZ(i,...positions[c[0]]));a.needsUpdate=true;}
 window.previewState={coat,saddle,motion,paused,frame:index};
}
async function load(){
 const key=coat==='original'?'original':`${coat}-${saddle}`,map=await texture(key),chunks=data[coat==='original'?'original':'bluemoon'];
 for(const {mesh} of entries){group.remove(mesh);mesh.geometry.dispose();mesh.material.dispose();}entries=[];
 for(const chunk of chunks){const g=new T.BufferGeometry();g.setAttribute('position',new T.Float32BufferAttribute(chunk.corners.flatMap(c=>chunk.points[c[0]]),3));g.setAttribute('uv',new T.Float32BufferAttribute(chunk.corners.flatMap(c=>c.slice(4,6)),2));const mesh=new T.Mesh(g,new T.MeshBasicMaterial({map,side:T.DoubleSide}));mesh.frustumCulled=false;group.add(mesh);entries.push({chunk,mesh});}
 document.querySelectorAll('[data-coat]').forEach(b=>b.classList.toggle('active',b.dataset.coat===coat));
 document.querySelectorAll('[data-saddle]').forEach(b=>{b.classList.toggle('active',b.dataset.saddle===saddle);b.disabled=coat==='original';});
 update();
}
function view(name){
 const box=new T.Box3().setFromObject(group),center=box.getCenter(new T.Vector3()),size=box.getSize(new T.Vector3());
 const dist=Math.max(size.y,size.z/camera.aspect)/2/Math.tan(T.MathUtils.degToRad(17))*1.35;
 let target=center.clone(),direction=new T.Vector3(1,.17,-.9),distance=dist;
 if(name==='side')direction.set(1,.10,0);
 if(name==='front')direction.set(.015,.06,-1);
 if(name==='horn'){target.set(center.x,box.max.y-.30,box.min.z+.26);direction.set(1,.25,-.75);distance=1.6;}
 if(name==='eyes'){target.set(center.x,box.max.y-.42,box.min.z+.32);direction.set(1,.08,-.35);distance=.95;}
 if(name==='mane'){target.set(center.x,box.max.y-.36,box.min.z+.62);direction.set(1,.35,-.15);distance=1.9;}
 camera.position.copy(target).addScaledVector(direction.normalize(),distance);controls.target.copy(target);controls.update();
}
document.querySelectorAll('[data-coat]').forEach(b=>b.onclick=()=>{coat=b.dataset.coat;load();});
document.querySelectorAll('[data-saddle]').forEach(b=>b.onclick=()=>{saddle=b.dataset.saddle;load();});
document.querySelectorAll('[data-view]').forEach(b=>b.onclick=()=>{viewName=b.dataset.view;view(viewName);});
for(const key of ['idle','run'])document.querySelector('#'+key).onclick=()=>{motion=key;time=0;document.querySelector('#idle').classList.toggle('active',key==='idle');document.querySelector('#run').classList.toggle('active',key==='run');update();};
document.querySelector('#pause').onclick=()=>{paused=!paused;document.querySelector('#pause').textContent=paused?'재생':'일시정지';};
let viewName='quarter',shown=[0,0];
function fit(){const w=innerWidth,h=innerHeight;if(!w||!h||(w===shown[0]&&h===shown[1]))return;shown=[w,h];camera.aspect=w/h;camera.updateProjectionMatrix();renderer.setSize(w,h);view(viewName);}
await load();fit();
const horn=data.bluemoon[0].points.length-data.original[0].points.length;
document.querySelector('#status').textContent=`원본 32본 · 뿔·털 ${horn}정점 추가 · 원본 대기 / 달리기`;
window.viewer={camera,controls,group,scene,renderer};window.viewerReady=true;const clock=new T.Clock();renderer.setAnimationLoop(()=>{fit();const dt=clock.getDelta();if(!paused){time+=dt;update();}controls.update();renderer.render(scene,camera);});
