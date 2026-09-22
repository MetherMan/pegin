import * as THREE from 'three';

// Native Matrix4 stores row vectors; transpose is implicit in fromArray.
// Quaternion::GetMatrix4 consequently requires the conjugate in Three.js.
function poseMatrix(p){return new THREE.Matrix4().compose(new THREE.Vector3(...p.slice(0,3)),new THREE.Quaternion(-p[3],-p[4],-p[5],p[6]),new THREE.Vector3(1,1,1))}

export class NativeActor {
  static async create(data,scene,facing,kind,target=false){
    const actor=new NativeActor(data,scene,facing,kind,target);await actor.load();return actor;
  }
  constructor(data,scene,facing,kind,target){
    this.data=data;this.person=data.people['0'];this.kind=kind;this.target=target;
    this.group=new THREE.Group();this.group.quaternion.copy(facing);scene.add(this.group);
    this.parts=[];this.weaponParts=[];this.cache=new Map();
  }
  async load(){
    const loader=new THREE.TextureLoader(),textures={};
    const needed=new Set([...this.person.parts,...(this.target?[]:this.data.weapons[this.kind].parts)].map(p=>p.texture));
    await Promise.all([...needed].map(async name=>{
      const t=await loader.loadAsync(this.data.textures[name]);t.flipY=false;t.colorSpace=THREE.SRGBColorSpace;t.needsUpdate=true;textures[name]=t;
    }));
    const add=(chunk,weapon)=>{
      const geometry=new THREE.BufferGeometry();
      geometry.setAttribute('position',new THREE.Float32BufferAttribute(chunk.corners.flatMap(c=>chunk.points[c[0]]),3));
      geometry.setAttribute('uv',new THREE.Float32BufferAttribute(chunk.corners.flatMap(c=>c.slice(4,6)),2));
      const material=new THREE.MeshBasicMaterial({map:textures[chunk.texture],side:THREE.DoubleSide,color:this.target?0xa6b3c3:0xffffff});
      const mesh=new THREE.Mesh(geometry,material);mesh.frustumCulled=false;this.group.add(mesh);
      const entry={chunk,mesh};(weapon?this.weaponParts:this.parts).push(entry);
      if(weapon)mesh.matrixAutoUpdate=false;
    };
    this.person.parts.forEach(c=>add(c,false));
    if(!this.target)this.data.weapons[this.kind].parts.forEach(c=>add(c,true));
    this.update(0);
  }
  frame(time){
    const clip=this.person[this.kind];
    if(this.target||time>=clip.duration){
      const idle=this.person.idle;return [idle,Math.floor(Math.max(0,time-clip.duration)%idle.duration/idle.duration*idle.frames)];
    }
    return [clip,Math.min(clip.frames-1,Math.floor(time/clip.duration*clip.frames))];
  }
  matrices(clip,index){
    let inverse=this.cache.get(clip);
    if(!inverse){inverse=clip.bones.map(b=>new THREE.Matrix4().fromArray(b.rest).invert());this.cache.set(clip,inverse)}
    const world=clip.bones.map(b=>poseMatrix(b.poses[index]));
    return {world,skin:world.map((m,i)=>m.clone().multiply(inverse[i]))};
  }
  bonePosition(time,name){
    const [clip,index]=this.frame(time),i=clip.bones.findIndex(b=>b.name===name);
    if(i<0)throw Error('원본 캐릭터 본 없음: '+name);
    return new THREE.Vector3(...clip.bones[i].poses[index].slice(0,3)).applyQuaternion(this.group.quaternion).add(this.group.position);
  }
  update(time){
    const [clip,index]=this.frame(time);if(this.lastClip===clip&&this.lastIndex===index)return;
    this.lastClip=clip;this.lastIndex=index;
    const {world,skin}=this.matrices(clip,index),point=new THREE.Vector3();
    for(const {chunk,mesh} of this.parts){
      if(!chunk.bones)continue;
      const positions=chunk.points.map((v,i)=>point.fromArray(v).applyMatrix4(skin[chunk.bones[i]]).toArray());
      const a=mesh.geometry.attributes.position;
      chunk.corners.forEach((c,i)=>a.setXYZ(i,...positions[c[0]]));a.needsUpdate=true;
    }
    if(!this.target){
      const bone=clip.bones.findIndex(b=>b.name===this.data.weapons[this.kind].bone);
      for(const {chunk,mesh} of this.weaponParts){mesh.matrix.copy(world[bone]).multiply(new THREE.Matrix4().fromArray(chunk.matrix));mesh.matrixWorldNeedsUpdate=true}
    }
  }
}

export class NativeSound {
  constructor(sources){this.sources=sources;this.buffers={};this.active=new Set()}
  async enable(){
    if(!this.context)this.context=new AudioContext();await this.context.resume();
    await Promise.all(Object.entries(this.sources).map(async([name,url])=>{
      if(!this.buffers[name])this.buffers[name]=await this.context.decodeAudioData(await (await fetch(url)).arrayBuffer());
    }));
  }
  stop(){for(const s of this.active){try{s.stop()}catch{}}this.active.clear()}
  play(name,rate){
    if(!this.context||!this.buffers[name])return;
    const s=this.context.createBufferSource(),gain=this.context.createGain();s.buffer=this.buffers[name];s.playbackRate.value=rate;
    gain.gain.value=.35;s.connect(gain);gain.connect(this.context.destination);s.onended=()=>this.active.delete(s);this.active.add(s);s.start();
  }
}
