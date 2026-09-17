import fs from 'node:fs';import path from 'node:path';import {fileURLToPath} from 'node:url';
const out=path.dirname(fileURLToPath(import.meta.url));const enc=s=>'data:text/javascript;base64,'+Buffer.from(s).toString('base64');const read=n=>fs.readFileSync(path.join(out,'vendor',n),'utf8');
const imports={three:enc(read('three.module.js')),controls:enc(read('OrbitControls.js')),loader:enc(read('GLTFLoader.js').replace('../utils/BufferGeometryUtils.js','bufferutils')),room:enc(read('RoomEnvironment.js')),bufferutils:enc(read('BufferGeometryUtils.js'))};
let html=fs.readFileSync(path.join(out,'viewer_template.html'),'utf8').replace('__IMPORTMAP__',JSON.stringify({imports})).replace('__GLB__',fs.readFileSync(path.join(out,'Bloodfang_Openwork_Spear.glb')).toString('base64'));
fs.writeFileSync(path.join(out,'모델_돌려보기.html'),html);console.log('Offline viewer saved',html.length);
