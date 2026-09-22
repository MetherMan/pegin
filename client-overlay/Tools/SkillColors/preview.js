import * as THREE from 'three';
import {OrbitControls} from './vendor/OrbitControls.js';
import {NativeActor,NativeSound} from './actor.js';

const vertexShader=`varying vec2 texUV;
uniform vec4 atlas;
void main(){texUV=uv*atlas.xy+atlas.zw;gl_Position=projectionMatrix*modelViewMatrix*vec4(position,1.0);}`;
const fragmentShader=`uniform sampler2D sourceTexture;
uniform vec3 tint;
varying vec2 texUV;
void main(){gl_FragColor=vec4(texture2D(sourceTexture,texUV).rgb*tint,1.0);}`;
const up=new THREE.Vector3(0,1,0);
const labels={meteor:'메테오',frost:'얼음',heaven:'헤븐',six:'식스폴드'};

// CMagicEffect::RelativeCoordinates: script X points to the caster's left.
function scriptPosition(p=[0,0,0]){return new THREE.Vector3(-p[0],p[2],-p[1])}

function nativeMatrix(frame){
  const sx=frame[9],sy=frame[8],sz=frame[10],homogeneous=frame[7];
  const axis=new THREE.Vector3(frame[3],frame[4],frame[5]),rotation=new THREE.Matrix4();
  if(axis.lengthSq()>0)rotation.makeRotationAxis(axis.normalize(),-frame[6]*Math.PI/180);
  const matrix=new THREE.Matrix4().makeScale(sx/homogeneous,sy/homogeneous,sz/homogeneous).multiply(rotation);
  matrix.setPosition(frame[0]*sx,frame[1]*sy,frame[2]*sz);
  return matrix;
}

