"""Opacity changes only ice ARGB alpha; geometry, RGB and other effects stay put."""
from pathlib import Path
import copy,hashlib,json,sys
R=Path(__file__).resolve().parents[1];D=R/'client-overlay';H=D/'Tools/SkillColors'
sys.dont_write_bytecode=True;sys.path.insert(0,str(H))
from tuning import compile_resources,load_settings,resource_reader,upgrade_settings
from native_assets import read_wed,load_preview
from ice_opacity import is_ice_surface

saved=load_settings(H);colors=json.loads((H/'colors.json').read_text());read=resource_reader(D)
results=[];zero=None;surfaces=None
for opacity in (0,25,85,100):
    cfg=copy.deepcopy(saved);cfg['frost']['iceOpacity']=opacity
    out,meta=compile_resources(read,cfg,colors,only=['frost'])
    current=set(meta['frost']['alphaSurfaces'])
    assert len(current)==meta['frost']['stations']+meta['frost']['radialSpikes']+1
    assert 'mf_cocytus_pillar_rim.wed' not in current
    assert current=={Path(p).name for p in out if is_ice_surface(p)}
    if zero is None:zero=out;surfaces=current
    assert current==surfaces
    for rel,raw in out.items():
        if is_ice_surface(rel):
            frames=read_wed(raw)['frames'];reference=read_wed(zero[rel])['frames']
            for a,b in zip(frames,reference):
                assert a[:11]==b[:11] and a[12]==b[12]
                assert a[11]&0xffffff==b[11]&0xffffff,'Opacity changed brightness instead of background visibility'
            assert max(f[11]>>24 for f in frames)==round(opacity*255/100)
            assert frames[0][11]>>24==frames[-1][11]>>24==0
        elif not rel.endswith('.json'):
            assert raw==zero[rel],('Opacity changed another layer',rel)
    preview=load_preview(H,D,generated=out,metadata=meta,compact=True,kinds=['frost'])
    assert preview['timing']['frost']['alphaSurfaces']==sorted(surfaces)
    assert all(preview['effects'][name]['frames']==read_wed(out['Effect/'+name])['frames'] for name in surfaces)
    results.append(dict(opacity=opacity,alphaByte=round(opacity*255/100),surfaces=len(surfaces)))
legacy=copy.deepcopy(saved);legacy['frost'].pop('iceOpacity')
upgraded=upgrade_settings(legacy)
assert upgraded['frost']['iceOpacity']==85
assert all(upgraded[k][p]==v for k,fields in legacy.items() for p,v in fields.items())
report=dict(passed=True,cases=results,rgbGeometryAndOtherLayersUnchanged=True,previewNativeAlphaParity=True,
    savedValuesPreserved=sum(map(len,legacy.values())),nativeRendererRequirement='ice-opacity-v1',inGameDirect3DTest=False)
(R/'assets/skills140/ice-opacity-validation.json').write_text(json.dumps(report,ensure_ascii=False,indent=2)+'\n',encoding='utf-8')
print(json.dumps(report))
