"""Register the four level-140 skills and compile native magic resources."""
from pathlib import Path
import re,json,struct,zlib,sys,math,io
R=Path(__file__).resolve().parents[1];O=R/'assets/skills140';D=R/'client-overlay';C=R/'runtime/client/GameClient'
sys.path.insert(0,str(R/'runtime/pylibs'))
SPECS=[
 dict(id=19120,name='헤븐 페너트레이션',key='mt_heaven',peer=12048,branch=1,hits=3,area=5,mp=140,cool=8,damage=[560,620]),
 dict(id=19121,name='식스폴드 샷',key='mt_six',peer=12045,branch=1,hits=6,area=0,mp=150,cool=9,damage=[380,420]),
 dict(id=19122,name='메테오 버스트',key='mt_meteor',peer=10106,branch=2,hits=7,area=6,mp=320,cool=4,damage=[550,610]),
 dict(id=19123,name='아일랜드 오브 코시투스',key='mt_frost',peer=10111,branch=2,hits=3,area=6,mp=280,cool=4,damage=[1120,1230])]
def read(p):return p.read_bytes().decode('cp949')
def write(p,s):p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes((s.rstrip()+'\r\n').replace('\r\r','\r').encode('cp949'))
def rows(s):return {int(r[0]):r for l in s.splitlines() if (r:=l.split('\t'))[0].isdigit()}
def merge(s,values):
 ids={int(r[0]) for r in values};ls=[l for l in s.splitlines() if not(l.split('\t')[0].isdigit() and int(l.split('\t')[0]) in ids)]
 at=max(i for i,l in enumerate(ls) if l.split('\t')[0].isdigit())+1;ls[at:at]=['\t'.join(r) for r in values]
 return '\r\n'.join(ls)+'\r\n'
def register():
 originals=rows(read(R/'game-data/DATA/ITEM_DATA.txt'));new=[]
 for s in SPECS:
  r=originals[s['peer']].copy();r[0]=str(s['id']);r[2]=s['name'];r[3]=s['key']
  # Scrolls use the existing branch icon and remain ordinary consumable skills.
  new.append(r)
 for p in [R/'game-data/DATA/ITEM_DATA.txt',D/'Item/ITEM.dat']:write(p,merge(read(p),new))
 p=D/'Interface/item.dat';b=p.read_bytes();ui=[r[:5]+['0']+r[5:] for r in new]
 raw=merge(zlib.decompress(b[20:]).decode('cp949'),ui).encode('cp949');p.write_bytes(b[:16]+struct.pack('<I',len(raw))+zlib.compress(raw,9))
 server=rows(read(R/'game-data/DATA/skill_data.txt'));byitem={int(r[3]):r for r in server.values()};skillrows=[]
 used={int(r[25]) for r in server.values() if int(r[3]) not in [s['id'] for s in SPECS]};assert not used.intersection(range(124,128))
 for i,s in enumerate(SPECS):
  r=byitem[s['peer']].copy();r[0]=str(100030001+i);r[1]=s['name'];r[2]=s['key'];r[3]=str(s['id'])
  r[8]=str(2 if s['area'] else 0);r[9:14]=['0']*5;r[9+s['branch']]='140';r[14:16]=map(str,s['damage']);r[20]=str(s['mp']);r[22]='11' if s['branch']==1 else '10'
  r[24:27]=map(str,[s['cool'],124+i,s['area']]);r[28]='0';r[29]=s['name'];skillrows.append(r)
 write(R/'game-data/DATA/skill_data.txt',merge(read(R/'game-data/DATA/skill_data.txt'),skillrows))
 client=[]
 for s,r in zip(SPECS,skillrows):
  scope=f"대상 중심 범위 {s['area']}" if s['area'] else '단일 대상'
  c=r[:30]+[f"{s['hits']}회 연속 공격.",f"MP {s['mp']} / 재사용 {s['cool']}초",('원거리' if s['branch']==1 else '흑마법')+' 140',scope];client.append(c)
 write(D/'Item/Skill.dat',merge(read(D/'Item/Skill.dat' if (D/'Item/Skill.dat').exists() else C/'Item/Skill.dat'),client))
 p=D/'Interface/skill.dat';b=(p if p.exists() else C/'Interface/skill.dat').read_bytes()
 raw=merge(zlib.decompress(b[20:]).decode('cp949'),client).encode('cp949');p.write_bytes(b[:16]+struct.pack('<I',len(raw))+zlib.compress(raw,9))
 anim=read(D/'Skill/Skill.dat' if (D/'Skill/Skill.dat').exists() else C/'Skill/Skill.dat')
 anim=re.sub(r'\[SKILL\]\s*\[NAME\]\s+mt_(?:heaven|six(?:_blue)?)\b.*?\[/SKILL\]','',anim,flags=re.S).replace('[END]','').rstrip()
 for key in ['mt_heaven','mt_six','mt_six_blue']:
  anim+=f'\n[SKILL]\n[NAME] {key}\n[WEAPON] 1\n[EFFECT] {key}\n[MALE_ACTION] arrow_m.ani\n[FEMALE_ACTION] arrow_f.ani\n[/SKILL]\n'
 write(D/'Skill/Skill.dat',anim+'\n[END]')
 # Existing GM warehouse search and branch shelves expose the new scrolls.
 p=R/'src/server/utf8/gm_catalog_data.utf8.h';s=p.read_text(encoding='utf-8')
 s=re.sub(r'\{1912[0-3],.*?\},\n','',s)
 lines=s.splitlines()
 for group in [4,5]:
  at=max(i for i,l in enumerate(lines) if f',3,{group},' in l)+1
  values=[v for v in SPECS if (4 if v['branch']==1 else 5)==group]
  entries=['{%d,"%s",3,%d,%d},'%(v['id'],v['name'],group,(13 if group==4 else 36)+i) for i,v in enumerate(values)]
  lines[at:at]=entries
 s='\n'.join(lines)+'\n';p.write_text(s,encoding='utf-8');(R/'src/server/LAQIA_GameServer/gm_catalog_data.h').write_bytes(s.encode('cp949'))
 (O/'skills.json').write_text(json.dumps(SPECS,ensure_ascii=False,indent=2),encoding='utf-8')

