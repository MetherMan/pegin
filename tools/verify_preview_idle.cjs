// Exercise the real preview methods without a WebGL device or browser globals.
const assert=require('node:assert/strict');
const fs=require('node:fs');
const path=require('node:path');
const vm=require('node:vm');
const source=fs.readFileSync(path.join(__dirname,'../client-overlay/Tools/SkillColors/preview.js'),'utf8');
const document={hidden:false};
const context={document,labels:{meteor:'메테오'}};
vm.runInNewContext(source.slice(source.indexOf('export class SkillPreview')).replace('export class SkillPreview','globalThis.SkillPreview=class SkillPreview'),context);
const proto=context.SkillPreview.prototype;
let draws=0,orbitUpdates=0,cameraChanged=false,soundsStopped=0;
const preview=Object.assign(Object.create(proto),{
  playing:false,time:100,lastTime:null,lastSoundTime:100,duration:1000,speed:1,
  needsRender:false,audible:false,
  orbit:{update(){orbitUpdates++;return cameraChanged}},
  sound:{stop(){soundsStopped++}},
  render(){draws++;this.needsRender=false},
  card:{querySelector(){return {setAttribute(){}}}},kind:'meteor'
});
for(let frame=0;frame<120;frame++)preview.tick(frame*16);
assert.equal(draws,0,'Paused static scenes must not redraw actors/effects');
assert.equal(preview.time,100,'Paused scene time must stay fixed');
cameraChanged=true;preview.tick(2000);assert.equal(draws,1,'Paused orbit damping must redraw');
cameraChanged=false;preview.needsRender=true;preview.tick(2016);assert.equal(draws,2,'Orbit events must invalidate the frame');
preview.tick(2032);assert.equal(draws,2,'Settled camera must stop drawing');
preview.setPlaying(true);preview.tick(4000);assert.equal(preview.time,100,'Resuming after pause must not jump');
preview.tick(4016);assert.equal(preview.time,116);assert.equal(draws,4);
document.hidden=true;const priorOrbit=orbitUpdates;
preview.tick(5000);preview.tick(90000);
assert.equal(orbitUpdates,priorOrbit,'Hidden frames must skip camera/scene work');
assert.equal(draws,4);assert.equal(preview.time,116);
proto.render.call(preview);assert.equal(preview.needsRender,true,'Hidden explicit renders must wait for visibility');
document.hidden=false;preview.tick(100000);assert.equal(preview.time,116,'Returning to a hidden tab must not jump');
preview.tick(100016);assert.equal(preview.time,132);
preview.setPlaying(false);assert.equal(soundsStopped,1);
preview.needsRender=true;preview.tick(100032);assert.equal(draws,7,'Changes to a paused scene must remain visible');
preview.tick(100048);assert.equal(draws,7);
console.log('PASS: paused idle, camera damping/invalidation, active playback, hidden work suppression, visibility/pause time continuity');
