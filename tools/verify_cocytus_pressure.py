"""Check irregular stationary ice bands; keep the old script name for QA callers."""
from pathlib import Path
import copy, hashlib, json, math, sys
R=Path(__file__).resolve().parents[1];D=R/'client-overlay';H=D/'Tools/SkillColors'
sys.dont_write_bytecode=True;sys.path[:0]=[str(H),str(R/'tools')]
from tuning import FIELDS,DEFAULTS,compile_resources,load_settings,resource_reader,upgrade_settings,validate_settings
from native_assets import read_magic,read_wed,read_wem
from verify_magic_loader import parse
from ice_opacity import is_ice_surface

def assert_rings(out, ft, effects):
    ice=[e for e in effects if (e['wed'] or '').startswith('mf_eruption_')]
    assert 4<=ft['rings']<=5 and len(ice)==ft['stations']==sum(ft['ringCounts'])
    assert len(ft['ringTimes'])==len(ft['ringRadii'])==len(ft['ringCounts'])==ft['rings']
    assert all(a<b for a,b in zip(ft['ringTimes'],ft['ringTimes'][1:]))
    assert all(a>b>0 for a,b in zip(ft['ringRadii'],ft['ringRadii'][1:]))
    assert ft['ringTimes'][-1]<ft['pillarAt']
    forbidden=('mf_pressure_front','mf_incoming_mist','mf_settling_mist','mf_aftershock_')
    assert not any((e['wed'] or '').startswith(forbidden) for e in effects)
    assert ft['pressureFronts']==0 and ft['aftershocks']==0
    at_index=0
    for ring_index,(at,radius,count,half_width,last_at) in enumerate(zip(ft['ringTimes'],ft['ringRadii'],ft['ringCounts'],ft['ringBandHalfWidths'],ft['ringEndTimes'])):
        ring=ice[at_index:at_index+count];at_index+=count
        points=[];times=[];heights=[];widths=[];rotations=[]
        for e in ring:
            assert len(e['parts'])==1 and e['parts'][0]['ENEMY']
            part=e['parts'][0];x,y,z=part['TARGET'];points.append((x,y))
            assert abs(math.hypot(x,y)-radius)<=half_width+1e-5,'Crystal roots left their eruption band'
            times.append(part['STARTTIME'])
            assert at<=part['STARTTIME']<=last_at and part['STARTTIME']+part['TIMELIMIT']==ft['fadeEnd']
            frames=read_wed(out['Effect/'+e['wed']])['frames']
            assert all(g[:3]==[0,0,0] for g in frames),'A ring crystal moved horizontally'
            assert all(g[3:8]==frames[0][3:8] and g[9:11]==frames[0][9:11] for g in frames),'Only vertical growth is allowed'
            assert frames[0][8]<max(g[8] for g in frames) and frames[-1][11]&0xffffff==0
            heights.append(max(g[8] for g in frames));widths.append(frames[0][9]);rotations.append(frames[0][6])
            assert all(a[8]<=b[8]+1e-6 for a,b in zip(frames,frames[1:])),'Ring ice must not wash back down'
        # Distinct clusters and holes replace exact angular spacing and radius.
        assert len(set(times))>=3 and min(times)==at and max(times)==last_at
        assert last_at<(ft['ringTimes'][ring_index+1] if ring_index+1<ft['rings'] else ft['pillarAt'])
        assert max(heights)/min(heights)>2.4 and max(widths)/min(widths)>1.4
        assert max(rotations)-min(rotations)>150
        actual_radii=[math.hypot(x,y) for x,y in points]
        assert max(actual_radii)-min(actual_radii)>half_width*.6
        angles=sorted(math.atan2(y,x)%math.tau for x,y in points)
        gaps=[(angles[(i+1)%count]-a)%math.tau for i,a in enumerate(angles)]
        assert max(gaps)>math.tau/count*1.7 and min(gaps)<math.tau/count*.6
    return ice

