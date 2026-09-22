// User-controlled skin tint. Red horns, eyes and blood keep their source pixels.
export const defaults={color:'#8A958D',strength:0,brightness:1};
export function skinWeight(r,g,b,x,y){
 if((x>=.275&&x<=.585&&y>=.36&&y<=.65)||
    (x<=.115&&y>=.30&&y<=.68)||
    (x>=.115&&x<=.30&&y>=.34&&y<=.68)||
    (x>=.325&&x<=.475&&y>=.64&&y<=.84)||
    (x>=.30&&x<=.68&&y>=.72)||
    (x>=.71&&x<=.87&&y>=.765&&y<=.865)) return 0;
 const redness=r-Math.max(g,b);
 return 1-Math.max(0,Math.min(1,redness/Math.max(8,r*.22)));
}
export function tint(source,output,width,height,settings){
 const color=[1,3,5].map(i=>parseInt(settings.color.slice(i,i+2),16));
 const value=Math.max(1,.2126*color[0]+.7152*color[1]+.0722*color[2]);
 for(let y=0;y<height;y++)for(let x=0;x<width;x++){
  const i=(y*width+x)*4,r=source[i],g=source[i+1],b=source[i+2];
  const weight=skinWeight(r,g,b,x/width,y/height),lum=.2126*r+.7152*g+.0722*b;
  for(let c=0;c<3;c++){
   const target=(source[i+c]*(1-settings.strength)+lum*color[c]/value*settings.strength)*settings.brightness;
   output[i+c]=Math.max(0,Math.min(255,Math.round(source[i+c]*(1-weight)+target*weight)));
  }
  output[i+3]=source[i+3];
 }
 return output;
}
function bmp(canvas){
 const w=canvas.width,h=canvas.height,stride=(w*3+3)&~3;
 const result=new Uint8Array(54+stride*h),v=new DataView(result.buffer),rgba=canvas.getContext('2d').getImageData(0,0,w,h).data;
 result[0]=66;result[1]=77;v.setUint32(2,result.length,true);v.setUint32(10,54,true);v.setUint32(14,40,true);
 v.setInt32(18,w,true);v.setInt32(22,h,true);v.setUint16(26,1,true);v.setUint16(28,24,true);v.setUint32(34,stride*h,true);
 for(let y=0;y<h;y++)for(let x=0;x<w;x++){
  const a=(y*w+x)*4,b=54+(h-1-y)*stride+x*3;
  result[b]=rgba[a+2];result[b+1]=rgba[a+1];result[b+2]=rgba[a];
 }
 let binary='';for(let i=0;i<result.length;i+=32768)binary+=String.fromCharCode(...result.subarray(i,i+32768));
 return btoa(binary);
}
export async function attachSkinEditor(texture,T){
 const canvas=document.createElement('canvas');canvas.width=canvas.height=1024;
 const context=canvas.getContext('2d',{willReadFrequently:true});context.drawImage(texture.image,0,0,1024,1024);
 const original=context.getImageData(0,0,1024,1024),pixels=context.createImageData(1024,1024);
 const replacement=new T.CanvasTexture(canvas);replacement.flipY=false;replacement.colorSpace=T.SRGBColorSpace;
 let settings=await (await fetch('/settings.json')).json();
 const color=document.querySelector('#skin-color'),strength=document.querySelector('#skin-strength'),brightness=document.querySelector('#skin-brightness'),message=document.querySelector('#save-status');
 function refresh(){
  tint(original.data,pixels.data,1024,1024,settings);context.putImageData(pixels,0,0);replacement.needsUpdate=true;
  color.value=settings.color;strength.value=settings.strength;brightness.value=settings.brightness;
  document.querySelector('#strength-value').textContent=Math.round(settings.strength*100)+'%';
  document.querySelector('#brightness-value').textContent=settings.brightness.toFixed(2)+'배';
 }
 let pending;
 function edited(){settings={color:color.value,strength:Number(strength.value),brightness:Number(brightness.value)};message.textContent='미리보기 변경 · 아직 저장하지 않음';cancelAnimationFrame(pending);pending=requestAnimationFrame(refresh);}
 color.oninput=()=>{if(Number(strength.value)===0)strength.value=.75;edited();};strength.oninput=brightness.oninput=edited;
 document.querySelector('#skin-reset').onclick=()=>{settings={...defaults};refresh();message.textContent='기본 피부색 · 저장하면 적용';};
 document.querySelector('#skin-save').onclick=async()=>{
  const button=document.querySelector('#skin-save');button.disabled=true;message.textContent='저장 중…';
  try{
   cancelAnimationFrame(pending);refresh();
   const response=await fetch('/apply',{method:'POST',headers:{'Content-Type':'application/json','X-Local-Token':document.querySelector('meta[name=local-token]').content},body:JSON.stringify({settings,bmp:bmp(canvas)})});
   const result=await response.json();if(!response.ok)throw Error(result.message);
   message.textContent=result.message;
  }catch(error){message.textContent='저장 실패: '+error.message;}finally{button.disabled=false;}
 };
 refresh();return replacement;
}