export class SkillPreview{
  static async create(card,assets,kind,color){
    const preview=new SkillPreview(card,assets,kind,color);
    await preview.load();
    const motion=['heaven','six'].includes(kind)?'arrow':'magic';
    preview.actor=await NativeActor.create(assets.actors,preview.scene,preview.facing,motion);
    preview.victim=await NativeActor.create(assets.actors,preview.scene,new THREE.Quaternion().setFromAxisAngle(up,Math.PI).multiply(preview.facing),motion,true);
    preview.victim.group.position.copy(preview.targetAnchor);
    preview.sound=new NativeSound(assets.sounds);preview.audible=false;
    preview.closeCamera=new THREE.PerspectiveCamera(32,1,.05,80);
    preview.closeCamera.position.copy(new THREE.Vector3(2.3,1.9,3.1).applyQuaternion(preview.facing));
    preview.closeCamera.lookAt(0,1,0);
    const insetLabel=document.createElement('span');insetLabel.className='inset-label';insetLabel.textContent='모션 확대';card.querySelector('.viewport').append(insetLabel);
    preview.setPhase('B');
    preview.bindControls();
    preview.render();
    preview.renderer.setAnimationLoop(ms=>preview.tick(ms));
    return preview;
  }
  constructor(card,assets,kind,color){
    this.card=card;this.assets=assets;this.kind=kind;this.color=color;
    this.time=0;this.playing=true;this.speed=Number(card.querySelector('.speed').value);this.lastTime=null;this.lastSoundTime=-1;
    this.targetAnchor=['heaven','six'].includes(kind)?new THREE.Vector3(5,0,-5):new THREE.Vector3(0,0,-7);
    this.facing=new THREE.Quaternion().setFromRotationMatrix(new THREE.Matrix4().lookAt(new THREE.Vector3(),this.targetAnchor,up));
    this.canvas=card.querySelector('canvas');
    this.renderer=new THREE.WebGLRenderer({canvas:this.canvas,antialias:true,alpha:false});
    this.renderer.setPixelRatio(Math.min(devicePixelRatio,2));
    this.renderer.setClearColor(0x0b121c,1);
    this.renderer.toneMapping=THREE.NoToneMapping;
    this.scene=new THREE.Scene();
    this.camera=new THREE.PerspectiveCamera(36,1,.05,120);
    this.orbit=new OrbitControls(this.camera,this.canvas);
    this.orbit.enableDamping=true;this.orbit.dampingFactor=.09;
    this.orbit.minDistance=3;this.orbit.maxDistance=70;
    this.orbit.maxPolarAngle=Math.PI*.485;
    this.group=new THREE.Group();this.scene.add(this.group);
    this.geometries={};this.textures={};this.timeline=[];
    const grid=new THREE.GridHelper(30,30,0x2a3d4c,0x1a2a38);
    grid.position.set(0,-.05,-3);this.scene.add(grid);
    const ground=new THREE.Mesh(new THREE.PlaneGeometry(60,60),new THREE.MeshBasicMaterial({color:0x101a25}));
    ground.rotation.x=-Math.PI/2;ground.position.y=-.055;this.scene.add(ground);
    this.addMarker(0,'시전자',0x9aaebe);this.targetMarker=this.addMarker(-7,'대상',0x738896);
    this.targetMarker.position.copy(this.targetAnchor);
    this.resizeObserver=new ResizeObserver(()=>this.resize());this.resizeObserver.observe(this.canvas);
    this.canvas.addEventListener('webglcontextlost',event=>{
      event.preventDefault();this.playing=false;
      const message=this.card.querySelector('.loading');message.hidden=false;message.textContent='3D 연결이 끊겼습니다. 이 설정 창을 새로 고침해 주세요.';
    });
  }
  addMarker(z,text,color){
    const group=new THREE.Group();group.position.z=z;
    const ring=new THREE.Mesh(new THREE.RingGeometry(.28,.3,48),new THREE.MeshBasicMaterial({color,side:THREE.DoubleSide}));
    ring.rotation.x=-Math.PI/2;ring.position.y=.005;group.add(ring);
    const image=document.createElement('canvas');image.width=256;image.height=64;
    const context=image.getContext('2d');context.font='27px system-ui';context.textAlign='center';context.fillStyle='#94aabc';context.fillText(text,128,41);
    const label=new THREE.Sprite(new THREE.SpriteMaterial({map:new THREE.CanvasTexture(image),transparent:true,depthTest:false}));
    label.position.set(0,.15,.7);label.scale.set(1.6,.4,1);group.add(label);
    this.scene.add(group);return group;
  }
  async load(){
    for(const [name,data] of Object.entries(this.assets.meshes)){
      const geometry=new THREE.BufferGeometry();
      geometry.setAttribute('position',new THREE.Float32BufferAttribute(data.vertices.flatMap(v=>v.slice(0,3)),3));
      geometry.setAttribute('uv',new THREE.Float32BufferAttribute(data.vertices.flatMap(v=>v.slice(3,5)),2));
      geometry.setIndex(data.indices);this.geometries[name]=geometry;
    }
    const loader=new THREE.TextureLoader();
    await Promise.all(Object.entries(this.assets.textures).map(async([name,url])=>{
      const texture=await loader.loadAsync(url);
      // D3D's V=0 is the top bitmap row. Preserve native gamma-space RGB.
      texture.flipY=false;texture.colorSpace=THREE.NoColorSpace;
      texture.wrapS=texture.wrapT=THREE.RepeatWrapping;
      texture.magFilter=THREE.LinearFilter;texture.minFilter=THREE.LinearMipmapLinearFilter;
      texture.needsUpdate=true;this.textures[name]=texture;
    }));
  }
  makeMesh(name){
    const effect=this.assets.effects[name];
    if(!effect)throw Error('이펙트 원본을 찾지 못했습니다: '+name);
    const material=new THREE.ShaderMaterial({
      uniforms:{sourceTexture:{value:this.textures[effect.texture]},tint:{value:new THREE.Vector3(1,1,1)},atlas:{value:new THREE.Vector4(1,1,0,0)}},
      vertexShader,fragmentShader,transparent:true,depthTest:true,depthWrite:false,side:THREE.FrontSide,
      blending:THREE.CustomBlending,blendEquation:THREE.AddEquation,blendSrc:THREE.OneFactor,blendDst:THREE.OneFactor,toneMapped:false
    });
    const mesh=new THREE.Mesh(this.geometries[effect.mesh],material);mesh.matrixAutoUpdate=false;mesh.frustumCulled=false;
    this.group.add(mesh);return {mesh,effect};
  }
  makeTrace(name,width){
    const source=this.assets.traces[name],count=source.level+1,geometry=new THREE.BufferGeometry();
    geometry.setAttribute('position',new THREE.BufferAttribute(new Float32Array(count*6),3));
    geometry.setAttribute('uv',new THREE.BufferAttribute(new Float32Array(count*4),2));
    geometry.setAttribute('brightness',new THREE.BufferAttribute(new Float32Array(count*2),1));
    const indices=[];for(let i=0;i<count-1;i++){const a=i*2;indices.push(a,a+1,a+2,a+1,a+3,a+2)}
    geometry.setIndex(indices);geometry.setDrawRange(0,0);
    const texture=this.textures[source.texture].clone();texture.wrapS=texture.wrapT=THREE.ClampToEdgeWrapping;texture.needsUpdate=true;
    const material=new THREE.ShaderMaterial({
      uniforms:{sourceTexture:{value:texture}},
      vertexShader:'attribute float brightness; varying vec2 texUV; varying float lightness; void main(){texUV=uv;lightness=brightness;gl_Position=projectionMatrix*modelViewMatrix*vec4(position,1.0);}',
      fragmentShader:'uniform sampler2D sourceTexture; varying vec2 texUV; varying float lightness; void main(){gl_FragColor=vec4(texture2D(sourceTexture,texUV).rgb*lightness,1.0);}',
      transparent:true,depthWrite:false,side:THREE.DoubleSide,blending:THREE.CustomBlending,
      blendEquation:THREE.AddEquation,blendSrc:THREE.OneFactor,blendDst:THREE.OneFactor,toneMapped:false
    });
    const mesh=new THREE.Mesh(geometry,material);mesh.frustumCulled=false;mesh.userData.dynamic=true;this.group.add(mesh);
    return {mesh,source,width};
  }
  updateTrace(entry){
    const trace=entry.trail;if(!trace)return;
    const elapsed=this.time-entry.start,duration=entry.end-entry.start;
    trace.mesh.visible=elapsed>0&&elapsed<duration+500;
    if(!trace.mesh.visible)return;
    const step=1000/this.assets.fps,last=Math.floor(elapsed/step),first=Math.max(0,last-trace.source.level),count=last-first+1;
    const geometry=trace.mesh.geometry,positions=geometry.attributes.position,uv=geometry.attributes.uv,brightness=geometry.attributes.brightness;
    for(let i=0;i<count;i++){
      const age=(first+i)*step,point=entry.from.clone().lerp(entry.to,Math.min(1,age/duration));
      // Native EtcEffect inserts the two ribbon edges along global game Y.
      const width=age<duration?trace.width:.25;
      positions.setXYZ(i*2,point.x,point.y,point.z-width);positions.setXYZ(i*2+1,point.x,point.y,point.z+width);
      const u=1-i/Math.max(1,count-1),light=(255-Math.floor(255/count)*(count-1-i))/255;
      uv.setXY(i*2,u,0);uv.setXY(i*2+1,u,1);brightness.setX(i*2,light);brightness.setX(i*2+1,light);
    }
    positions.needsUpdate=uv.needsUpdate=brightness.needsUpdate=true;geometry.setDrawRange(0,Math.max(0,count-1)*6);
  }
  setPhase(phase){
    this.phase=phase;
    this.sound?.stop();this.lastSoundTime=-1;this.soundEvents=[];
    for(const child of [...this.group.children]){if(child.userData.dynamic){child.geometry.dispose();child.material.uniforms.sourceTexture.value.dispose()}child.material.dispose();this.group.remove(child)}
    this.timeline=[];
    for(const effect of this.assets.scripts[this.kind][phase]){
      let elapsed=0,position=new THREE.Vector3(),primaryAge=0,previousBone=null;
      for(const part of effect.parts){
        elapsed+=part.STARTTIME||0;
        // Native ENEMY anchors the impact at the target, independently of
        // the preceding projectile's body-height endpoint.
        if(part.ENEMY)position=scriptPosition(part.TARGET).applyQuaternion(this.facing).add(this.targetAnchor);
        else if(previousBone===14)position=this.actor.bonePosition(elapsed,'Bip01 R Hand');
        else if(part.POS)position=scriptPosition(part.POS).applyQuaternion(this.facing);
        const start=position.clone();let end=start.clone(),duration=part.TIMELIMIT||0;
        if(!duration&&part.ATTACK){
          const target=part.TARGET||[0,0,0];
          end=new THREE.Vector3(target[0],target[2],target[1]).applyQuaternion(this.facing).add(this.targetAnchor);
          // Original Tracking moves SPEED / 1000 units on each engine update.
          duration=Math.ceil(start.distanceTo(end)/((part.SPEED||1000)/1000))*1000/this.assets.fps;
        }
        if(!duration)duration=1000/this.assets.fps;
        if(part.SOUND)this.soundEvents.push({time:elapsed+part.SOUND.delay,name:part.SOUND.name});
        if(part.HITSOUND)this.soundEvents.push({time:elapsed,name:part.HITSOUND});
        const moving=!!part.ATTACK,loop=effect.loop||part.LOOP;
        const orientation=this.facing.clone();
        if(moving)orientation.setFromRotationMatrix(new THREE.Matrix4().lookAt(start,end,up));
        const add=(name,age,primary=false)=>{
          const entry={...this.makeMesh(name),start:elapsed,end:elapsed+duration,from:start.clone(),to:end.clone(),orientation,moving,loop,pause:part.PAUSE,age};
          if(primary&&moving&&effect.trace)entry.trail=this.makeTrace(effect.trace,part.TRACEDIST??.25);
          this.timeline.push(entry);
        };
        if(effect.wed&&!part.NOSHOW)add(effect.wed,primaryAge,true);
        if(part.TAIL)add(part.TAIL,0);
        primaryAge=part.NOSHOW?0:primaryAge+duration;
        elapsed+=duration;position=end;previousBone=part.BONE??null;
      }
    }
    this.duration=Math.max(...this.timeline.map(e=>e.end),this.assets.timing[this.kind].motionEnd,...this.soundEvents.map(e=>e.time+(this.assets.soundDurations[e.name]||0)))+600;
    this.time=0;this.card.querySelector('.seek').max=Math.ceil(this.duration);
    this.targetMarker.visible=phase!=='A';
    if(!this.hasView){this.fitView();this.hasView=true}this.resize();
    const timing=this.assets.timing[this.kind];
    this.card.querySelector('.timing-note').textContent=`모션 ${(timing.motionEnd/1000).toFixed(2)}초 · 첫 발 ${(timing.first/1000).toFixed(2)}초 · 마지막 ${(timing.last/1000).toFixed(2)}초`+(timing.last>timing.motionEnd?' · 마지막 발이 모션 뒤에 나갑니다':'');
    if(this.kind==='frost')this.card.querySelector('.timing-note').textContent+=` · 좌우 ${timing.spreadAngle}도`;
    if(this.kind==='meteor')this.card.querySelector('.timing-note').textContent+=` · 불꽃 높이 ${timing.impactHeight.toFixed(2)}`;
  }
  applyAssets(partial){
    const pausedTime=this.playing?null:this.time;
    this.assets={...this.assets,...partial,meshes:this.assets.meshes,textures:this.assets.textures,actors:this.assets.actors,sounds:this.assets.sounds,soundDurations:this.assets.soundDurations};
    this.setPhase(this.phase);
    if(pausedTime!==null)this.time=Math.min(pausedTime,this.duration);
    this.setPlaying(pausedTime===null);this.render();
  }
  fitView(){
    // Fit actual animated vertices, including the expanding ground-wave mesh.
    const bounds=new THREE.Box3();
    bounds.expandByPoint(new THREE.Vector3(-.8,0,.8));bounds.expandByPoint(new THREE.Vector3(.8,2,0));
    bounds.expandByPoint(this.targetAnchor.clone().add(new THREE.Vector3(0,2,0)));
    for(const entry of this.timeline){
      const geometry=this.assets.meshes[entry.effect.mesh];
      for(const frame of entry.effect.frames){
        if(!frame[7]||Math.max((frame[11]>>16)&255,(frame[11]>>8)&255,frame[11]&255)<40)continue;
        const local=nativeMatrix(frame);
        for(const origin of [entry.from,entry.to]){
          const world=new THREE.Matrix4().compose(origin,entry.orientation,new THREE.Vector3(1,1,1)).multiply(local);
          for(const vertex of geometry.vertices){
            const point=new THREE.Vector3(...vertex.slice(0,3)).applyMatrix4(world);
            if(point.y>=0)bounds.expandByPoint(point);
          }
        }
      }
    }
    if(bounds.isEmpty())return;
    const center=bounds.getCenter(new THREE.Vector3());
    const direction=new THREE.Vector3(.75,.75,1).normalize();
    const right=new THREE.Vector3().crossVectors(up,direction).normalize();
    const cameraUp=new THREE.Vector3().crossVectors(direction,right).normalize();
    const aspect=this.canvas.clientWidth/this.canvas.clientHeight,tangent=Math.tan(this.camera.fov*Math.PI/360);
    let distance=4;
    for(const x of [bounds.min.x,bounds.max.x])for(const y of [bounds.min.y,bounds.max.y])for(const z of [bounds.min.z,bounds.max.z]){
      const point=new THREE.Vector3(x,y,z).sub(center),depth=point.dot(direction);
      distance=Math.max(distance,depth+Math.abs(point.dot(cameraUp))/tangent,depth+Math.abs(point.dot(right))/(tangent*aspect));
    }
    this.orbit.target.copy(center);this.camera.position.copy(center).addScaledVector(direction,distance*1.08);
    this.orbit.update();
  }
  setColor(color){this.color=color;this.render()}
  nativeFrame(entry,time){
    const effect=entry.effect;let frame=Math.floor(time*this.assets.fps/1000);
    if(frame>=effect.end){if(entry.loop)frame%=effect.end;else if(entry.pause)frame=effect.end-1;else return null}
    // Match CW3DSpecialEffect::CheckCount's final sample and start clamp.
    if(frame===effect.end-1)frame=effect.end;
    if(time*this.assets.fps/1000<=effect.start)frame=effect.start;
    return effect.frames[frame];
  }
  updateEntry(entry){
    this.updateTrace(entry);
    const age=this.time-entry.start;
    entry.mesh.visible=age>=0&&this.time<entry.end;
    if(!entry.mesh.visible)return;
    const frame=this.nativeFrame(entry,age+entry.age);
    if(!frame){entry.mesh.visible=false;return}
    const homogeneous=frame[7];
    if(!homogeneous){entry.mesh.visible=false;return}
    // Native row-vector matAnimation * matScale, including unusual _44=fScale.
    // Divide XYZ by _44 after projection; translation is scaled before world placement.
    const local=nativeMatrix(frame);
    const position=entry.from.clone().lerp(entry.to,age/(entry.end-entry.start));
    const world=new THREE.Matrix4().compose(position,entry.orientation,new THREE.Vector3(1,1,1));
    entry.mesh.matrix.multiplyMatrices(world,local);entry.mesh.matrixWorldNeedsUpdate=true;
    const luminosity=frame[11]&255;
    const rgb=entry.effect.tintable
      ?[1,3,5].map(i=>Math.round(parseInt(this.color.slice(i,i+2),16)*luminosity/255)/255)
      :[16,8,0].map(shift=>((frame[11]>>>shift)&255)/255);
    entry.mesh.material.uniforms.tint.value.set(...rgb);
    const id=frame[12]-1,cols=entry.effect.cols,rows=entry.effect.rows;
    entry.mesh.material.uniforms.atlas.value.set(id<0?1:1/cols,id<0?1:1/rows,id<0?0:(id%cols)/cols,id<0?0:Math.floor(id/cols)/rows);
  }
  setPlaying(value){
    if(!value)this.sound?.stop();
    this.playing=value;const button=this.card.querySelector('.play');button.textContent=value?'일시정지':'재생';
    button.setAttribute('aria-label',labels[this.kind]+' '+(value?'일시정지':'재생'));
  }
  bindControls(){
    this.card.querySelector('.play').onclick=()=>this.setPlaying(!this.playing);
    this.card.querySelector('.restart').onclick=()=>{this.sound.stop();this.time=0;this.lastSoundTime=-1;this.setPlaying(true)};
    this.card.querySelector('.phase').onchange=event=>{this.setPhase(event.target.value);this.render()};
    this.card.querySelector('.speed').onchange=event=>{this.sound.stop();this.speed=Number(event.target.value)};
    this.card.querySelector('.seek').oninput=event=>{this.setPlaying(false);this.time=Number(event.target.value);this.lastSoundTime=this.time;this.render()};
    this.card.querySelector('.sound').onclick=async()=>{
      try{
        if(!this.audible){await this.sound.enable();window.dispatchEvent(new CustomEvent('skill-audio',{detail:this.kind}))}
        this.audible=!this.audible;this.sound.stop();this.card.querySelector('.sound').textContent=this.audible?'소리 끄기':'소리 켜기';
        if(this.audible){this.time=0;this.lastSoundTime=-1;this.setPlaying(true)}
      }catch(error){this.card.querySelector('.timing-note').textContent='사운드 로드 실패: '+error.message}
    };
    window.addEventListener('skill-audio',event=>{if(event.detail!==this.kind){this.audible=false;this.sound.stop();this.card.querySelector('.sound').textContent='소리 켜기'}});
    this.card.querySelector('.camera').onclick=()=>{this.fitView();this.render()};
  }
  resize(){
    const {width,height}=this.canvas.getBoundingClientRect();
    if(!width||!height)return;
    this.renderer.setSize(width,height,false);this.camera.aspect=width/height;this.camera.updateProjectionMatrix();
    this.render();
  }
  render(){
    this.actor?.update(this.time);this.victim?.update(this.time);
    for(const entry of this.timeline)this.updateEntry(entry);
    this.renderer.render(this.scene,this.camera);
    if(this.closeCamera){
      const width=this.canvas.clientWidth,height=this.canvas.clientHeight,w=Math.min(155,Math.floor(width*.36)),h=180;
      this.closeCamera.aspect=w/h;this.closeCamera.updateProjectionMatrix();
      this.renderer.setScissorTest(true);this.renderer.setScissor(8,height-h-8,w,h);this.renderer.setViewport(8,height-h-8,w,h);
      this.group.visible=false;this.renderer.render(this.scene,this.closeCamera);this.group.visible=true;
      this.renderer.setScissorTest(false);this.renderer.setViewport(0,0,width,height);
    }
    this.card.querySelector('.seek').value=Math.floor(this.time);
    this.card.querySelector('.time').textContent=(this.time/1000).toFixed(2)+'초';
  }
  tick(ms){
    if(this.lastTime!==null&&this.playing&&!document.hidden){
      this.time=(this.time+Math.min(ms-this.lastTime,100)*this.speed)%this.duration;
      if(this.time<this.lastSoundTime){this.sound?.stop();this.lastSoundTime=-1}
      if(this.audible)for(const e of this.soundEvents)if(e.time>this.lastSoundTime&&e.time<=this.time)this.sound.play(e.name,this.speed);
      this.lastSoundTime=this.time;
    }
    this.lastTime=ms;this.orbit.update();this.render();
  }
}
