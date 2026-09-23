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
            elif rel.startswith('Effect/') and rel.lower().endswith('.wed'):
                effect=read_wed(data)
                assert effect=={k:v for k,v in preview['effects'][Path(rel).name].items() if k!='tintable'}
                assert all(math.isfinite(v) for f in effect['frames'] for v in f[:11])
        # A changed UI parameter must produce real installed bytes, not just a
        # playback multiplier or a browser-only mesh transform.
        meteor=preview['scripts']['meteor']['B'];mt=meta['meteor']
        circles=[e for e in meteor if (e['wed'] or '').startswith('mm_battery_')]
        assert len(circles)==24
        assert max(e['parts'][0]['STARTTIME'] for e in circles)<mt['first']
        assert len({tuple(e['parts'][0]['TARGET']) for e in circles})==24
        normals={tuple(preview['effects'][e['wed']]['frames'][10][3:6]) for e in circles};assert len(normals)==8
        final_seals=[e for e in meteor if (e['wed'] or '').startswith('mm_final_seal')]
        assert len(final_seals)==3 and min(e['parts'][0]['STARTTIME'] for e in final_seals)>=mt['barrageEnd']
        assert max(e['parts'][0]['STARTTIME'] for e in final_seals)<mt['last']
        assert mt['finalSealAt'] - mt['barrageEnd'] == cfg['meteor']['finalSealDelay']
        assert mt['last'] - mt['finalSealAt'] == cfg['meteor']['finalFallDelay']
        balls=[e for e in meteor if e['wed'] in ('mm_cannon.wed','mm_final_fireball.wed')]
        launches=[e for e in meteor if e['parts'][0].get('SOUND',{}).get('name')=='mm_meteor_launch.wav']
        assert len(balls)==len(launches)==9
        for launch,ball in zip(launches,balls):
            anchor,shot,hit=ball['parts'];assert anchor['ENEMY'] and shot['ATTACK']
            assert launch['parts'][0]['STARTTIME']==anchor['STARTTIME']+80
            assert hit['ENEMY'] and hit['TARGET'][:2]==shot['TARGET'][:2]
            assert hit['TARGET'][2]==cfg['meteor']['impactHeight']
            assert abs(shot['TARGET'][2]+hit['POS'][2]-hit['TARGET'][2])<1e-6
        followers=[e for e in meteor if any(p.get('TAIL','').startswith(('mm_fire_vortex','mm_magma','mm_final_wave')) for p in e['parts'])]
        assert len(followers)==14
        for e in followers:
            assert e['parts'][:2]==[dict(balls[-1]['parts'][0]),dict(balls[-1]['parts'][1],NOSHOW=True)]
        shells=[e for e in meteor if any(p.get('TAIL','').startswith(('mm_shell_bloom','mm_shell_wave')) for p in e['parts'])]
        assert len(shells)==16
        for i in range(8):
            for e in shells[i*2:i*2+2]:
                assert e['parts'][:2]==[dict(balls[i]['parts'][0]),dict(balls[i]['parts'][1],NOSHOW=True)]
                assert e['parts'][2]['TARGET'][:2]==balls[i]['parts'][1]['TARGET'][:2]
                assert e['parts'][2]['POS'][:2]==[0,0],'Arrival displacement applied twice'
        storm=[e for e in meteor if any(p.get('TAIL','').startswith('mm_stormfront') for p in e['parts'])]
        blast_scale=cfg['meteor'].get('finalImpactSize',1)
        assert len(storm)==18 and mt['stormReach']>12*min(1,blast_scale)
        for e in storm:
            assert e['parts'][:2]==[dict(balls[-1]['parts'][0]),dict(balls[-1]['parts'][1],NOSHOW=True)]
            fs=preview['effects'][e['parts'][-1]['TAIL']]['frames']
            radii=[math.hypot(g[0]*g[9],g[2]*g[10]) for g in fs]
            assert radii[-1]>10*blast_scale and all(a<=b+1e-5 for a,b in zip(radii,radii[1:])), 'Final blast failed to expand outward'
            assert fs[-1][11]&0xffffff==0
        pressure=preview['effects']['mm_storm_pressure.wed']
        assert pressure['texture']=='ta_06ani.wtm' and (pressure['cols'],pressure['rows'])==(3,3)
        assert all(g[3:7]==[1,0,0,270] for g in pressure['frames'])
        assert preview['effects']['mm_final_fireball.wed']['mesh']=='pl_05.WEM'
        assert preview['effects']['mm_final_fireball.wed']['texture']=='fireball01.wtm'
        assert preview['effects']['mm_fire_vortex0.wed']['mesh']=='mm_fire_funnel.WEM'
        assert preview['effects']['mm_fire_vortex0.wed']['texture']=='fire_03.wtm'
        assert preview['effects']['mm_cannon_hit.wed']['texture']=='fire_02.wtm'
        assert len({preview['effects'][e['wed']]['texture'] for e in final_seals})==3
        # Each battery's plane normal points back along its actual shot path.
        for i,ball in enumerate(balls[:-1]):
            origin=ball['parts'][0]['TARGET'];dest=ball['parts'][1]['TARGET']
            expected=(origin[0]-dest[0],origin[2]-dest[2],origin[1]-dest[1])
            n=preview['effects'][f'mm_battery_{i}_0.wed']['frames'][10][3:6]
            assert sum(a*b for a,b in zip(expected,n))/math.sqrt(sum(v*v for v in expected))>.9999
        magma=preview['effects']['mm_magma.wed']['frames'];assert magma[-1][11]&0xffffff==0
        assert meta['meteor']['fadeEnd']-meta['meteor']['finalImpact']==3800
        frost=preview['scripts']['frost']['B'];ft=meta['frost']
        ice=[e for e in frost if (e['wed'] or '').startswith('mf_eruption_')]
        assert len(ice)==ft['stations'] and len(ice)>=32 and all(e['parts'][0]['ENEMY'] for e in ice)
        assert all(e['parts'][0]['STARTTIME']+e['parts'][0]['TIMELIMIT']==ft['fadeEnd'] for e in ice)
        fronts=[e for e in frost if (e['wed'] or '').startswith('mf_pressure_front')]
        assert len(fronts)==ft['pressureFronts']==0
        assert ft['supportsTrueOpacity'] is True
        # Stationary broken bands rise outside-in with unequal clustered roots.
        radii=[]
        for e in ice:
            fs=preview['effects'][e['wed']]['frames']
            assert all(g[:3]==[0,0,0] for g in fs),'Ice slid away from its ground root'
            assert fs[0][8]<fs[10][8] and max(g[8] for g in fs)<=fs[-1][8]*1.08
            assert preview['effects'][e['wed']]['texture']=='ice_02.wtm'
            radii.append(math.hypot(*e['parts'][0]['TARGET'][:2]))
        offset=0
        assert all(a<b for a,b in zip(ft['ringTimes'],ft['ringTimes'][1:]))
        assert all(a>b for a,b in zip(ft['ringRadii'],ft['ringRadii'][1:]))
        for at,radius,count,half_width,last_at in zip(ft['ringTimes'],ft['ringRadii'],ft['ringCounts'],ft['ringBandHalfWidths'],ft['ringEndTimes']):
            values=radii[offset:offset+count]
            times=[e['parts'][0]['STARTTIME'] for e in ice[offset:offset+count]]
            assert all(abs(r-radius)<=half_width+2e-5 for r in values)
            assert max(values)-min(values)>half_width*.6
            assert min(times)==at and max(times)==last_at and len(set(times))>=3
            offset+=count
        assert offset==len(ice)
        assert max(e['parts'][0]['STARTTIME'] for e in ice)<ft['pillarAt']
        aftershocks=[e for e in frost if (e['wed'] or '').startswith('mf_aftershock_')]
        assert len(aftershocks)==ft['aftershocks']==0
        snow=[e for e in frost if (e['wed'] or '').startswith('mf_blizzard')]
        assert len(snow)==3 and ft['blizzardEnd']>ft['fadeEnd']
        for e in snow:
            fs=preview['effects'][e['wed']]['frames']
            assert fs[0][6]!=fs[-1][6] and fs[-1][11]&0xffffff==0
            assert e['parts'][0]['STARTTIME']+e['parts'][0]['TIMELIMIT']==ft['blizzardEnd']
        assert ft['pillarRiseMs']<250
        floor=[e for e in frost if (e['wed'] or '').startswith('mf_frozen_ground')]
        assert len(floor)==6 and ft['floorEnd']>ft['fadeEnd']+1000
        assert all(e['parts'][0]['STARTTIME']+e['parts'][0]['TIMELIMIT']==ft['floorEnd'] for e in floor)
        for e in floor:
            assert preview['effects'][e['wed']]['frames'][-1][11]&0xffffff==0
        seals=[e for e in frost if (e['wed'] or '').startswith('mf_cocytus_seal')]
        assert len(seals)==3 and all(e['parts'][0]['ENEMY'] for e in seals)
        heights=[e['parts'][0]['TARGET'][2] for e in seals]
        assert all(math.isfinite(h) for h in heights)
        assert all(e['parts'][0]['TARGET'][:2]==[cfg['frost'].get('sealOffsetX',0),cfg['frost'].get('sealOffsetY',0)] for e in seals)
        assert all(e['parts'][0]['STARTTIME']+e['parts'][0]['TIMELIMIT']==ft['fadeEnd'] for e in seals)
        pillar=next(e for e in frost if e['wed']=='mf_cocytus_pillar.wed')
        assert pillar['parts'][0]['STARTTIME']+pillar['parts'][0]['TIMELIMIT']==ft['fadeEnd']
        spikes=[e for e in frost if (e['wed'] or '').startswith('mf_crown_spike')]
        assert len(spikes)==ft['radialSpikes']
        assert all(e['parts'][0]['STARTTIME']+e['parts'][0]['TIMELIMIT']==ft['fadeEnd'] for e in spikes)
        for kind,phase,hits in [('heaven','B',3),('six','B',6),('six','N',6)]:
            arrows=[e for e in preview['scripts'][kind][phase] if any(p.get('ATTACK') for p in e['parts']) and any('HITSOUND' in p for p in e['parts'])]
            assert len(arrows)==hits
            assert [e['parts'][0]['STARTTIME'] for e in arrows]==[cfg[kind]['startDelay']+i*cfg[kind]['shotGap'] for i in range(hits)]
            assert all(e['parts'][1]['SPEED']==cfg[kind]['flightSpeed'] for e in arrows)
            assert len(set(e['parts'][1]['TARGET'][2] for e in arrows))==hits
            muzzle=[e for e in preview['scripts'][kind][phase] if (e['wed'] or '').endswith('_launch.wed')]
            assert len(muzzle)==hits
            assert [e['parts'][0]['STARTTIME'] for e in muzzle]==[e['parts'][0]['STARTTIME'] for e in arrows]
            assert all(e['parts'][0]['POS'][1]>0 and not e['parts'][0].get('ENEMY') for e in muzzle)
            for e in muzzle:
                for f in preview['effects'][e['wed']]['frames']:
                    assert abs(f[1]*f[8]-1.25)<1e-5,'Launch flash moved away from bow height'
        # Changing impactSize must enlarge around the arrow's hit point,
        # without also scaling vb0001's original one-unit upward offset.
        hit=preview['effects']['mh_vb0001.wed']
        assert all(f[:3]==[0,0,0] for f in hit['frames'])
    base,_=compile_resources(read,saved,colors)
    # Custom rocks and three glyph planes are native meshes in both outputs.
    from native_assets import read_wem
    for rel,data in base.items():
        if rel.endswith('.WEM'):
            mesh=read_wem(data)
            assert len(mesh['indices'])%3==0 and len(mesh['vertices'])<65535
    assert read_wed(base['Effect/mm_cannon.wed'])['mesh']=='pl_05.WEM'
    for kind,fields in FIELDS.items():
        for key,field in fields.items():
            cfg=copy.deepcopy(saved);cfg[kind][key]=field[2] if cfg[kind][key]!=field[2] else field[1]
            changed,_=compile_resources(read,cfg,colors)
            assert any(base.get(p)!=v for p,v in changed.items() if not p.endswith('.json')),(kind,key)
            if kind=='meteor' and key=='impactHeight':
                assert changed['Magic/mt_meteorB.ms']!=base['Magic/mt_meteorB.ms']
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
    # Upgrade the original twenty-control schema without resetting user values.
    original_keys={
        'meteor':{'portalGap','readyHold','shotGap','flightSpeed','impactHeight'},
        'frost':{'startDelay','stepGap','distance','size','riseSpeed','brightness','spreadAngle','mistStrength'},
        'heaven':{'startDelay','shotGap','flightSpeed','impactSize'},
        'six':{'startDelay','shotGap','flightSpeed'},
    }
    original={kind:{key:saved[kind][key] for key in keys} for kind,keys in original_keys.items()}
    migrated=upgrade_settings(original)
    assert all(migrated[kind][key]==value for kind,fields in original.items() for key,value in fields.items())
    assert all(migrated[kind][key]==DEFAULTS[kind][key] for kind,fields in FIELDS.items() for key in fields if key not in original_keys[kind] and key != 'pierceWidth')
    assert migrated['heaven']['pierceWidth'] == original['heaven']['impactSize']
    # Also keep migration from the older seventeen-control editor working.
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
    print('PASS',sum(map(len,FIELDS.values())),'controls, settings migration, 9 launch cues / 7 damage events, sound lifetimes, frost waves/angles, native loader and preview parity')

if __name__=='__main__':main()
