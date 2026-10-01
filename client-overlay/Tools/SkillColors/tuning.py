"""One compiler for the live preview and the installed native skill resources.

Only these four private skills are edited. Server damage, hit counts and AoE
remain authoritative and are not changed by the visual-distance control.
"""
from pathlib import Path
import array, copy, io, json, math, re, struct, wave, zlib

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
        'shotGap': ['운석 낙하 간격',30,250,10,'ms'],
        'flightSpeed': ['운석 낙하 속도',600,4000,100,''],
        'impactHeight': ['적중 불꽃 높이',-.5,2,.01,'발밑 기준'],
    },
    'frost': {
        'startDelay': ['첫 얼음 발생 시점',100,1500,10,'ms'],
        'stepGap': ['얼음 전개 간격',20,200,5,'ms'],
        'distance': ['얼음 포위 지름',7,20,.5,''],
        'spreadAngle': ['포위 얼음 회전 배치',10,45,1,'도'],
        'size': ['얼음기둥 크기',1,2.5,.1,'배'],
        'riseSpeed': ['기둥 솟는 속도',.5,3,.1,'배'],
        'brightness': ['이펙트 밝기',.5,3,.1,'배'],
        'mistStrength': ['충격파·눈발 광량',.3,3,.1,'배'],
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

# Additional visual controls migrate independently, preserving every saved value.
_VISUAL_FIELDS = {
    'meteor': {
        'brightness': ['전체 광량', .2, 3, .05, '배'],
        'intensity': ['보조층·잔광 진하기', .25, 2.5, .05, '배'],
        'projectileSize': ['포격 화염탄 크기', .35, 3, .05, '배'],
        'finalProjectileSize': ['최종 화염탄 크기', .35, 3, .05, '배'],
        'finalFlightSpeed': ['최종 낙하 속도 배율', .1, 5, .05, '클수록 빠름 · 일반 탄속 대비'],
        'finalSealDelay': ['포격 종료 → 최종 마법진 대기', 0, 4000, 10, 'ms'],
        'finalFallDelay': ['최종 마법진 → 거대 메테오 낙하 대기', 250, 4000, 10, 'ms'],
        'impactSize': ['포격 폭발 크기', .35, 3, .05, '배'],
        'finalImpactSize': ['최종 폭풍 폭발 크기', .35, 2.5, .05, '배'],
        'portalSize': ['포격 마법진 크기', .35, 2.5, .05, '배'],
        'finalPortalSize': ['최종 마법진 크기', .35, 2.5, .05, '배'],
        'portalHeight': ['포격 마법진 높이', -5, 12, .1, '기존 높이에서'],
        'finalPortalHeight': ['최종 마법진 높이', -8, 15, .1, '기존 높이에서'],
        'portalOffsetX': ['포격 마법진 좌우', -10, 10, .1, '대상 기준'],
        'portalOffsetY': ['포격 마법진 앞뒤', -10, 10, .1, '대상 기준'],
        'finalPortalOffsetX': ['최종 마법진 좌우', -10, 10, .1, '대상 기준'],
        'finalPortalOffsetY': ['최종 마법진 앞뒤', -10, 10, .1, '대상 기준'],
        'portalSpread': ['포격 마법진 분산', .25, 2.5, .05, '배'],
    },
    'frost': {
        'intensity': ['마법진·잔광 진하기', .25, 2.5, .05, '배'],
        'iceOpacity': ['얼음 불투명도', 0, 100, 1, '% · 0=투명, 100=불투명'],
        'iceDensity': ['얼음 표면 진하기', .2, 2.5, .05, '배'],
        'mistAmount': ['얼음 안개량', 0, 3, .1, '배 · 0은 끄기'],
        'inwardStartRadius': ['외곽 시작 반경', .6, 2, .05, '포위 거리 배율'],
        'inwardEndRadius': ['안쪽 도착 반경', 0, .5, .05, '포위 거리 배율'],
        'sealSize': ['세 마법진 크기', .35, 2.5, .05, '배'],
        'sealLowerHeight': ['하단 마법진 높이', -.1, 5, .1, '기존 높이에서'],
        'sealMiddleHeight': ['중단 마법진 높이', -3, 10, .1, '기존 높이에서'],
        'sealUpperHeight': ['상단 마법진 높이', -5, 15, .1, '기존 높이에서'],
        'sealOffsetX': ['마법진 좌우', -8, 8, .1, '대상 기준'],
        'sealOffsetY': ['마법진 앞뒤', -8, 8, .1, '대상 기준'],
        'blizzardStrength': ['눈보라 입자·바람 밀도', .25, 2.5, .05, '배'],
    },
    'heaven': {
        'brightness': ['전체 광량', .2, 3, .05, '배'],
        'intensity': ['보조층·잔광 진하기', .25, 2.5, .05, '배'],
        'projectileSize': ['관통 화살 크기', .35, 3, .05, '배'],
        'launchSize': ['활 앞 발사 폭발 크기', .35, 4, .05, '배'],
        'impactHeight': ['적중 높이 보정', -1, 4, .05, '기존 높이에서'],
        'areaSize': ['몸 앞뒤 관통선 길이', .4, 3, .05, '배 · 피해 범위 유지'],
        'impactSize': ['기존 적중 이펙트 크기', .5, 3, .1, '배'],
        'pierceWidth': ['추가 관통선 굵기', .25, 4, .05, '배'],
    },
    'six': {
        'brightness': ['전체 광량', .2, 3, .05, '배'],
        'intensity': ['보조층·잔광 진하기', .25, 2.5, .05, '배'],
        'projectileSize': ['연사 화살 크기', .35, 3, .05, '배'],
        'launchSize': ['활 앞 발사 폭발 크기', .35, 4, .05, '배'],
        'impactSize': ['개별 적중 폭발 크기', .35, 4, .05, '배'],
        'impactHeight': ['적중 높이 보정', -1, 4, .05, '기존 높이에서'],
    },
}
_OFFSET_FIELDS = {'portalHeight', 'finalPortalHeight', 'portalOffsetX', 'portalOffsetY', 'finalPortalOffsetX', 'finalPortalOffsetY', 'sealLowerHeight', 'sealMiddleHeight', 'sealUpperHeight', 'sealOffsetX', 'sealOffsetY', 'impactHeight'}
for _kind, _fields in _VISUAL_FIELDS.items():
    FIELDS[_kind].update(_fields)
    for _key in _fields:
        DEFAULTS[_kind].setdefault(_key, 0 if _key in _OFFSET_FIELDS else 1)
DEFAULTS['frost'].update(inwardStartRadius=.95, inwardEndRadius=.10)
DEFAULTS['frost']['iceOpacity'] = 85
DEFAULTS['meteor'].update(finalSealDelay=220, finalFallDelay=650)


def validate_colors(value):
    """Upgrade the two saved spell colours without replacing either of them."""
    if not isinstance(value, dict) or not {'meteor', 'frost'} <= set(value) or not set(value) <= {'meteor', 'frost', 'frostIce'}:
        raise ValueError('메테오·마법진·얼음 몸체의 색상 값을 확인해 주세요.')
    if not all(isinstance(v, str) and re.fullmatch(r'#[0-9a-fA-F]{6}', v) for v in value.values()):
        raise ValueError('색상은 #RRGGBB 형식이어야 합니다.')
    result = {key: color.upper() for key, color in value.items()}
    result.setdefault('frostIce', '#2374D8')
    return result


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
            if step >= 1:
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
        value['meteor'].setdefault('finalFallDelay', max(650, value['meteor'].get('readyHold', 250)))
    if isinstance(value,dict) and isinstance(value.get('heaven'),dict):
        value['heaven'].setdefault('pierceWidth', value['heaven'].get('impactSize', 1))
    if isinstance(value, dict):
        for kind, fields in _VISUAL_FIELDS.items():
            if isinstance(value.get(kind), dict):
                for key in fields:
                    value[kind].setdefault(key, DEFAULTS[kind][key])
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

def compile_resources(read, settings, colors, only=None):
    cfg=validate_settings(settings);colors=validate_colors(colors);out={};meta={}
    selected=set(DEFAULTS if only is None else only)
    if not selected or not selected<=set(DEFAULTS):raise ValueError('잘못된 미리보기 스킬입니다.')
    if 'meteor' in selected:out.update(meteor_sounds(read))
    def grayscale(source,target):
        key='Texture/Effect/'+Path(target).with_suffix('.wtm').name
        if key in out:return
        raw=read('Texture/Effect/'+Path(source).with_suffix('.wtm').name)
        bmp=bytearray(zlib.decompress(raw[13:]));off=struct.unpack_from('<I',bmp,10)[0]
        width,height=struct.unpack_from('<ii',bmp,18);bits=struct.unpack_from('<H',bmp,28)[0]
        assert bmp[:2]==b'BM' and bits in (8,24,32) and struct.unpack_from('<I',bmp,30)[0]==0
        if bits==8:
            start=14+struct.unpack_from('<I',bmp,14)[0]
            for at in range(start,off,4):bmp[at:at+3]=bytes([max(bmp[at:at+3])])*3
        else:
            stride=((width*bits+31)//32)*4
            for y in range(abs(height)):
                for x in range(width):
                    at=off+y*stride+x*(bits//8);bmp[at:at+3]=bytes([max(bmp[at:at+3])])*3
        out[key]=raw[:9]+struct.pack('<I',len(bmp))+zlib.compress(bmp,9)
    def wed(source,name,color=None,scale=1,brightness=1,speed=1,portal=None,origin=None,transform=None):
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
            original_texture=names[1];names[1]=prefix+'_'+Path(names[1]).stem+'.bmp'
            grayscale(original_texture,names[1])
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
        if transform:
            names,frames=transform(names,frames);start=0;end=len(frames)-1
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
        if kind not in selected:continue
        text=read('Magic/'+src+'.ms').decode('cp949')
        for w in set(re.findall(r'\[SPE\]\s+(\S+)',text)):
            new=('mm_' if kind=='meteor' else 'mf_')+w
            wed(w,new,colors[kind],1.35 if kind=='meteor' else cfg['frost']['size'])
            text=text.replace(w,new)
        inner=text.split('\n',1)[1].rsplit('[/NAME]',1)[0]
        emit('mt_'+kind+'A',inner)
    from meteor_storm import build as build_meteor_storm
    # Native ATTACKMAGIC retains the last position after target removal. Without
    # it, a lethal hit discards every remaining layer, including the final blast.
    if 'meteor' in selected:
        storm,meta['meteor']=build_meteor_storm(read,out,wed,part,effect,sound,blow,cfg['meteor'],colors['meteor'])
        emit('mt_meteorB','[ATTACKMAGIC]\n'+storm)
    from cocytus import build as build_cocytus
    if 'frost' in selected:
        frost,meta['frost']=build_cocytus(read,out,wed,part,effect,sound,blow,cfg['frost'],colors['frost'],colors['frostIce'])
        emit('mt_frostB','[ATTACKMAGIC]\n'+frost)
    from archer_effects import build as build_archers
    if selected&{'heaven','six'}:meta.update(build_archers(read,out,wed,part,effect,sound,blow,cfg,emit,only=selected))
    out['Tools/SkillColors/tuning.json']=(json.dumps(cfg,indent=2)+'\n').encode()
    out['Tools/SkillColors/colors.json']=(json.dumps(colors,indent=2)+'\n').encode()
    return out,meta
