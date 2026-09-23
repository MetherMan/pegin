// Coalesce rapid input per skill without throwing away every completed frame.
// One request is active; queued values replace older queued values. A response
// can never arrive after a newer response and overwrite the final adjustment.
export class DirtySkills{
  constructor(kinds){this.kinds=kinds;this.pending=new Set()}
  mark(kind){for(const selected of kind?[kind]:this.kinds)this.pending.add(selected)}
  flush(allowed,submit){
    if(!allowed)return false;
    const kinds=[...this.pending];this.pending.clear();
    for(const kind of kinds)submit(kind);
    return kinds.length>0;
  }
  get size(){return this.pending.size}
}

export class LivePreviewQueue{
  constructor({request,apply,onApplied=()=>{},onError=()=>{},onIdle=()=>{},delay=35}){
    Object.assign(this,{request,apply,onApplied,onError,onIdle,delay});
    this.pending=new Map();this.running=false;this.timer=null;this.revision=0;
  }
  enqueue(kind,value){
    this.pending.set(kind,{value,revision:++this.revision});
    // A throttle, not a trailing debounce: dragging never postpones all work.
    if(!this.running&&!this.timer)this.timer=setTimeout(()=>{this.timer=null;this.drain()},this.delay);
  }
  get idle(){return !this.running&&!this.timer&&!this.pending.size}
  async drain(){
    if(this.running)return;
    this.running=true;
    try{
      while(this.pending.size){
        const [kind,item]=this.pending.entries().next().value;this.pending.delete(kind);
        const started=performance.now();
        try{
          const data=await this.request(kind,item.value,item.revision);
          await this.apply(kind,data,item.revision);
          this.onApplied(kind,{milliseconds:performance.now()-started,revision:item.revision,pending:this.pending.has(kind)});
        }catch(error){this.onError(kind,error,this.pending.has(kind))}
      }
    }finally{this.running=false;if(!this.pending.size)this.onIdle();else this.drain()}
  }
}
