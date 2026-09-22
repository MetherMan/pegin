"""One compiler for the live preview and the installed native skill resources.

Only these four private skills are edited. Server damage, hit counts and AoE
remain authoritative and are not changed by the visual-distance control.
"""
from pathlib import Path
import array, copy, io, json, math, re, struct, wave

DEFAULTS = {
    'meteor': dict(portalGap=80, readyHold=250, shotGap=80, flightSpeed=1800, impactHeight=.06),
    'frost': dict(startDelay=350, stepGap=45, distance=12, size=1.2, riseSpeed=1.5, brightness=1.3, spreadAngle=25, mistStrength=1.3),
    'heaven': dict(startDelay=1750, shotGap=85, flightSpeed=3000, impactSize=1.8),
    'six': dict(startDelay=1750, shotGap=70, flightSpeed=3000),
}
# label, minimum, maximum, step, unit. Also used to build the editor controls.
FIELDS = {
    'meteor': {
        'portalGap': ['마법진 생성 간격',40,250,10,'ms'],
        'readyHold': ['모두 생성 후 발사 대기',180,1000,10,'ms'],
        'shotGap': ['포탄 연사 간격',30,250,10,'ms'],
        'flightSpeed': ['포탄 비행 속도',600,4000,100,''],
        'impactHeight': ['적중 불꽃 높이',-.5,2,.01,'발밑 기준'],
    },
    'frost': {
        'startDelay': ['첫 얼음 발생 시점',100,1500,10,'ms'],
        'stepGap': ['얼음 전개 간격',20,200,5,'ms'],
        'distance': ['얼음이 뻗는 거리',7,20,.5,''],
        'spreadAngle': ['좌우로 벌어지는 각도',10,45,1,'도'],
        'size': ['얼음기둥 크기',1,2.5,.1,'배'],
        'riseSpeed': ['기둥 솟는 속도',.5,3,.1,'배'],
        'brightness': ['이펙트 밝기',.5,3,.1,'배'],
        'mistStrength': ['소환 안개·파동 선명도',.3,3,.1,'배'],
    },
    'heaven': {
        'startDelay': ['첫 화살 발사 시점',500,2200,10,'ms'],
        'shotGap': ['3연사 간격',30,250,5,'ms'],
        'flightSpeed': ['화살 비행 속도',1500,5000,100,''],
        'impactSize': ['적중 이펙트 크기',.5,3,.1,'배'],
    },
    'six': {
        'startDelay': ['첫 화살 발사 시점',500,2200,10,'ms'],
        'shotGap': ['6연사 간격',30,250,5,'ms'],
        'flightSpeed': ['화살 비행 속도',1500,5000,100,''],
    },
}

def validate_settings(value):
    if not isinstance(value,dict) or set(value)!=set(DEFAULTS):
        raise ValueError('네 스킬의 조절값이 필요합니다.')
    out=copy.deepcopy(value)
    for kind, fields in FIELDS.items():
        if not isinstance(out[kind],dict) or set(out[kind])!=set(fields):
            raise ValueError('잘못된 스킬 조절 항목입니다: '+kind)
        for key,(label,lo,hi,step,unit) in fields.items():
            n=out[kind][key]
            if type(n) not in (int,float) or not math.isfinite(n) or not lo<=n<=hi:
                raise ValueError(f'{label}: {lo}~{hi} 범위로 입력해 주세요.')
            if key not in ('distance','size','riseSpeed','brightness','impactSize','mistStrength','impactHeight'):
                if n!=int(n): raise ValueError(label+': 정수로 입력해 주세요.')
                out[kind][key]=int(n)
    return out

def load_settings(here):
    p=Path(here)/'tuning.json'
    value=json.loads(p.read_text()) if p.exists() else copy.deepcopy(DEFAULTS)
    return upgrade_settings(value)

def upgrade_settings(value):
    value=copy.deepcopy(value)
    # Upgrade the previous editor schema without resetting any saved controls.
    if isinstance(value,dict) and isinstance(value.get('frost'),dict):
        for key in ('spreadAngle','mistStrength'):value['frost'].setdefault(key,DEFAULTS['frost'][key])
    if isinstance(value,dict) and isinstance(value.get('meteor'),dict):
        value['meteor'].setdefault('impactHeight',DEFAULTS['meteor']['impactHeight'])
    return validate_settings(value)

