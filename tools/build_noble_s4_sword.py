"""Build the user-selected S4 sword while keeping bow/staff byte-identical."""
from pathlib import Path
import json,shutil,subprocess,sys
from build_noble_moon import R,O,D,build,digest
from mesh_tools import read_mod,np,Image,BytesIO,zlib
from verify_closed_surfaces import audit
from refine_s4_blade import verify_refined_native

SPEC=O/'noble-moon-s4-sword-geometry.json'
REVIEW=O/'s4'

def main():
    REVIEW.mkdir(parents=True,exist_ok=True)
    protected=[]
    for prefix in ['mt_longbow','mt_staff']:
        protected.extend(D/f'Equip/{prefix}_{lod}.mod' for lod in (1,2,3))
        protected.append(D/f'Texture/Equip/{prefix}_atlas.wtm')
    protected.extend([O/'longbow-native.glb',O/'staff-native.glb',D/'Item/ITEM.dat',D/'Interface/item.dat',D/'Equip/Trace.txt'])
    before={str(p.relative_to(R)):digest(p) for p in protected}
    sword=D/'Equip/mt_twilight_1.mod';old=read_mod(sword)[0]
    prior=dict(sha256=digest(sword),bounds=[old['points'].min(0).tolist(),old['points'].max(0).tolist()],triangles=len(old['corners'])//3)
    if not (REVIEW/'prior-sword.json').exists():
        (REVIEW/'prior-sword.json').write_text(json.dumps(prior,indent=2)+'\n')
        shutil.copy2(O/'sword-native.glb',REVIEW/'prior-sword-native.glb')
    spec=json.loads(SPEC.read_text())['weapons'][0]
    assert spec['kind']=='sword'
    result=build(spec)
    if spec.get('blade_refinement'):
        refinement_reference=json.loads((REVIEW/'blade-refinement-validation.json').read_text())
        result['blade_refinement']=verify_refined_native(sword.read_bytes(),spec,refinement_reference)
    surface=audit(sword);assert surface['passed'],surface
    raw=(D/'Texture/Equip/mt_twilight_atlas.wtm').read_bytes()
    native=np.asarray(Image.open(BytesIO(zlib.decompress(raw[13:]))).convert('RGB'))
    r=result['atlas']['material_rects'][0];source=np.asarray(Image.open(R/spec['source']).convert('RGB').crop(spec['box']))
    assert np.array_equal(native[r['y']:r['y']+r['height'],r['x']:r['x']+r['width']],source)
    subprocess.run([sys.executable,'-B','-X','utf8',str(O/'preview_native.py'),'--kind','sword'],cwd=R,check=True)
    after={str(p.relative_to(R)):digest(p) for p in protected};assert before==after,'Only the sword may change'
    active=json.loads((O/'active-design.json').read_text(encoding='utf-8'))
    active.setdefault('default_build_kinds',['sword','longbow'])
    active.setdefault('overrides',{})['sword']=dict(design='noble-blue-moon-s4-sword',source='../visual-refresh-20260923/noble-moon-15/sword-04.png',geometry_spec=SPEC.name,builder='tools/build_noble_s4_sword.py',approved_by_user=True)
    active['sources'][0]='../visual-refresh-20260923/noble-moon-15/sword-04.png'
    (O/'active-design.json').write_text(json.dumps(active,ensure_ascii=False,indent=2)+'\n',encoding='utf-8')
    common=json.loads((O/'noble-moon-validation.json').read_text())
    common['weapons']=[result if w['kind']=='sword' else w for w in common['weapons']]
    (O/'noble-moon-validation.json').write_text(json.dumps(common,indent=2)+'\n')
    replacements=[D/f'Equip/mt_twilight_{lod}.mod' for lod in (1,2,3)]+[D/'Texture/Equip/mt_twilight_atlas.wtm']
    report=dict(passed=True,design='noble-blue-moon-s4-sword',model=result,surface=surface,
                native_source_pixels_verified=int(source.shape[0]*source.shape[1]),protected_sha256=after,
                bow_and_staff_not_regenerated=True,installed_client_written=False,
                previous=json.loads((REVIEW/'prior-sword.json').read_text()),
                replacements=[dict(path=str(p.relative_to(D)).replace('\\','/'),sha256=digest(p),bytes=p.stat().st_size) for p in replacements])
    (REVIEW/'build-validation.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps(dict(passed=True,replacements=report['replacements'],protected_files=len(after))))

if __name__=='__main__':main()
