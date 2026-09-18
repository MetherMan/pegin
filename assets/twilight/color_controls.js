const appearanceDefaults={blueColor:'#246aff',blueSaturation:1,blueBrightness:1,metalColor:'#ffffff',metalRoughness:.38,clothColor:'#b8b1a0',clothBrightness:1,clothRoughness:.95,clothWeave:.55};
const appearanceKey='twilight-appearance-v1';
const appearanceRanges={blueSaturation:[0,2],blueBrightness:[.25,2],metalRoughness:[.08,1],clothBrightness:[.3,1.8],clothRoughness:[.2,1],clothWeave:[0,1]};
function validAppearance(raw){const value={...appearanceDefaults};for(const key of Object.keys(value)){if(key.endsWith('Color')){if(typeof raw?.[key]==='string'&&/^#[0-9a-f]{6}$/i.test(raw[key]))value[key]=raw[key].toLowerCase();}else if(Number.isFinite(raw?.[key]))value[key]=Math.min(appearanceRanges[key][1],Math.max(appearanceRanges[key][0],raw[key]));}return value;}
const shippedAppearance=__GAME_APPEARANCE__;
let appearance=validAppearance({...appearanceDefaults,...shippedAppearance});try{const saved=JSON.parse(localStorage.getItem(appearanceKey));if(saved)appearance=validAppearance(saved)}catch{}
const appearanceUniforms={twHue:{value:0},twSaturation:{value:1},twBrightness:{value:1},twMetal:{value:new THREE.Color('#ffffff')},twWeave:{value:.55}};
const hsvGLSL=`
vec3 twRgbToHsv(vec3 c){vec4 K=vec4(0.,-1./3.,2./3.,-1.);vec4 p=mix(vec4(c.bg,K.wz),vec4(c.gb,K.xy),step(c.b,c.g));vec4 q=mix(vec4(p.xyw,c.r),vec4(c.r,p.yzx),step(p.x,c.r));float d=q.x-min(q.w,q.y),e=1.e-10;return vec3(abs(q.z+(q.w-q.y)/(6.*d+e)),d/(q.x+e),q.x);}
vec3 twHsvToRgb(vec3 c){vec3 p=abs(fract(c.xxx+vec3(0.,2./3.,1./3.))*6.-3.);return c.z*mix(vec3(1.),clamp(p-1.,0.,1.),c.y);}
`;
const appearanceMaterials=new Set();
function initAppearance(model,originals){
 for(const mat of originals.values())appearanceMaterials.add(mat);
 for(const mat of appearanceMaterials){
  if(mat.map&&['Moonlit honed silver','Lunar relief silver'].includes(mat.name)){
   mat.map.anisotropy=renderer.capabilities.getMaxAnisotropy();
   mat.bumpMap=mat.map.clone();mat.bumpMap.colorSpace=THREE.NoColorSpace;mat.bumpMap.needsUpdate=true;mat.bumpScale=.00035;
  }
  if(mat.map&&mat.name!=='Weathered ivory woven bandage'){mat.onBeforeCompile=shader=>{Object.assign(shader.uniforms,appearanceUniforms);shader.fragmentShader='uniform float twHue,twSaturation,twBrightness;uniform vec3 twMetal;\n'+hsvGLSL+shader.fragmentShader;shader.fragmentShader=shader.fragmentShader.replace('#include <map_fragment>',`#include <map_fragment>
   vec3 twSrc=diffuseColor.rgb;vec3 twHsv=twRgbToHsv(twSrc);
   float twBlue=smoothstep(.03,.22,(twSrc.b-max(twSrc.r,twSrc.g))/max(twHsv.z,.0001)) * smoothstep(.08,.30,twHsv.y);
   vec3 twEdited=twHsvToRgb(vec3(fract(twHsv.x+twHue+1.),clamp(twHsv.y*twSaturation,0.,1.),twHsv.z*twBrightness));
   float twMetalMask=(1.-twBlue)*smoothstep(.012,.14,max(twSrc.r,max(twSrc.g,twSrc.b)));
   diffuseColor.rgb=mix(twSrc,twEdited,twBlue);diffuseColor.rgb*=mix(vec3(1.),twMetal,twMetalMask);
  `);};mat.customProgramCacheKey=()=> 'twilight-color-mask-v1';}
  if(mat.name==='Weathered ivory woven bandage'){
   // The albedo carries the reference's irregular fibers and wear. Derive a
   // subtle relief from this same map, instead of overlaying regular stripes.
   mat.map.anisotropy=renderer.capabilities.getMaxAnisotropy();
   mat.bumpMap=mat.map.clone();mat.bumpMap.colorSpace=THREE.NoColorSpace;mat.bumpMap.needsUpdate=true;
   mat.bumpScale=.0007+.002*appearance.clothWeave;
   mat.onBeforeCompile=shader=>{shader.fragmentShader=shader.fragmentShader.replace('#include <map_fragment>',`#include <map_fragment>
    diffuseColor.rgb*=1.3;
   `);};
   mat.customProgramCacheKey=()=> 'twilight-reference-linen-v2';
  }
  mat.needsUpdate=true;
 }
 document.querySelector('#originalIcon').append(document.querySelector('#icon').cloneNode());
 function syncControls(){for(const [key,value] of Object.entries(appearance)){const el=document.getElementById(key);el.value=value;if(key.endsWith('Color'))document.getElementById(key.replace('Color','Hex')).value=value;else document.querySelector(`label[for="${key}"] output`).textContent=Math.round(value*100)+'%';}}
 function apply(persist=true){
  if(persist&&clay)document.querySelector('#clay').click();
  const referenceRgb=new THREE.Color(appearanceDefaults.blueColor),chosenRgb=new THREE.Color(appearance.blueColor);
  const reference=referenceRgb.getHSL({}),chosen=chosenRgb.getHSL({});
  const hsvSV=c=>{const v=Math.max(c.r,c.g,c.b);return {s:v?(v-Math.min(c.r,c.g,c.b))/v:0,v};};
  const referenceSV=hsvSV(referenceRgb),chosenSV=hsvSV(chosenRgb);
  appearanceUniforms.twHue.value=chosen.h-reference.h;appearanceUniforms.twSaturation.value=appearance.blueSaturation*chosenSV.s/referenceSV.s;appearanceUniforms.twBrightness.value=appearance.blueBrightness*chosenSV.v/referenceSV.v;appearanceUniforms.twMetal.value.set(appearance.metalColor);appearanceUniforms.twWeave.value=appearance.clothWeave;
  for(const mat of appearanceMaterials){
   if(mat.map&&mat.name!=='Weathered ivory woven bandage')mat.roughness=appearance.metalRoughness;
   if(['Moonlit honed silver','Lunar relief silver'].includes(mat.name)){mat.color.set(mat.map?'#ffffff':appearance.metalColor);mat.roughness=appearance.metalRoughness;}
   if(mat.name==='Weathered ivory woven bandage'){mat.color.set(appearance.clothColor).multiplyScalar(appearance.clothBrightness);mat.roughness=appearance.clothRoughness;mat.metalness=0;mat.bumpScale=.0007+.002*appearance.clothWeave;}
   if(mat.name==='Linen edge stitching')mat.color.set(appearance.clothColor).multiplyScalar(.26*appearance.clothBrightness);
  }
  syncControls();if(persist){try{localStorage.setItem(appearanceKey,JSON.stringify(appearance));document.querySelector('#appearanceStatus').textContent='자동 저장됨 · 현재 브라우저의 3D 미리보기에 적용';}catch{document.querySelector('#appearanceStatus').textContent='브라우저 저장이 제한돼 있어요. 설정 파일로 저장해 주세요.';}}
  if(!persist&&Object.keys(shippedAppearance).length)document.querySelector('#appearanceStatus').textContent=Object.entries(shippedAppearance).every(([k,v])=>appearance[k]===v)?'이 색상은 게임에 반영되어 있습니다. 추가 조정은 미리보기에만 적용됩니다.':'저장된 조절값을 불러왔습니다. 게임에 반영하려면 설정을 저장해 주세요.';
  window.appearanceState={...appearance};window.dispatchEvent(new Event('appearancechange'));
 }
 for(const key of Object.keys(appearanceDefaults))document.getElementById(key).addEventListener('input',e=>{if(clay)document.querySelector('#clay').click();appearance[key]=key.endsWith('Color')?e.target.value:Number(e.target.value);apply();});
 for(const key of ['blue','metal','cloth'])document.getElementById(key+'Hex').addEventListener('change',e=>{if(/^#[0-9a-f]{6}$/i.test(e.target.value)){e.target.setCustomValidity('');appearance[key+'Color']=e.target.value;apply();}else{e.target.setCustomValidity('#RRGGBB 형식으로 입력해 주세요');e.target.reportValidity();}});
 document.querySelector('#resetAppearance').onclick=()=>{appearance={...appearanceDefaults};apply();};
 const presets={moon:{blueColor:'#246aff',blueSaturation:1,blueBrightness:1,metalColor:'#ffffff'},deep:{blueColor:'#2646cf',blueSaturation:1.25,blueBrightness:.6},linen:{clothColor:'#b0a087',clothBrightness:.9,clothRoughness:1,clothWeave:.8}};
 document.querySelectorAll('[data-preset]').forEach(b=>b.onclick=()=>{appearance={...appearance,...presets[b.dataset.preset]};apply();});
 document.querySelector('#saveAppearance').onclick=()=>{const data={format:'twilight-appearance',version:1,weapon:'푸른달의 트와일라잇 양손검',settings:appearance};const url=URL.createObjectURL(new Blob([JSON.stringify(data,null,2)],{type:'application/json'}));const link=document.createElement('a');link.href=url;link.download='twilight-colors.json';link.click();setTimeout(()=>URL.revokeObjectURL(url),1000);document.querySelector('#appearanceStatus').textContent='색상 설정을 파일로 저장했습니다. 나중에 이 파일로 반영할 수 있어요.';};
 document.querySelector('#loadAppearance').onclick=()=>document.querySelector('#appearanceFile').click();
 document.querySelector('#appearanceFile').onchange=async e=>{try{const f=e.target.files[0];if(!f)return;if(f.size>50000)throw Error('설정 파일이 너무 큽니다.');const data=JSON.parse(await f.text());if(data.format!=='twilight-appearance'||data.version!==1||!data.settings)throw Error('이 미리보기에서 저장한 설정 파일을 선택해 주세요.');appearance=validAppearance(data.settings);apply();}catch(err){document.querySelector('#appearanceStatus').textContent=err.message;}finally{e.target.value='';}};
 window.twilightAppearance={get:()=>({...appearance}),set:settings=>{appearance=validAppearance({...appearance,...settings});apply();},reset:()=>{appearance={...appearanceDefaults};apply();}};
 apply(false);window.appearanceReady=true;
}
