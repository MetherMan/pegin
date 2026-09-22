import {SkillPreview} from './preview.js';
const settings=JSON.parse(document.getElementById('settings').textContent);
const colors={...settings.colors},tuning=structuredClone(settings.tuning),previews={};
const status=document.getElementById('status'),applyButton=document.getElementById('apply');
const valid=value=>/^#[0-9a-f]{6}$/i.test(value);
let ready=false,serial=0,timer=null;
const dirty=new Set();
function validForm(){return [...document.querySelectorAll('.tune-number')].every(e=>e.value!==''&&e.checkValidity())&&Object.keys(colors).every(k=>valid(document.getElementById(k+'Hex').value))}
async function request(path){
  const response=await fetch(path,{method:'POST',headers:{'Content-Type':'application/json','X-Local-Token':settings.token},body:JSON.stringify({colors,tuning})});
  const data=await response.json();if(!response.ok)throw Error(data.message||data.error||'설정을 읽지 못했습니다.');return data;
}
function queue(kind){
  if(kind)dirty.add(kind);else Object.keys(previews).forEach(k=>dirty.add(k));
  serial++;clearTimeout(timer);applyButton.disabled=true;
  if(!ready||!validForm()){status.textContent='각 항목의 입력 범위와 색상 코드를 확인해 주세요.';return}
  status.textContent='미리보기 반영 중…';timer=setTimeout(refresh,180);
}
async function refresh(){
  const version=serial,kinds=[...dirty];
  try{
    const data=await request('/preview');if(version!==serial)return;
    for(const kind of kinds)previews[kind].applyAssets(data);
    dirty.clear();status.textContent='미리보기에 반영했습니다. 게임에 적용하려면 전체 설정 적용을 눌러 주세요.';applyButton.disabled=false;
  }catch(error){if(version===serial)status.textContent=error.message}
}
for(const kind of Object.keys(colors)){
  const picker=document.getElementById(kind),hex=document.getElementById(kind+'Hex');picker.value=hex.value=colors[kind];
  function change(value){colors[kind]=value.toUpperCase();picker.value=hex.value=colors[kind];queue(kind)}
  picker.oninput=()=>change(picker.value);hex.oninput=()=>{if(valid(hex.value))change(hex.value);else{applyButton.disabled=true;status.textContent='색상을 #RRGGBB 형식으로 입력해 주세요.'}};
}
for(const [kind,fields] of Object.entries(settings.fields)){
  const body=document.querySelector(`[data-kind="${kind}"] .body`),panel=document.createElement('div');panel.className='tuning';body.append(panel);
  for(const [key,[label,min,max,step,unit]] of Object.entries(fields)){
    const row=document.createElement('label');row.className='tune-row';
    row.innerHTML=`<span>${label}<small>${unit}</small></span><input class="tune-range" type="range" min="${min}" max="${max}" step="${step}" aria-label="${label} 슬라이더"><input class="tune-number" type="number" min="${min}" max="${max}" step="${step}" aria-label="${label}" data-kind="${kind}" data-key="${key}">`;
    const range=row.querySelector('.tune-range'),number=row.querySelector('.tune-number');range.value=number.value=tuning[kind][key];
    range.oninput=()=>{number.value=range.value;tuning[kind][key]=Number(range.value);queue(kind)};
    number.oninput=()=>{if(number.value!==''&&number.checkValidity()){range.value=number.value;tuning[kind][key]=Number(number.value)}queue(kind)};
    panel.append(row);
  }
}
document.getElementById('reset').onclick=()=>{
  for(const kind in tuning)Object.assign(tuning[kind],settings.defaults[kind]);
  for(const number of document.querySelectorAll('.tune-number')){number.value=tuning[number.dataset.kind][number.dataset.key];number.previousElementSibling.value=number.value}
  queue();
};
applyButton.onclick=async()=>{
  if(!validForm())return;applyButton.disabled=true;status.textContent='게임 파일에 적용 중…';
  try{const data=await request('/apply');status.textContent=data.message}
  catch(error){status.textContent=error.message}
  finally{applyButton.disabled=false}
};
try{
  const response=await fetch('/preview.json'),assets=await response.json();
  if(!response.ok)throw Error(assets.error||'게임 원본을 읽지 못했습니다.');
  for(const kind of ['meteor','frost','heaven','six']){
    const card=document.querySelector(`[data-kind="${kind}"]`);
    previews[kind]=await SkillPreview.create(card,assets,kind,colors[kind]||null);card.querySelector('.loading').hidden=true;
  }
  ready=true;applyButton.disabled=false;status.textContent='원본 캐릭터·모션·사운드를 불러왔습니다. 소리 켜기를 누르면 해당 스킬만 들립니다.';
}catch(error){
  for(const loading of document.querySelectorAll('.loading')){loading.hidden=false;loading.textContent='미리보기 로드 실패: '+error.message}
  status.textContent=error.message;
}
