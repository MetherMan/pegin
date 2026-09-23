import {SkillPreview} from './preview.js';
import {DirtySkills,LivePreviewQueue} from './live_preview_queue.js';
const settings=JSON.parse(document.getElementById('settings').textContent);
const colors={...settings.colors},tuning=structuredClone(settings.tuning),previews={};
const status=document.getElementById('status'),applyButton=document.getElementById('apply'),saveButton=document.getElementById('savePreview');
const previewOnly=document.body.dataset.previewOnly==='true';
saveButton.hidden=!previewOnly;
const valid=value=>/^#[0-9a-f]{6}$/i.test(value);
let ready=false,previewError=false;
const dirtySkills=new DirtySkills(Object.keys(tuning));
const groups={
  meteor:[
    ['광량 · 진하기',['brightness','intensity'],'광량은 전체 빛의 세기, 진하기는 핵심 화염탄을 제외한 마법진·잔광·폭풍의 비중입니다.'],
    ['포격 · 화염탄',['projectileSize','finalProjectileSize','flightSpeed'],'일반 포격의 탄속과 화염탄 크기를 조절합니다.'],
    ['마지막 메테오 · 속도와 간격',['finalFlightSpeed','finalSealDelay','finalFallDelay'],'속도 배율을 낮추면 천천히 낙하합니다. 앞선 포격 종료 후 대기와 최종 마법진 생성 후 낙하 대기를 각각 조절합니다.'],
    ['폭발',['impactSize','finalImpactSize','impactHeight'],'보이는 폭발 크기만 바뀌며 서버의 피해 범위는 유지됩니다.'],
    ['포격 마법진 배치',['portalSize','portalHeight','portalOffsetX','portalOffsetY','portalSpread'],'좌우·앞뒤·높이는 기존 배치에서 더하는 값입니다.'],
    ['최종 마법진 배치',['finalPortalSize','finalPortalHeight','finalPortalOffsetX','finalPortalOffsetY'],'최종 화염탄의 출발점도 마법진을 따라 이동합니다.'],
    ['연출 타이밍',['portalGap','readyHold','shotGap'],'밀리초 단위로 마법진과 포격 간격을 맞춥니다.']
  ],
  frost:[
    ['얼음 · 안개 · 눈보라',['iceOpacity','iceDensity','brightness','intensity','mistAmount','blizzardStrength','mistStrength'],'불투명도는 얼음 결정·중앙 기둥·가시 뒤로 배경이 비치는 정도입니다. 완전히 솟은 얼음은 100%에서 배경을 가립니다. 테두리 빛은 남으며, 표면 진하기·안개·눈보라는 따로 조절합니다.'],
    ['불규칙한 원형 얼음 군집',['distance','inwardStartRadius','inwardEndRadius','size','riseSpeed','spreadAngle'],'외곽부터 안쪽으로 솟는 순서를 유지하며 결정의 간격·크기·기울기를 불규칙하게 배치합니다. 서버 피해 범위는 유지됩니다.'],
    ['마법진 배치',['sealSize','sealLowerHeight','sealMiddleHeight','sealUpperHeight','sealOffsetX','sealOffsetY'],'세 마법진의 높이는 각 기존 높이에서 더하는 값입니다.'],
    ['연출 타이밍',['startDelay','stepGap'],'바깥 원형 고리부터 안쪽 고리까지 차례로 솟은 뒤 마지막 중앙 거대 기둥이 나타나는 시간을 조절합니다.']
  ],
  heaven:[
    ['광량 · 진하기',['brightness','intensity'],'광량은 전체 빛의 세기, 진하기는 발사·적중의 보조층과 잔광 비중입니다.'],
    ['화살 · 기존 적중',['projectileSize','launchSize','impactSize','impactHeight'],'기존 발사광·화살·몸통 타격 이펙트를 유지합니다.'],
    ['추가 관통선',['pierceWidth','areaSize'],'기존 타격 위에 직선 관통광을 더합니다. 몸 앞뒤로 뻗는 길이와 굵기를 따로 조절하며 피해 횟수·범위는 유지됩니다.'],
    ['연사 타이밍',['startDelay','shotGap','flightSpeed'],'발사 시점·3연사 간격·실제 화살 비행 속도입니다.']
  ],
  six:[
    ['광량 · 진하기',['brightness','intensity'],'광량은 전체 빛의 세기, 진하기는 발사·적중의 보조층과 잔광 비중입니다.'],
    ['화살 · 폭발',['projectileSize','launchSize','impactSize','impactHeight'],'기존 활과 푸른달 활의 두 연출에 같은 조절값을 사용합니다.'],
    ['연사 타이밍',['startDelay','shotGap','flightSpeed'],'발사 시점·6연사 간격·실제 화살 비행 속도입니다.']
  ]
};
function validForm(){return [...document.querySelectorAll('.tune-number')].every(e=>e.value!==''&&e.checkValidity())&&Object.keys(colors).every(k=>valid(document.getElementById(k+'Hex').value))}
function enableActions(enabled){applyButton.disabled=!enabled;saveButton.disabled=!enabled}
function changedBadges(){
  for(const group of document.querySelectorAll('.tune-group')){
    const count=[...group.querySelectorAll('.tune-number')].filter(n=>Number(n.value)!==settings.defaults[n.dataset.kind][n.dataset.key]).length;
    group.querySelector('.group-count').textContent=count?'추천값과 '+count+'개 다름':'';
  }
}
async function request(path,extra={},snapshot={colors,tuning}){
  const response=await fetch(path,{method:'POST',headers:{'Content-Type':'application/json','X-Local-Token':settings.token},body:JSON.stringify({...snapshot,...extra})});
  const data=await response.json();if(!response.ok)throw Error(data.message||data.error||'설정을 읽지 못했습니다.');return data;
}
function feedback(kind,text){
  const card=document.querySelector('[data-kind="'+kind+'"]');
  const note=card.querySelector('.live-feedback');if(note)note.textContent=text;
}
const liveQueue=new LivePreviewQueue({
  request:(kind,snapshot)=>{
    const knownSources=Object.fromEntries(Object.entries(previews[kind].assets.sources).filter(([path])=>/^(Effect\/.*\.WEM|Texture\/Effect\/.*\.wtm)$/i.test(path)));
    return request('/preview',{kinds:[kind],knownSources},snapshot);
  },
  apply:(kind,data)=>previews[kind].applyAssets(data),
  onApplied:(kind,result)=>{
    feedback(kind,result.pending?'다음 조절값 반영 중…':`반영됨 · ${(result.milliseconds/1000).toFixed(2)}초`);
  },
  onError:(kind,error,pending)=>{
    feedback(kind,pending?'다음 조절값으로 다시 반영 중…':error.message);
    if(!pending){previewError=true;status.textContent=error.message}
  },
  onIdle:()=>{
    if(!previewError&&validForm())status.textContent=previewOnly?'미리보기에 반영했습니다. 설정 저장을 누르면 다음에 열 때도 유지됩니다.':'미리보기에 반영했습니다. 게임에 적용하려면 전체 설정 적용을 눌러 주세요.';
    enableActions(!previewError&&validForm());
  }
});
function queue(kind){
  // Remember the changed skill before any gate. Another temporarily blank
  // field, or the initial asset load, must never drop this native update.
  dirtySkills.mark(kind);
  enableActions(false);changedBadges();
  if(!ready){status.textContent='원본을 불러오는 중입니다. 조절값은 완료 후 반영합니다.';return}
  if(!validForm()){status.textContent='각 항목의 입력 범위와 색상 코드를 확인해 주세요.';return}
  previewError=false;status.textContent='미리보기 반영 중…';
  const snapshot=structuredClone({colors,tuning});
  dirtySkills.flush(true,selected=>{
    feedback(selected,'반영 중…');liveQueue.enqueue(selected,snapshot);
  });
}
for(const kind of Object.keys(colors)){
  const picker=document.getElementById(kind),hex=document.getElementById(kind+'Hex');picker.value=hex.value=colors[kind];
  const skill=kind==='frostIce'?'frost':kind;
  function change(value){colors[kind]=value.toUpperCase();picker.value=hex.value=colors[kind];queue(skill)}
  picker.oninput=()=>change(picker.value);hex.oninput=()=>{if(valid(hex.value))change(hex.value);else{dirtySkills.mark(skill);enableActions(false);status.textContent='색상을 #RRGGBB 형식으로 입력해 주세요.'}};
}
function syncInputs(kind){
  for(const number of document.querySelectorAll('.tune-number')){
    if(kind&&number.dataset.kind!==kind)continue;
    number.value=tuning[number.dataset.kind][number.dataset.key];number.previousElementSibling.value=number.value;
  }
}
for(const [kind,fields] of Object.entries(settings.fields)){
  const body=document.querySelector('[data-kind="'+kind+'"] .body'),panel=document.createElement('div');panel.className='tuning';body.append(panel);
  const top=document.createElement('div');top.className='tuning-title';top.innerHTML='<strong>세부 연출 조절</strong><span class="live-feedback" role="status" aria-live="polite"></span><button type="button" class="quiet reset-skill">이 스킬 추천값</button>';panel.append(top);
  top.querySelector('button').onclick=()=>{Object.assign(tuning[kind],settings.defaults[kind]);syncInputs(kind);queue(kind)};
  const grouped=new Set();
  const makeGroup=([title,keys,help],index)=>{
    const section=document.createElement('details');section.className='tune-group';section.open=index===0||keys.includes('finalFlightSpeed')||keys.includes('pierceWidth');
    const summary=document.createElement('summary');summary.innerHTML='<span>'+title+'</span><span class="group-count"></span>';section.append(summary);
    const content=document.createElement('div');content.className='tune-group-content';section.append(content);
    if(help){const note=document.createElement('p');note.className='control-help';note.textContent=help;content.append(note)}
    for(const key of keys){
      if(!fields[key])continue;grouped.add(key);
      const [label,min,max,step,unit]=fields[key],row=document.createElement('label');row.className='tune-row';
      row.innerHTML='<span>'+label+'<small>'+unit+'</small></span><input class="tune-range" type="range" min="'+min+'" max="'+max+'" step="'+step+'" aria-label="'+label+' 슬라이더"><input class="tune-number" type="number" min="'+min+'" max="'+max+'" step="'+step+'" aria-label="'+label+'" data-kind="'+kind+'" data-key="'+key+'">';
      const range=row.querySelector('.tune-range'),number=row.querySelector('.tune-number');range.value=number.value=tuning[kind][key];
      range.oninput=()=>{number.value=range.value;tuning[kind][key]=Number(range.value);queue(kind)};
      number.oninput=()=>{if(number.value!==''&&number.checkValidity()){range.value=number.value;tuning[kind][key]=Number(number.value)}queue(kind)};
      content.append(row);
    }
    panel.append(section);
  };
  groups[kind].forEach(makeGroup);
  const rest=Object.keys(fields).filter(k=>!grouped.has(k));if(rest.length)makeGroup(['기타',rest,''],99);
}
changedBadges();
document.getElementById('reset').onclick=()=>{
  for(const kind in tuning)Object.assign(tuning[kind],settings.defaults[kind]);
  syncInputs();queue();
};
applyButton.onclick=async()=>{
  if(!validForm())return;enableActions(false);status.textContent='게임 파일에 적용 중…';
  try{const data=await request('/apply');status.textContent=data.message}
  catch(error){status.textContent=error.message}
  finally{enableActions(validForm())}
};
saveButton.onclick=async()=>{
  if(!previewOnly||!validForm())return;enableActions(false);status.textContent='미리보기 설정 저장 중…';
  try{const data=await request('/save-preview');status.textContent=data.message||'미리보기 설정을 저장했습니다. 설치된 게임은 그대로입니다.'}
  catch(error){status.textContent=error.message}
  finally{enableActions(validForm())}
};
try{
  const response=await fetch('/preview.json'),assets=await response.json();
  if(!response.ok)throw Error(assets.error||'게임 원본을 읽지 못했습니다.');
  for(const kind of ['meteor','frost','heaven','six']){
    const card=document.querySelector('[data-kind="'+kind+'"]');
    previews[kind]=await SkillPreview.create(card,assets,kind,colors[kind]||null);card.querySelector('.loading').hidden=true;
  }
  ready=true;
  if(dirtySkills.size){
    if(validForm()){
      previewError=false;const snapshot=structuredClone({colors,tuning});
      dirtySkills.flush(true,kind=>{feedback(kind,'반영 중…');liveQueue.enqueue(kind,snapshot)});
    }
  }else{
    enableActions(true);status.textContent='원본 캐릭터·모션·사운드를 불러왔습니다. '+(previewOnly?'항목을 펼쳐 조절하고 미리보기 설정 저장으로 남길 수 있습니다. 설치된 게임은 바뀌지 않습니다.':'소리 켜기를 누르면 해당 스킬만 들립니다.');
  }
}catch(error){
  for(const loading of document.querySelectorAll('.loading')){loading.hidden=false;loading.textContent='미리보기 로드 실패: '+error.message}
  status.textContent=error.message;
}
