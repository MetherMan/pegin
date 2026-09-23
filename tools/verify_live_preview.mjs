import assert from 'node:assert/strict';
import {readFile} from 'node:fs/promises';
import vm from 'node:vm';

const root=new URL('../client-overlay/Tools/SkillColors/',import.meta.url);
const queueSource=await readFile(new URL('live_preview_queue.js',root),'utf8');
const {DirtySkills,LivePreviewQueue}=await import('data:text/javascript;base64,'+Buffer.from(queueSource).toString('base64'));
const pause=ms=>new Promise(resolve=>setTimeout(resolve,ms));
const until=async condition=>{for(let i=0;i<200;i++){if(condition())return;await pause(5)}throw Error('Live update queue did not settle')};

const dirty=new DirtySkills(['meteor','frost','heaven','six']),flushed=[];
dirty.mark('meteor');dirty.flush(false,kind=>flushed.push(kind)); // blank input
dirty.mark('frost');dirty.flush(false,kind=>flushed.push(kind)); // other skill changed
dirty.mark('meteor');dirty.flush(true,kind=>flushed.push(kind)); // first input fixed
assert.deepEqual(flushed,['meteor','frost'],'Invalid input must not discard other changed skills');
dirty.mark('heaven');dirty.flush(false,kind=>flushed.push(kind)); // initial assets loading
assert.equal(dirty.size,1);
dirty.flush(true,kind=>flushed.push(kind)); // ready
assert.deepEqual(flushed,['meteor','frost','heaven']);
dirty.mark('six');dirty.flush(false,kind=>flushed.push(kind)); // invalid color
dirty.mark('frost');dirty.flush(true,kind=>flushed.push(kind));
assert.deepEqual(flushed.slice(-2),['six','frost']);

const requests=[],renders=[];let active=0,maximum=0;
const queue=new LivePreviewQueue({delay:0,
  request:(kind,value)=>new Promise(resolve=>{
    active++;maximum=Math.max(maximum,active);
    requests.push({kind,value,finish:()=>{active--;resolve(value)}});
  }),
  apply:async(kind,data)=>{await pause(5);renders.push([kind,data])}
});
queue.enqueue('heaven',1);await until(()=>requests.length===1);
for(let value=2;value<=40;value++)queue.enqueue('heaven',value);
queue.enqueue('frost',.4);
requests[0].finish();await until(()=>requests.length===2);
assert.deepEqual(renders,[['heaven',1]],'Continuous input must still display completed native frames');
assert.equal(requests[1].value,40,'Coalesce all pending slider values to the newest');
queue.enqueue('heaven',41);requests[1].finish();await until(()=>requests.length===3);
assert.equal(requests[2].kind,'frost','An unrelated skill cannot starve');
requests[2].finish();await until(()=>requests.length===4);requests[3].finish();await until(()=>queue.idle);
assert.equal(maximum,1,'No stale request may finish after a newer request');
assert.deepEqual(renders,[['heaven',1],['heaven',40],['frost',.4],['heaven',41]]);

// Exercise the actual applyAssets method independently of the graphics driver.
// setPhase intentionally resets time as the real native timeline builder does.
let source=await readFile(new URL('preview.js',root),'utf8');
source=source.replace(/^import .*;\r?\n/gm,'').replace('export class SkillPreview','class SkillPreview')+'\nglobalThis.PreviewClass=SkillPreview;';
const context={THREE:{Vector3:class{},TextureLoader:class{}},performance};
vm.runInNewContext(source,context);
for(const playing of [false,true]){
  const preview=Object.create(context.PreviewClass.prototype);
  Object.assign(preview,{time:2190,playing,phase:'B',duration:4000,geometries:{},textures:{},
    assets:{effects:{old:{}},scripts:{heaven:{B:[]},frost:{B:['preserved']}},timing:{heaven:{first:1750},frost:{first:100}},
      meshes:{},textures:{},sources:{},traces:{},actors:{native:true},sounds:{},soundDurations:{}},
    setPhase(){this.time=0;this.duration=4200},setPlaying(value){this.playing=value},render(){this.rendered=true}
  });
  await preview.applyAssets({effects:{new:{}},scripts:{heaven:{B:['changed']}},timing:{heaven:{first:1500}},meshes:{},textures:{},sources:{},traces:{}});
  assert.equal(preview.time,2190,'Applying values must retain the current effect moment');
  assert.equal(preview.playing,playing,'Pause/play state must survive a native update');
  assert.equal(preview.lastSoundTime,2190,'Applying values must not replay already heard launch sounds');
  assert.deepEqual(preview.assets.scripts.frost.B,['preserved']);
  assert.equal(preview.assets.timing.frost.first,100);
  assert.equal(preview.rendered,true);
}
console.log('PASS live-preview queue: rapid input coalescing, intermediate native frames, latest value, independent skills, playing/paused timeline preservation');
