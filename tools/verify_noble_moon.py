"""Audit the shipped blue artwork, geometry, and preserved staff assets."""
from pathlib import Path
import sys,json,hashlib
R=Path(__file__).resolve().parents[1];sys.path.insert(0,str(R/'assets/twilight-set'))
from mesh_tools import *
from verify_closed_surfaces import audit
from build_noble_moon import active_specs
O=R/'assets/twilight-set';D=R/'client-overlay'
specs=active_specs()['weapons']
built=json.loads((O/'noble-moon-validation.json').read_text())
results=[]
for spec in specs:
    prefix='mt_twilight' if spec['kind']=='sword' else 'mt_longbow'
    path=D/f'Equip/{prefix}_1.mod';parts=read_mod(path);assert len(parts)==1
    surface=audit(path);assert surface['passed'],surface
    for lod in [2,3]:assert path.read_bytes()==(D/f'Equip/{prefix}_{lod}.mod').read_bytes()
    part=parts[0];p=part['points'];c=part['corners'];t=p[c['index']].reshape(-1,3,3)
    area=np.linalg.norm(np.cross(t[:,1]-t[:,0],t[:,2]-t[:,0]),axis=1)/2
    raw=(D/'Texture/Equip'/Path(part['texture']).with_suffix('.wtm')).read_bytes()
    im=np.asarray(Image.open(BytesIO(zlib.decompress(raw[13:]))).convert('RGB'))
    build=next(w for w in built['weapons'] if w['kind']==spec['kind'])
    face_rect=build['atlas']['material_rects']['0']
    a,b,fw,fh=[face_rect[k] for k in ['x','y','width','height']]
    source=np.asarray(Image.open(R/spec['source']).convert('RGB').crop(spec['box']))
    assert np.array_equal(im[b:b+fh,a:a+fw],source),'Native WTM must preserve every cropped source pixel'
    tex=(c['uv'].reshape(-1,3,2).mean(1)*im.shape[0]).astype(int)
    rgb=im[tex[:,1],tex[:,0]].astype(float)
    illuminated=rgb.max(1)>65
    blue=(rgb[:,2]>rgb[:,0]*1.23)&(rgb[:,2]>rgb[:,1]*1.035)&illuminated
    blue_area=float(area[blue].sum()/max(area[illuminated].sum(),1e-12))
    assert blue_area>.40,('Blue should dominate the visible native material',spec['kind'],blue_area)
    result=dict(kind=spec['kind'],native_sha256=hashlib.sha256(path.read_bytes()).hexdigest(),blue_surface_fraction=blue_area,closed_surface=surface,
                full_resolution_native_texture=True,source_pixels_verified=int(fw*fh),face_texture_size=[fw,fh])
    if spec['kind']=='sword':
        y=spec['grip'][1]-p[:,0]/spec['scale']
        u=(spec['blade_root_y']-y)/(spec['blade_root_y']-spec['tip_y'])
        depth=np.abs(p[:,2]);middle=(u>.25)&(u<.5);tip=(u>.9)&(u<=1.015);blade=(u>.1)&(u<1.015)
        mid=float(depth[middle].max());point=float(depth[tip].max())
        assert mid>.008 and point<mid*.60,(mid,point)
        assert np.count_nonzero(depth[blade]<.00027)>100
        result.update(blade_middle_full_depth=mid*2,blade_tip_full_depth=point*2,thin_rim_vertices=int(np.count_nonzero(depth[blade]<.00027)))
    results.append(result)
for name,expected in built['staff_sha256'].items():assert hashlib.sha256((D/name).read_bytes()).hexdigest()==expected
result=dict(passed=True,design='noble-blue-moon',models=results,staff_unchanged=True,in_game_visual_test=False)
(O/'noble-moon-native-validation.json').write_text(json.dumps(result,indent=2)+'\n')
print(json.dumps(result))
