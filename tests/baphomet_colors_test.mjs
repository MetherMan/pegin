import assert from 'node:assert/strict';
import fs from 'node:fs';
const root = new URL('../', import.meta.url);
const source = fs.readFileSync(new URL('client-overlay/Tools/BaphometColors/skin.js', root), 'utf8');
const {tint, defaults, skinWeight} = await import('data:text/javascript;base64,' + Buffer.from(source).toString('base64'));
const input = new Uint8Array([80, 85, 90, 255, 180, 10, 10, 255]);
const output = new Uint8Array(input.length);
assert.deepEqual(tint(input, output, 2, 1, defaults), input);
tint(input, output, 2, 1, {color:'#4080FF',strength:.8,brightness:2});
assert.notDeepEqual(output.slice(0,4), input.slice(0,4));
assert.deepEqual(output.slice(4), input.slice(4));
for (const [x,y] of [[.4,.5],[.78,.80],[.83,.84]]) {
  assert.equal(skinWeight(80,85,90,x,y), 0, 'horn/eye pixels must be protected even when not red');
}
if (process.argv[2]) {
  const dir = process.argv[2];
  const pixels = fs.readFileSync(dir + '/source.rgba');
  const settings = JSON.parse(fs.readFileSync(dir + '/settings.json'));
  fs.writeFileSync(dir + '/tinted.rgba', tint(pixels, new Uint8Array(pixels.length),1024,1024,settings));
}
console.log('Skin tint: neutral identity, changed skin, unchanged red/horn/eye pixels passed.');