def effects(config=None):
 from PIL import Image
 cfg=config or json.loads((O/'colors.json').read_text());report=[];magics={}
 def texture(name,new):
  p=C/'Texture/Effect'/Path(name).with_suffix('.wtm');raw=p.read_bytes();bmp=bytearray(zlib.decompress(raw[13:]));off=struct.unpack_from('<I',bmp,10)[0];w,h=struct.unpack_from('<ii',bmp,18);bits=struct.unpack_from('<H',bmp,28)[0]
  assert bits in (24,32) and struct.unpack_from('<I',bmp,30)[0]==0
  stride=((w*bits+31)//32)*4
  for y in range(abs(h)):
   for x in range(w):
    i=off+y*stride+x*(bits//8);value=max(bmp[i:i+3]);bmp[i:i+3]=bytes([value]*3)
  p=D/'Texture/Effect'/Path(new).with_suffix('.wtm');p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes(raw[:9]+struct.pack('<I',len(bmp))+zlib.compress(bmp,9))
 def wed(old,prefix,color,scale=1,fade_start_ms=None,fade_ms=500,variant=''):
  target=prefix+'_'+Path(old).stem+variant+'.wed';p=D/'Effect'/target;raw=(C/'Effect'/old).read_bytes();at=31;strings=[]
  for _ in range(2):n=struct.unpack_from('<i',raw,at)[0];at+=4;strings.append(raw[at:at+n].rstrip(b'\0').decode('cp949'));at+=n
  tex=prefix+'_'+Path(strings[1]).stem+'.bmp';texture(strings[1],tex)
  def string(s):b=s.encode('cp949')+b'\0';return struct.pack('<i',len(b))+b
  data=bytearray(raw[:31]+string(strings[0])+string(tex)+raw[at:]);pos=31+4+len(strings[0].encode('cp949'))+1+4+len(tex)+1
  _,_,count,_,_=struct.unpack_from('<5i',data,pos);pos+=20;assert pos+52*count==len(data)
  if fade_start_ms is not None:
   # Each delayed portal keeps its original formation/rotation, but shares the
   # same global fade window. Resample the native 33..40 fade, then reach black.
   # Pad past the PART lifetime so native CheckCount cannot wrap to frame zero.
   original=[data[pos+i*52:pos+(i+1)*52] for i in range(count)]
   end=math.ceil((fade_start_ms+fade_ms)*30/1000)+2;frames=[]
   for i in range(end+1):
    frame=bytearray(original[min(i,33)])
    if i*1000/30>=fade_start_ms:
     source=33+8*min(1,(i*1000/30-fade_start_ms)/fade_ms)
     lo=min(40,math.floor(source));hi=min(40,lo+1);mix=source-math.floor(source)
     a=struct.unpack('<11fIi',original[lo]);b=struct.unpack('<11fIi',original[hi])
     values=[a[k]+(b[k]-a[k])*mix for k in range(11)]
     if source>=40:light=round((a[11]&255)*(41-source))
     else:light=round((a[11]&255)+((b[11]&255)-(a[11]&255))*mix)
     frame=bytearray(struct.pack('<11fIi',*values,(a[11]&0xff000000)|(light<<16)|(light<<8)|light,a[12]))
    struct.pack_into('<f',frame,24,i*4.5)
    frames.append(frame)
   struct.pack_into('<3i',data,pos-20,0,end,end+1)
   data=data[:pos]+b''.join(frames);count=end+1
  rgb=[int(color[i:i+2],16) for i in [1,3,5]]
  intensities=[]
  for j in range(count):
   o=pos+j*52
   # Native WED has pos xyz, axis/angle, homogeneous scale, axis scales, ARGB, texture frame.
   for k in [32,36,40]:v=struct.unpack_from('<f',data,o+k)[0];struct.pack_into('<f',data,o+k,v*scale)
   v=struct.unpack_from('<I',data,o+44)[0];brightness=max((v>>16)&255,(v>>8)&255,v&255)/255
   intensities.append(round(255*brightness))
   c=[round(k*brightness) for k in rgb];struct.pack_into('<I',data,o+44,(v&0xff000000)|(c[0]<<16)|(c[1]<<8)|c[2])
  if prefix in ['mm','mf']:
   template=bytearray(data)
   for j,value in enumerate(intensities):
    o=pos+j*52+44;alpha=struct.unpack_from('<I',template,o)[0]&0xff000000;struct.pack_into('<I',template,o,alpha|(value<<16)|(value<<8)|value)
   tp=D/'Tools/SkillColors/templates'/target;tp.parent.mkdir(parents=True,exist_ok=True);tp.write_bytes(template)
  p.write_bytes(data);report.append(dict(source=old,target=target,frames=count,color=color,scale=scale));return target
 def emit(key,s):
  # ReadEffect retains its last keyword on an empty line. In particular, a
  # blank after [/PART] re-enters ReadPart and swallows the remaining script.
  script=f'[NAME] {key}\n'+s+'\n[/NAME]'
  write(D/'Magic'/f'{key}.ms','\r\n'.join(line for line in script.splitlines() if line.strip()))
  magics[key]=key+'.ms'
 def effect(wed,parts,loop=False):return '[EFFECT]\n'+(f'[SPE] {wed}\n' if wed else '')+('[LOOP]\n' if loop else '')+parts+'\n[/EFFECT]\n'
 def part(s):return '[PART]\n'+s+'\n[/PART]\n'
 def blow():return '[BLOWTIMING]\n[STARTTIME] 0\n[TYPE] 0\n[SOUND] attack100049.wav\n[/BLOWTIMING]'
 # Heaven uses the same penetrating energy-arrow mesh as Death Penetration.
 hw=wed('ps0002_1.wed','mh','#72DFFF',1.15);ht=wed('vb0001.wed','mh','#72DFFF',1.15)
 for key,n,arrow,tail,trace in [('mt_heaven',3,hw,ht,'mt_arrow_trace'),('mt_six',6,'arrow01.wed','ta_arrow01.wed','활'),('mt_six_blue',6,'mt_arrow.wed','mt_arrow_hit.wed','mt_arrow_trace')]:
  s=''
  for i in range(n):
   parts=part(f'[NOSHOW]\n[BONE] 14\n[STARTTIME] {2000+i*140}\n[TIMELIMIT] 5')
   parts+=part('[ATTACK]\n[POS] 0 0 1.3\n[TARGET] 0 0 1.1\n[MOVETYPE] 1\n[TIMELIMIT] 0\n[SPEED] 2500\n[TRACEDIST] 0.05')
   parts+=part('[NOSHOW]\n'+blow()+f'\n[TAIL] {tail}\n[TIMELIMIT] 500')
   s+=effect(arrow,parts,True).replace('[EFFECT]\n',f'[EFFECT]\n[TRACE] {trace}\n',1)
  emit(key,s)
 # Copy the established cast animation type; only its private resources change.
 for key,original,prefix,color,scale in [('mt_meteor','mb_0004_A.ms','mm',cfg['meteor'],1.35),('mt_frost','mb_0009_A.ms','mf',cfg['frost'],1.2)]:
  s=read(C/'Magic'/original);s=re.sub(r'\[NAME\]\s+\S+',f'[NAME] {key}A',s)
  for w in set(re.findall(r'\[SPE\]\s+(\S+)',s)):s=s.replace(w,wed(w,prefix,color,scale))
  write(D/'Magic'/f'{key}A.ms',s);magics[key+'A']=key+'A.ms'
 ball=wed('fireball01.wed','mm',cfg['meteor'],1.4);hit=wed('ta_fire.wed','mm',cfg['meteor'],1.4)
 s='';portal_positions=[]
 for i,x in enumerate([0,1,-1,2,-2,3,-3]):
  delay=i*150;xx=round(x*1.9,2);z=4.1 if abs(x)%2==0 else 3.1
  portal_positions.append([xx,.5,z])
  circle=wed('fireball02.wed','mm',cfg['meteor'],1.4*1.2,fade_start_ms=2700-delay,variant=f'_{i}' if i else '')
  s+=effect(circle,part(f'[POS] {xx} 0.5 {z}\n[STARTTIME] {delay}\n[TIMELIMIT] {3200-delay}'))
  parts=part(f'[ATTACK]\n[POS] {xx} 0.5 {z}\n[TARGET] 0 0 1\n[MOVETYPE] 1\n[STARTTIME] {delay+1500}\n[TIMELIMIT] 0\n[SPEED] 1000')
  parts+=part('[NOSHOW]\n'+blow()+f'\n[TAIL] {hit}\n[TIMELIMIT] 500')
  s+=effect(ball,parts,True)
 emit('mt_meteorB',s)
 # Positions are rotated in the caster's local XY plane, producing +/-45 degree arms.
 ice=wed('spikeice02.wed','mf',cfg['frost'],1.2);ripple=wed('pajang02.wed','mf',cfg['frost'],1.2);s=''
 for branch,angle in enumerate([0,-45,45]):
  a=math.radians(angle)
  for i,y in enumerate([1,2,3,4,5,6]):
   x=-math.sin(a)*y;yy=math.cos(a)*y
   parts=part(f'[POS] {x:.5f} {yy:.5f} 0\n[STARTTIME] {200+i*100}\n[TIMELIMIT] 1000\n[TAIL] {ripple}')
   if i==5:parts+=part(blow()+'\n[TIMELIMIT] 10')
   s+=effect(ice,parts)
 emit('mt_frostB',s)
 p=D/'Magic/MagicList.ml';s=read(p if p.exists() else C/'Magic/MagicList.ml')
 s='\n'.join(l for l in s.splitlines() if not re.match(r'\[MAGIC\]\s+mt_(?:heaven|six|meteor|frost)',l))
 write(p,s+'\n'+'\n'.join(f'[MAGIC] {k}\t{v}' for k,v in magics.items()))
 (O/'effect-build.json').write_text(json.dumps(dict(resources=report,magic=magics,meteor_offsets=[0,1,-1,2,-2,3,-3],meteor_portal_positions=portal_positions,meteor_first_shot_ms=1500,meteor_portals_fade_start_ms=2700,meteor_portals_end_ms=3200,meteor_portal_relative_scale=1.2,frost_angles=[0,-45,45]),indent=2))
 (D/'Tools/SkillColors/colors.json').write_text(json.dumps(cfg,indent=2)+'\n')
 # Finish with the same compiler used by the local editor. Rebuilding assets
 # must preserve saved timings instead of silently reinstating old constants.
 sys.path.insert(0,str(D/'Tools/SkillColors'))
 from tuning import compile_resources,load_settings,resource_reader
 tuning=load_settings(D/'Tools/SkillColors')
 compiled,timing=compile_resources(resource_reader(D),tuning,cfg)
 for rel,data in compiled.items():
  target=D/rel;target.parent.mkdir(parents=True,exist_ok=True);target.write_bytes(data)
 (O/'tuning.json').write_bytes(compiled['Tools/SkillColors/tuning.json'])
 (O/'effect-timing.json').write_text(json.dumps(timing,indent=2)+'\n')
 (R/'game-data/DATA/SKILL140_DEATH.txt').write_bytes(('19122 %d\n19123 %d\n'%(timing['meteor']['finalImpact'],timing['frost']['pillarAt'])).encode('ascii'))
 from native_assets import read_wed
 final_resources=[dict(target=Path(rel).name,frames=len(read_wed(data)['frames'])) for rel,data in compiled.items() if rel.startswith('Effect/') and rel.lower().endswith('.wed')]
 (O/'effect-build.json').write_text(json.dumps(dict(resources=final_resources,magic=magics,tuning=tuning,timing=timing,meteor_portal_positions=portal_positions,frost_angles=[0,-tuning['frost']['spreadAngle'],tuning['frost']['spreadAngle']]),indent=2)+'\n')
 print('Built',len(magics),'magic scripts and',len(final_resources),'private WED resources using saved tuning')

if __name__=='__main__':
 O.mkdir(exist_ok=True)
 if not (O/'colors.json').exists():(O/'colors.json').write_text(json.dumps(dict(meteor='#FF8048',frost='#72DFFF'),indent=2))
 if '--effects-only' not in sys.argv:register()
 effects()
 from build_skill140_scroll_icons import main as build_scroll_icons
 build_scroll_icons()
 from build_skill140_icons import main as build_skill_icons
 build_skill_icons()