def meteor_sounds(read):
    """Separate the original combined Fireball B cue at its impact onset.

    Keep original mono PCM/rate/pitch. Trim the lead-in and add short edge
    fades; lower each voice for seven overlapping projectiles.
    """
    with wave.open(io.BytesIO(read('Sound/mb_0004_B.wav')),'rb') as source:
        assert source.getnchannels()==1 and source.getsampwidth()==2
        rate=source.getframerate();pcm=array.array('h',source.readframes(source.getnframes()))
    result={}
    for name,start,end,gain in [('mm_meteor_launch.wav',.12,.98,.5),('mm_meteor_hit.wav',1.0,2.20,.4)]:
        samples=pcm[round(start*rate):round(end*rate)]
        attack=round(.003*rate);release=round(.03*rate)
        for i,v in enumerate(samples):
            fade=min(1,i/attack,(len(samples)-1-i)/release)
            samples[i]=round(v*gain*fade)
        buf=io.BytesIO()
        with wave.open(buf,'wb') as target:
            target.setparams((1,2,rate,0,'NONE','not compressed'));target.writeframes(samples.tobytes())
        result['Sound/'+name]=buf.getvalue()
    return result

def resource_reader(client,mirror=None):
    roots=[Path(client)]
    if mirror: roots.append(Path(mirror))
    if Path(client).name=='client-overlay': roots.append(Path(client).parent/'runtime/client/GameClient')
    def read(rel):
        for root in roots:
            p=root/rel
            if p.is_file(): return p.read_bytes()
        raise FileNotFoundError('게임 원본 파일이 없습니다: '+rel)
    return read

