"""Exercise settings extremes, native loader, sound sources and preview parity."""
from pathlib import Path
import base64,copy,hashlib,io,json,math,struct,sys,wave
R=Path(__file__).resolve().parents[1];D=R/'client-overlay';H=D/'Tools/SkillColors'
sys.dont_write_bytecode=True;sys.path[:0]=[str(H),str(R/'tools')]
from tuning import DEFAULTS,FIELDS,validate_settings,compile_resources,resource_reader,load_settings,meteor_sounds,upgrade_settings
from native_assets import load_preview,read_magic,read_wed
from verify_magic_loader import parse
from native_actor import animation,motion_table

def main():
    read=resource_reader(D);colors=json.loads((H/'colors.json').read_text());saved=load_settings(H)
    variants=[saved]
    for limit in (1,2):variants.append({kind:{key:field[limit] for key,field in fields.items()} for kind,fields in FIELDS.items()})
    for cfg in variants:
        out,meta=compile_resources(read,cfg,colors)
        preview=load_preview(H,D,generated=out,metadata=meta,compact=True)
        for rel,data in out.items():
            if rel.startswith('Magic/'):
                counts=parse(data);expected={'mt_heaven':3,'mt_six':6,'mt_six_blue':6,'mt_meteorA':0,'mt_meteorB':7,'mt_frostA':0,'mt_frostB':3}[Path(rel).stem]
                assert counts['blows']==expected
                for e in read_magic(data.decode('cp949')):
                    for part in e['parts']:
                        for sound in ([part['SOUND']['name']] if 'SOUND' in part else [])+([part['HITSOUND']] if 'HITSOUND' in part else []):
                            relsound='Sound/'+sound
                            assert (out[relsound] if relsound in out else read(relsound))[:4]==b'RIFF'
            elif rel.startswith('Effect/'):
                effect=read_wed(data)
                assert effect=={k:v for k,v in preview['effects'][Path(rel).name].items() if k!='tintable'}
                assert all(math.isfinite(v) for f in effect['frames'] for v in f[:11])
        # A changed UI parameter must produce real installed bytes, not just a
        # playback multiplier or a browser-only mesh transform.
        circles=[e for e in preview['scripts']['meteor']['B'] if (e['wed'] or '').startswith('mm_fireball02')]
        assert len(circles)==7
        assert max(e['parts'][0]['STARTTIME'] for e in circles)<meta['meteor']['first']
        assert {e['parts'][0]['STARTTIME']+e['parts'][0]['TIMELIMIT'] for e in circles}=={meta['meteor']['fadeEnd']}
        for e in circles:
            frames=preview['effects'][e['wed']]['frames'];delay=e['parts'][0]['STARTTIME']
            fade=[max((f[11]>>16)&255,(f[11]>>8)&255,f[11]&255) for j,f in enumerate(frames) if j*1000/30+delay>=meta['meteor']['fadeStart']]
            assert fade[-1]==0 and all(a>=b for a,b in zip(fade,fade[1:]))
        ice=[e for e in preview['scripts']['frost']['B'] if e['wed'] in ('mf_spikeice01.wed','mf_spikeice02.wed')];stations=meta['frost']['stations']
        assert len(ice)==3*stations
        for branch,angle in enumerate((0,-cfg['frost']['spreadAngle'],cfg['frost']['spreadAngle'])):
            arm=ice[branch*stations:(branch+1)*stations]
            assert abs(math.hypot(*arm[-1]['parts'][0]['POS'][:2])-cfg['frost']['distance'])<1e-4
            for e in arm:
                x,y,z=e['parts'][0]['POS'];assert abs(math.degrees(math.atan2(-x,y))-angle)<.001
        mist=[e for e in preview['scripts']['frost']['B'] if e['wed']=='mf_pajang03.wed']
        rings=[e for e in preview['scripts']['frost']['B'] if e['wed']=='mf_pajang02.wed']
        assert len(mist)==9 and len(rings)==3
        assert all(e['parts'][0]['POS'][2]==.06 for e in mist+rings)
        assert sorted(e['parts'][0]['STARTTIME'] for e in mist)==sorted([cfg['frost']['startDelay']+i*cfg['frost']['stepGap'] for i in (1,3,4)]*3)
        assert all(f[:3]==[0,0,0] for f in preview['effects']['mf_pajang03.wed']['frames'])
        # Never replay the combined original B cue: launch follows each shot,
        # and impact belongs to its arrival PART, so distance/speed cannot
        # desynchronise the seven impacts as a fixed wall-clock timer would.
        meteor=preview['scripts']['meteor']['B']
        launches=[e for e in meteor if e['parts'][0].get('SOUND',{}).get('name')=='mm_meteor_launch.wav']
        balls=[e for e in meteor if e['wed']=='mm_fireball01.wed']
        assert len(launches)==len(balls)==7
        assert all(p.get('SOUND',{}).get('name')!='mb_0004_B.wav' for e in meteor for p in e['parts'])
        for launch,ball in zip(launches,balls):
            shot,hit,linger=ball['parts']
            assert launch['parts'][0]['STARTTIME']==shot['STARTTIME']
            assert launch['parts'][0]['POS']==shot['POS']
            assert shot['ATTACK'] and shot['TIMELIMIT']==0
            assert hit['HITSOUND']=='mm_meteor_hit.wav' and hit.get('STARTTIME',0)==0
            assert shot['TARGET']==[0,0,1] and hit['ENEMY']
            assert hit['TARGET']==[0,0,cfg['meteor']['impactHeight']]
            # Native NextStep initially adds POS to the previous endpoint;
            # ENEMY then uses target feet + TARGET on every later update.
            for target_height in (0,2.75,-1.5):
                entry=target_height+shot['TARGET'][2]+hit['POS'][2]
                subsequent=target_height+hit['TARGET'][2]
                assert abs(entry-subsequent)<1e-6
            assert hit['TIMELIMIT']==500 and linger['NOSHOW']
            with wave.open(io.BytesIO(out['Sound/mm_meteor_hit.wav']),'rb') as wav:
                assert hit['TIMELIMIT']+linger['TIMELIMIT']>=wav.getnframes()/wav.getframerate()*1000
        for kind,phase,hits in [('heaven','B',3),('six','B',6),('six','N',6)]:
            arrows=[e for e in preview['scripts'][kind][phase] if e['wed']]
            assert len(arrows)==hits
            assert [e['parts'][0]['STARTTIME'] for e in arrows]==[cfg[kind]['startDelay']+i*cfg[kind]['shotGap'] for i in range(hits)]
            assert all(e['parts'][1]['SPEED']==cfg[kind]['flightSpeed'] for e in arrows)
            assert all(e['parts'][1]['TARGET']==[0,0,0.9 if kind=='heaven' else 1.1] for e in arrows)
        # Changing impactSize must enlarge around the arrow's hit point,
        # without also scaling vb0001's original one-unit upward offset.
        hit=preview['effects']['mh_vb0001.wed']
        assert all(f[:3]==[0,0,0] for f in hit['frames'])
    base,_=compile_resources(read,saved,colors)
    # The impact is the original Fireball mesh/animation, with only its
    # existing 1.4x scale and user tint. Height edits must not alter its art.
    original=read_wed(read('Effect/ta_fire.wed'));private=read_wed(base['Effect/mm_ta_fire.wed'])
    assert original['mesh']==private['mesh']
    assert (original['start'],original['end'],original['cols'],original['rows'])==(private['start'],private['end'],private['cols'],private['rows'])
    for before,after in zip(original['frames'],private['frames']):
        assert before[:8]==after[:8] and before[12]==after[12]
        assert all(abs(before[i]*1.4-after[i])<1e-5 for i in (8,9,10))
    for kind,fields in FIELDS.items():
        for key,field in fields.items():
            cfg=copy.deepcopy(saved);cfg[kind][key]=field[2] if cfg[kind][key]!=field[2] else field[1]
            changed,_=compile_resources(read,cfg,colors)
            assert any(base[p]!=v for p,v in changed.items() if not p.endswith('.json')),(kind,key)
            if key=='impactHeight':
                assert [p for p,v in changed.items() if base[p]!=v and not p.endswith('.json')]==['Magic/mt_meteorB.ms']
    for bad in ({},dict(saved,unknown={})):
        try:validate_settings(bad)
        except ValueError:pass
        else:raise AssertionError('Invalid settings accepted')
    for bad in (True,float('nan'),float('inf'),-1,999999):
        cfg=copy.deepcopy(saved);cfg['frost']['distance']=bad
        try:validate_settings(cfg)
        except ValueError:pass
        else:raise AssertionError('Invalid numeric setting accepted')
    full=load_preview(H,D)
    for name,url in full['sounds'].items():assert base64.b64decode(url.split(',',1)[1])==read('Sound/'+name)
    for rel,data in meteor_sounds(read).items():
        assert data==base[rel]
        with wave.open(io.BytesIO(data),'rb') as clip:
            assert (clip.getnchannels(),clip.getsampwidth(),clip.getframerate())==(1,2,22050)
            pcm=struct.unpack('<'+'h'*clip.getnframes(),clip.readframes(clip.getnframes()))
            assert pcm[0]==pcm[-1]==0 and max(map(abs,pcm))>1000
    # Existing user values must survive adding the two frost controls.
    legacy=copy.deepcopy(saved)
    for key in ('spreadAngle','mistStrength'):legacy['frost'].pop(key)
    legacy['meteor'].pop('impactHeight')
    upgraded=upgrade_settings(legacy)
    assert all(upgraded[kind][key]==value for kind,fields in legacy.items() for key,value in fields.items())
    assert upgraded['frost']['spreadAngle']==25
    assert upgraded['meteor']['impactHeight']==.06
    actor=full['actors']['people']['0'];arrow=actor['arrow'];raw=read('Skill/Ani/arrow_m.ani')
    assert arrow['duration']==2500 and arrow['sourceFrames']==list(range(struct.unpack_from('<i',raw,32)[0]))
    for part in actor['parts']:
        if part['bones']:assert max(part['bones'])<len(arrow['bones'])
    assert actor['magic']['sourceFrames']==[i for s in motion_table(read('Body/character.wad'))[0]['magic'] for i in range(s['begin'],s['end']+1)]
    assert len({tuple(p) for p in arrow['bones'][17]['poses']})>10
    for rel,data in base.items():assert (D/rel).read_bytes()==data,rel
    report=dict(passed=True,configurations=len(variants),controls=sum(map(len,FIELDS.values())),timing=full['timing'],original_sound_files=list(full['sounds']),original_character_frames=arrow['frames'],source_hashes=full['sources'],in_game_visual_test=False,limitations=['Browser uses native assets; 30 Hz projectile timing is approximate.','Frost visual reach does not change server AoE 6.','The shipped Death Penetration ps_0001.wav is absent; the existing blue-arrow family arrow100048.wav is used.'])
    (R/'assets/skills140/tuning-validation.json').write_text(json.dumps(report,ensure_ascii=False,indent=2)+'\n',encoding='utf-8')
    print('PASS',sum(map(len,FIELDS.values())),'controls, settings migration, 7 launch/arrival cues, sound lifetimes, frost waves/angles, native loader and preview parity')

if __name__=='__main__':main()
