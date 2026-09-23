import * as T from 'three';

export async function installHornControls(textures) {
  const slider=document.querySelector('#horn-strength'), value=document.querySelector('#horn-value'),
    save=document.querySelector('#horn-save'), status=document.querySelector('#horn-save-status');
  const response=await fetch('/api/horn-strength',{cache:'no-store'});
  if(!response.ok)throw new Error('조절 패널 서버(8879)에서 열어 주세요.');
  const settings=await response.json();
  const tiles=[];
  for(let i=0;i<2;i++){
    const tileImage=await new T.TextureLoader().loadAsync('/api/horn-base/'+i+'.png');
    const source=document.createElement('canvas');source.width=tileImage.image.width;source.height=tileImage.image.height;
    const sourceContext=source.getContext('2d',{willReadFrequently:true});sourceContext.drawImage(tileImage.image,0,0);
    const raw=sourceContext.getImageData(0,0,source.width,source.height);
    const canvas=document.createElement('canvas');canvas.width=2048;canvas.height=1024;
    const ctx=canvas.getContext('2d');ctx.drawImage(textures.primordial[i].image,0,0);
    const texture=new T.CanvasTexture(canvas);texture.flipY=false;texture.colorSpace=T.SRGBColorSpace;
    textures.primordial[i].dispose();textures.primordial[i]=texture;tileImage.dispose();
    tiles.push({raw,out:new ImageData(raw.width,raw.height),ctx,texture,x:2048-raw.width});
  }
  let saved=Number(settings.strength), scheduled=false;
  function paint(){
    scheduled=false;const strength=Number(slider.value), centers=[39,35,31];
    value.textContent=strength.toFixed(1)+'배';
    for(const tile of tiles){
      const input=tile.raw.data,out=tile.out.data;
      for(let p=0;p<input.length;p+=4){
        for(let c=0;c<3;c++)out[p+c]=Math.max(0,Math.min(255,Math.floor(centers[c]+(input[p+c]-centers[c])*strength+.5)));
        out[p+3]=255;
      }
      tile.ctx.putImageData(tile.out,tile.x,0);tile.texture.needsUpdate=true;
    }
    status.textContent=strength===saved?'저장된 값':'미저장 · 화면에 즉시 반영 중';
  }
  slider.value=String(saved);paint();slider.disabled=false;save.disabled=false;
  slider.addEventListener('input',()=>{value.textContent=Number(slider.value).toFixed(1)+'배';if(!scheduled){scheduled=true;requestAnimationFrame(paint);}});
  save.addEventListener('click',async()=>{
    slider.disabled=true;save.disabled=true;status.textContent='게임용 저장 중…';
    try{
      const result=await fetch('/api/horn-strength',{method:'POST',headers:{'Content-Type':'application/json'},body:JSON.stringify({strength:Number(slider.value)})});
      const body=await result.json();if(!result.ok||body.error)throw new Error(body.error||'저장 실패');
      saved=Number(body.strength);status.textContent='저장 완료 · 게임용 반영됨';
    }catch(error){status.textContent='저장 실패: '+error.message;}
    finally{slider.disabled=false;save.disabled=false;}
  });
}