def compile_resources(read, settings, colors):
    cfg=validate_settings(settings);out={};meta={}
    out.update(meteor_sounds(read))
    def wed(source,name,color=None,scale=1,brightness=1,speed=1,portal=None,origin=None):
        raw=read('Effect/'+source);at=31;names=[]
        for _ in range(2):
            n=struct.unpack_from('<i',raw,at)[0];at+=4
            names.append(raw[at:at+n].rstrip(b'\0').decode('cp949'));at+=n
        start,end,count,cols,rows=struct.unpack_from('<5i',raw,at);at+=20
        frames=[list(struct.unpack_from('<11fIi',raw,at+i*52)) for i in range(count)]
        assert at+count*52==len(raw)
        # The private grayscale texture retains the source texture's luminance.
        if color:
            prefix=name.split('_')[0]
            names[1]=prefix+'_'+Path(names[1]).stem+'.bmp'
        if portal:
            fade,fade_ms=portal;last=math.ceil((fade+fade_ms)*.03)+2;resampled=[]
            for i in range(last+1):
                f=frames[min(i,33)].copy();t=i*1000/30
                if t>=fade:
                    source_frame=33+8*min(1,(t-fade)/fade_ms)
                    a=min(40,int(source_frame));b=min(40,a+1);mix=source_frame-int(source_frame)
                    f[:11]=[frames[a][k]*(1-mix)+frames[b][k]*mix for k in range(11)]
                    lum=round((frames[a][11]&255)*(41-source_frame)) if source_frame>=40 else round((frames[a][11]&255)*(1-mix)+(frames[b][11]&255)*mix)
                    f[11]=(f[11]&0xff000000)|lum*0x010101
                f[6]=i*4.5;resampled.append(f)
            frames=resampled;start=0;end=last
        elif speed!=1:
            # Resample the actual rise/fade frames, including the original last
            # frame. Never let a short WED wrap and flash again during its PART.
            last=max(2,math.ceil(end/speed))
            frames=[frames[min(end,round(i*speed))].copy() for i in range(last+1)]
            start=0;end=last
        rgb=[int(color[i:i+2],16) for i in (1,3,5)] if color else None
        for f in frames:
            if origin is not None:f[:3]=origin
            for i in (8,9,10):f[i]*=scale
            if rgb:
                lum=max((f[11]>>16)&255,(f[11]>>8)&255,f[11]&255)/255
                c=[min(255,round(v*lum*brightness)) for v in rgb]
                f[11]=(f[11]&0xff000000)|(c[0]<<16)|(c[1]<<8)|c[2]
        def string(s):
            b=s.encode('cp949')+b'\0';return struct.pack('<i',len(b))+b
        out['Effect/'+name]=raw[:31]+b''.join(map(string,names))+struct.pack('<5i',start,end,len(frames),cols,rows)+b''.join(struct.pack('<11fIi',*f) for f in frames)
        return name
    def part(s): return '[PART]\n'+s+'\n[/PART]\n'
    def effect(spe,parts,loop=False,trace=None):
        return '[EFFECT]\n'+(f'[SPE] {spe}\n' if spe else '')+('[LOOP]\n' if loop else '')+(f'[TRACE] {trace}\n' if trace else '')+parts+'[/EFFECT]\n'
    def sound(name,at=0,pos=None):
        # Use native WAVs or the separately trimmed original meteor cues.
        # The absent blue-arrow cast WAV uses the existing arrow-family sound.
        if 'Sound/'+name not in out:read('Sound/'+name)
        position='' if pos is None else '[POS] '+' '.join(map(str,pos))+'\n'
        return effect(None,part(position+f'[STARTTIME] {at}\n[SOUND] 0 {name} 3\n[TIMELIMIT] 3000'))
    def blow(sound_name=None):
        return '[BLOWTIMING]\n[STARTTIME] 0\n[TYPE] 0\n'+(f'[SOUND] {sound_name}\n' if sound_name else '')+'[/BLOWTIMING]'
    def emit(key,s):
        # Native ReadEffect repeats its previous keyword on blank lines.
        text=f'[NAME] {key}\n'+s+'[/NAME]'
        out['Magic/'+key+'.ms']=('\r\n'.join(l for l in text.splitlines() if l.strip())+'\r\n').encode('cp949')
    for kind,src in [('meteor','mb_0004_A'),('frost','mb_0009_A')]:
        text=read('Magic/'+src+'.ms').decode('cp949')
        for w in set(re.findall(r'\[SPE\]\s+(\S+)',text)):
            new=('mm_' if kind=='meteor' else 'mf_')+w
            wed(w,new,colors[kind],1.35 if kind=='meteor' else cfg['frost']['size'])
            text=text.replace(w,new)
        inner=text.split('\n',1)[1].rsplit('[/NAME]',1)[0]
        emit('mt_'+kind+'A',inner)
    m=cfg['meteor'];first=6*m['portalGap']+m['readyHold'];last=first+6*m['shotGap'];fade=last+300;end=fade+500
    ball=wed('fireball01.wed','mm_fireball01.wed',colors['meteor'],1.4)
    hit=wed('ta_fire.wed','mm_ta_fire.wed',colors['meteor'],1.4)
    s=sound('mb_0004_A.wav')
    for i,x in enumerate([0,1,-1,2,-2,3,-3]):
        delay=i*m['portalGap'];xx=round(x*1.9,2);z=4.1 if abs(x)%2==0 else 3.1
        circle=wed('fireball02.wed','mm_fireball02'+(f'_{i}' if i else '')+'.wed',colors['meteor'],1.68,portal=(fade-delay,500))
        s+=effect(circle,part(f'[POS] {xx} 0.5 {z}\n[STARTTIME] {delay}\n[TIMELIMIT] {end-delay}'))
        # A separate native sound object per shot lets the launch tail finish
        # independently of its arrival-triggered impact sound.
        s+=sound('mm_meteor_launch.wav',first+i*m['shotGap'],(xx,.5,z))
        p=part(f'[ATTACK]\n[POS] {xx} 0.5 {z}\n[TARGET] 0 0 1\n[MOVETYPE] 1\n[STARTTIME] {first+i*m["shotGap"]}\n[TIMELIMIT] 0\n[SPEED] {m["flightSpeed"]}')
        # Leave the projectile's body-height hit intact, but anchor its fire
        # marker relative to the target's feet. NextStep applies POS to the
        # previous TARGET (+1); ENEMY then uses the absolute target offset.
        height=m['impactHeight'];entry_offset=round(height-1,6)
        p+=part(f'[NOSHOW]\n[ENEMY]\n[POS] 0 0 {entry_offset:g}\n[TARGET] 0 0 {height:g}\n'+blow('mm_meteor_hit.wav')+f'\n[TAIL] {hit}\n[TIMELIMIT] 500')
        # Keep the native sound object alive for the whole 1.2 s impact cue,
        # while the visual explosion retains its original 500 ms lifetime.
        p+=part('[NOSHOW]\n[TIMELIMIT] 800')
        s+=effect(ball,p,True)
    emit('mt_meteorB',s);meta['meteor']=dict(first=first,last=last,fadeStart=fade,fadeEnd=end,motionEnd=1500,impactHeight=m['impactHeight'])
    f=cfg['frost'];ice=wed('spikeice02.wed','mf_spikeice02.wed',colors['frost'],f['size'],f['brightness'],f['riseSpeed'])
    small_ice=wed('spikeice01.wed','mf_spikeice01.wed',colors['frost'],f['size'],f['brightness'],f['riseSpeed'])
    ripple=wed('pajang02.wed','mf_pajang02.wed',colors['frost'],f['size'],f['brightness'],f['riseSpeed'])
    # Restore the source spell's smaller, soft-edged summoning waves. Their
    # own ground position avoids inheriting the buried ice mesh's negative Z.
    mist_color='#'+''.join(f'{round(int(colors["frost"][i:i+2],16)*.65+255*.35):02X}' for i in (1,3,5))
    mist=wed('pajang03.wed','mf_pajang03.wed',mist_color,f['size']*.75,f['mistStrength'],f['riseSpeed'],origin=(0,0,0))
    s=sound('mb_0009_A.wav')+sound('mb_0009_B.wav',f['startDelay'])
    # Restore the original alternation of thin spikes and clusters and their
    # emergence heights. Repeating the largest cluster at every station hides
    # the silhouette under additive overlap. Original reach was seven units.
    stations=6;life=max(350,math.ceil(1000/f['riseSpeed']))
    for angle in (0,-f['spreadAngle'],f['spreadAngle']):
        a=math.radians(angle)
        for i in range(stations):
            y=[1,2,3,4,5,7][i]*f['distance']/7;x=-math.sin(a)*y;yy=math.cos(a)*y;z=[-1.8,-1,-1.8,-1,-.3,.5][i]*f['size']
            at=f['startDelay']+i*f['stepGap']
            p=part(f'[POS] {x:.5f} {yy:.5f} {z:.5f}\n[STARTTIME] {at}\n[TIMELIMIT] {life}')
            if i==stations-1:p+=part(blow()+'\n[TIMELIMIT] 10')
            s+=effect(small_ice if i in (0,1,3) else ice,p)
            if i in (1,3,4,5):
                s+=effect(ripple if i==5 else mist,part(f'[POS] {x:.5f} {yy:.5f} 0.06\n[STARTTIME] {at}\n[TIMELIMIT] {life}'))
    emit('mt_frostB',s);meta['frost']=dict(first=f['startDelay'],last=f['startDelay']+(stations-1)*f['stepGap'],distance=f['distance'],spreadAngle=f['spreadAngle'],stations=stations,motionEnd=1500)
    hw=wed('ps0002_1.wed','mh_ps0002_1.wed','#72DFFF',1.15)
    # vb0001 embeds Y=+1. Native rendering scales this translation too, so
    # enlarging the impact lifted it above the projectile's hit point.
    ht=wed('vb0001.wed','mh_vb0001.wed','#72DFFF',1.15*cfg['heaven']['impactSize'],origin=(0,0,0))
    for kind,key,n,arrow,tail,trace in [('heaven','mt_heaven',3,hw,ht,'mt_arrow_trace'),('six','mt_six',6,'arrow01.wed','ta_arrow01.wed','활'),('six','mt_six_blue',6,'mt_arrow.wed','mt_arrow_hit.wed','mt_arrow_trace')]:
        v=cfg[kind];s='';target_height=0.9 if kind=='heaven' else 1.1
        for i in range(n):
            at=v['startDelay']+i*v['shotGap'];s+=sound('arrow100048.wav',at)
            p=part(f'[NOSHOW]\n[BONE] 14\n[STARTTIME] {at}\n[TIMELIMIT] 5')
            p+=part(f'[ATTACK]\n[POS] 0 0 1.3\n[TARGET] 0 0 {target_height}\n[MOVETYPE] 1\n[TIMELIMIT] 0\n[SPEED] {v["flightSpeed"]}\n[TRACEDIST] 0.05')
            p+=part('[NOSHOW]\n'+blow('attack100049.wav')+f'\n[TAIL] {tail}\n[TIMELIMIT] 500')
            s+=effect(arrow,p,True,trace)
        emit(key,s);meta[kind]=dict(first=v['startDelay'],last=v['startDelay']+(n-1)*v['shotGap'],motionEnd=2500)
    out['Tools/SkillColors/tuning.json']=(json.dumps(cfg,indent=2)+'\n').encode()
    out['Tools/SkillColors/colors.json']=(json.dumps(colors,indent=2)+'\n').encode()
    return out,meta