def main():
    read=resource_reader(D);saved=load_settings(H);colors=json.loads((H/'colors.json').read_text())
    base,meta=compile_resources(read,saved,colors,only=['frost'])
    keys=['iceOpacity','iceDensity','mistAmount','inwardStartRadius','inwardEndRadius']
    variants=[('saved',saved)]
    for key in keys:
        for edge in (1,2):
            cfg=copy.deepcopy(saved);cfg['frost'][key]=FIELDS['frost'][key][edge]
            variants.append((f'{key}:{cfg["frost"][key]}',cfg))
    for edge in (1,2):
        cfg=copy.deepcopy(saved)
        cfg['frost']={key:field[edge] for key,field in FIELDS['frost'].items()}
        variants.append((f'all-frost-{edge}',cfg))
    reports=[]
    for label,cfg in variants:
        out,metadata=compile_resources(read,cfg,colors,only=['frost']);ft=metadata['frost']
        script=out['Magic/mt_frostB.ms'];counts=parse(script)
        assert counts['blows']==3
        effects=read_magic(script.decode('cp949'))
        for rel,data in out.items():
            if rel.endswith('.wed'):
                wed=read_wed(data)
                assert all(math.isfinite(n) for frame in wed['frames'] for n in frame[:11])
                if is_ice_surface(rel):
                    assert max(frame[11]>>24 for frame in wed['frames']) == round(255*cfg['frost']['iceOpacity']/100)
                    assert wed['frames'][0][11]>>24 == wed['frames'][-1][11]>>24 == 0
                else:
                    assert all(frame[11]>>24==255 for frame in wed['frames']), 'Unrelated additive alpha changed'
            elif rel.endswith('.WEM'):
                mesh=read_wem(data)
                assert len(mesh['indices'])%3==0 and max(mesh['indices'])<len(mesh['vertices'])<65535
        assert_rings(out,ft,effects)
        assert abs(ft['ringRadii'][0]-ft['outerRadius'])<1e-5
        if cfg['frost']['inwardEndRadius']>0:assert abs(ft['ringRadii'][-1]-ft['innerRadius'])<1e-5
        else:assert ft['rings']==4
        assert ft['pillarRiseMs']<200 and ft['supportsTrueOpacity']
        for key in ('mf_cocytus_pillar.wed','mf_cocytus_pillar_rim.wed'):
            item=next(e for e in effects if e['wed']==key)
            assert item['parts'][0]['STARTTIME']==ft['pillarAt']
            assert item['parts'][0]['TIMELIMIT']+ft['pillarAt']==ft['fadeEnd']
        pfs=read_wed(out['Effect/mf_cocytus_pillar.wed'])['frames']
        peak=max((g[11]&0xffffff for g in pfs),key=lambda c:c&255)
        assert peak&255>(peak>>8)&255>(peak>>16)&255, 'The broad ice body lost its saturated blue'
        fog=[e for e in effects if (e['wed'] or '').startswith('mf_ring_mist')]
        assert len(fog)==ft['mistPuffs']
        assert bool(fog)==(cfg['frost']['mistAmount']>0)
        for e in fog:
            frames=read_wed(out['Effect/'+e['wed']])['frames']
            assert all(g[:11]==frames[0][:11] for g in frames),'Fog must remain fixed at its ring'
        assert out['Texture/Effect/mf_cold_mist.wtm']==read('Texture/Particle/ob_11.wtm')
        if label.startswith('iceDensity:'):
            assert out['Effect/mf_cocytus_pillar.wed']!=base['Effect/mf_cocytus_pillar.wed']
            assert out['Effect/mf_cocytus_pillar_rim.wed']==base['Effect/mf_cocytus_pillar_rim.wed']
            assert out['Effect/mf_ring_mist0_0.wed']==base['Effect/mf_ring_mist0_0.wed']
        elif label.startswith('mistAmount:'):
            assert out['Effect/mf_cocytus_pillar.wed']==base['Effect/mf_cocytus_pillar.wed']
            assert out['Magic/mt_frostB.ms']!=base['Magic/mt_frostB.ms']
        elif label.startswith(('inwardStartRadius:','inwardEndRadius:')):
            assert out['Magic/mt_frostB.ms']!=base['Magic/mt_frostB.ms']
        reports.append(dict(configuration=label,nativeParts=counts['parts'],damageEvents=counts['blows'],ringTimes=ft['ringTimes'],ringEndTimes=ft['ringEndTimes'],ringRadii=ft['ringRadii'],ringBandHalfWidths=ft['ringBandHalfWidths'],ringCounts=ft['ringCounts'],ringClusters=ft['ringClusters'],mistPuffs=len(fog),fixedHorizontalAnchors=True))
    # Recreate the immediately preceding schema independently of saved files.
    old56=copy.deepcopy(saved)
    for key in keys:old56['frost'].pop(key)
    assert sum(map(len,old56.values()))==sum(map(len,saved.values()))-len(keys)
    migrated=upgrade_settings(old56)
    assert all(migrated[k][p]==v for k,fields in old56.items() for p,v in fields.items())
    assert all(migrated['frost'][key]==DEFAULTS['frost'][key] for key in keys)
    for key in keys:
        for value in (True,float('nan'),FIELDS['frost'][key][1]-.1,FIELDS['frost'][key][2]+.1):
            bad=copy.deepcopy(saved);bad['frost'][key]=value
            try:validate_settings(bad)
            except ValueError:pass
            else:raise AssertionError((key,value,'invalid setting accepted'))
    baseline=R/'.cache/v12-before.json';protected=None
    if baseline.exists():
        before=json.loads(baseline.read_text(encoding='utf-8'))
        assert all(saved[kind][key]==value for kind,fields in before['saved_tuning'].items() for key,value in fields.items())
        assert colors==before['saved_colors']
        for path,expected in before['protected'].items():assert hashlib.sha256(Path(path).read_bytes()).hexdigest()==expected,path
        protected=len(before['protected'])
    report=dict(passed=True,configurations=reports,controls=keys,savedValuesPreserved=sum(map(len,before['saved_tuning'].values())) if baseline.exists() else None,protectedFilesUnchanged=protected,supportsTrueOpacity=True,nativeBlend='SRCALPHA/INVSRCALPHA for ice surfaces',inGameVisualTest=False)
    (R/'assets/skills140/cocytus-organic-rings-validation.json').write_text(json.dumps(report,ensure_ascii=False,indent=2)+'\n',encoding='utf-8')
    print('PASS:',len(reports),'irregular fixed-band native configurations, true alpha, independent controls, saved values, 3 damage events; protected files:',protected)

if __name__=='__main__':main()
