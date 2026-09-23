"""Read-only HTTP regression for targeted live updates and native asset deltas."""
from pathlib import Path
import argparse
import base64
import copy
import hashlib
import json
import re
import sys
import time
from urllib.request import Request, urlopen

R=Path(__file__).resolve().parents[1]
H=R/'client-overlay/Tools/SkillColors'
sys.dont_write_bytecode=True
sys.path.insert(0,str(H))
from native_assets import png_from_wtm, read_wem
from tuning import compile_resources, resource_reader


def main():
    parser=argparse.ArgumentParser();parser.add_argument('--port',type=int,default=8878);args=parser.parse_args()
    base_url=f'http://127.0.0.1:{args.port}'
    protected={name:hashlib.sha256((H/name).read_bytes()).hexdigest() for name in ('colors.json','tuning.json')}
    html=urlopen(base_url,timeout=10).read().decode()
    settings=json.loads(re.search(r'<script[^>]*id="settings"[^>]*>(.*?)</script>',html,re.S)[1])
    initial=json.load(urlopen(base_url+'/preview.json',timeout=30))
    known={key:value for key,value in initial['sources'].items() if re.fullmatch(r'(Effect/.*\.WEM|Texture/Effect/.*\.wtm)',key,re.I)}
    timings={}

    def post(kind,tuning,known_sources):
        payload=dict(colors=settings['colors'],tuning=tuning,kinds=[kind],knownSources=known_sources)
        request=Request(base_url+'/preview',data=json.dumps(payload).encode(),headers={
            'Content-Type':'application/json','X-Local-Token':settings['token'],'Origin':base_url})
        started=time.perf_counter();raw=urlopen(request,timeout=30).read();elapsed=time.perf_counter()-started
        result=json.loads(raw)
        assert set(result['scripts'])=={kind} and set(result['timing'])=={kind}
        assert result['previewMetrics']['kinds']==[kind]
        return result,dict(seconds=round(elapsed,3),bytes=len(raw))

    for kind in ('meteor','frost','heaven','six'):
        result,timings[kind]=post(kind,settings['tuning'],known)
        assert not result['meshes'] and not result['textures'],kind
        assert result['scripts'][kind]==initial['scripts'][kind]
        for name,effect in result['effects'].items():assert effect==initial['effects'][name]

    changed=copy.deepcopy(settings['tuning']);changed['heaven']['brightness']=.3
    if changed['heaven']['brightness']==settings['tuning']['heaven']['brightness']:changed['heaven']['brightness']=.6
    updated,_=post('heaven',changed,known)
    generated,_=compile_resources(resource_reader(R/'client-overlay'),changed,settings['colors'],only=['heaven'])
    assert 'mh_tuned_trace.wtm' in updated['textures']
    trace_png=base64.b64decode(updated['textures']['mh_tuned_trace.wtm'].split(',',1)[1])
    assert trace_png==png_from_wtm(generated['Texture/Effect/mh_tuned_trace.wtm'])
    assert updated['textures']['mh_tuned_trace.wtm']!=initial['textures']['mh_tuned_trace.wtm']
    unchanged,_=post('heaven',changed,{**known,**updated['sources']})
    assert not unchanged['textures'] and not unchanged['meshes']

    changed=copy.deepcopy(settings['tuning']);changed['frost']['blizzardStrength']=.35
    if changed['frost']['blizzardStrength']==settings['tuning']['frost']['blizzardStrength']:changed['frost']['blizzardStrength']=.8
    updated,_=post('frost',changed,known)
    generated,_=compile_resources(resource_reader(R/'client-overlay'),changed,settings['colors'],only=['frost'])
    assert updated['meshes'],'Changed blizzard geometry must be sent to the visible preview'
    for name,data in updated['meshes'].items():assert data==read_wem(generated['Effect/'+name])
    assert all(hashlib.sha256((H/name).read_bytes()).hexdigest()==digest for name,digest in protected.items())
    report=dict(passed=True,targeted_requests=timings,delta_trace_pixels_match=True,delta_blizzard_mesh_matches=True,
                saved_settings_unchanged=protected,in_game_visual_test=False)
    (R/'assets/skills140/live-preview-validation.json').write_text(json.dumps(report,ensure_ascii=False,indent=2)+'\n',encoding='utf-8')
    print('PASS live-preview HTTP:',json.dumps(timings),'native trace/geometry deltas; settings unchanged')


if __name__=='__main__':main()
