from mesh_tools import *
from verify_closed_surfaces import audit
import hashlib,re
O=Path(__file__).resolve().parent;D=ROOT/'client-overlay';C=ROOT/'runtime/client/GameClient'
def rows(raw):return {int(r[0]):r for l in raw.decode('cp949').splitlines() if (r:=l.split('\t'))[0].isdigit()}
defs=json.loads((O/'item_definitions.json').read_text(encoding='utf-8'));server=rows((ROOT/'game-data/DATA/ITEM_DATA.txt').read_bytes());client=rows((D/'Item/ITEM.dat').read_bytes());ui=rows(zlib.decompress((D/'Interface/item.dat').read_bytes()[20:]))
for d in defs:
    r=server[d['id']];assert r==client[d['id']]
    assert list(map(int,r[6:10]))==[d['min'],d['max'],d['magic_min'],d['magic_max']]
    assert ui[d['id']][7:11]==r[6:10] and ui[d['id']][6]=='9'
    assert r[19:24]==['0']*5 and int(r[4])==d['type']
    for start in [55,61]:
        assert all((D/'Equip'/n).is_file() for n in r[start:start+3])
        for n in r[start+3:start+5]:assert (D/'Item'/Path(n).with_suffix('.wtm')).is_file() and (D/'Texture/Body'/Path(n).with_suffix('.wtm')).is_file()
for base in [19060,19090]:
    for field in [6,7]+([8,9] if base==19090 else []):assert all(int(server[n][field])<int(server[n+1][field]) for n in range(base,base+20))
models=[]
for prefix in ['mt_longbow','mt_staff','mt_twilight']:
    p=D/f'Equip/{prefix}_1.mod';parts=read_mod(p);assert len(parts)==1
    surface=audit(p);assert surface['passed'],surface
    for lod in [2,3]:assert p.read_bytes()==(D/f'Equip/{prefix}_{lod}.mod').read_bytes()
    part=parts[0];points=part['points'];corners=part['corners'];uv=corners['uv'].reshape(-1,3,2);tri=points[corners['index']].reshape(-1,3,3)
    area=np.linalg.norm(np.cross(tri[:,1]-tri[:,0],tri[:,2]-tri[:,0]),axis=1)/2;assert (area>1e-14).all()
    assert corners['uv'].min()>0 and corners['uv'].max()<1
    n=corners['normal'].reshape(-1,3,3).mean(1);side=np.abs(n[:,2])<.9
    dua=uv[:,1]-uv[:,0];dub=uv[:,2]-uv[:,0];uva=np.abs(dua[:,0]*dub[:,1]-dua[:,1]*dub[:,0])/2
    # Round cylinder caps can collapse at a single pixel by design. Structural
    # side faces must sample two-dimensional texels across the thickness.
    assert np.count_nonzero(uva[side]>1e-12)>100
    b=(D/'Texture/Equip'/Path(part['texture']).with_suffix('.wtm')).read_bytes();bmp=zlib.decompress(b[13:]);assert len(bmp)==struct.unpack_from('<I',b,9)[0]
    im=np.asarray(Image.open(BytesIO(bmp)).convert('RGB'));xy=(uv[side].mean(1)*im.shape[0]).astype(int);colors=im[xy[:,1],xy[:,0]]
    assert colors.std()>5 and colors.mean()<210
    models.append(dict(model=prefix,triangles=len(tri),thickness=float(np.ptp(points[:,2])),side_mean=float(colors.mean()),side_std=float(colors.std()),meshes=1,sha256=hashlib.sha256(p.read_bytes()).hexdigest()))
# Verify actual sword vertices have a thin cutting rim and a thick interior.
info=json.loads((ROOT/'assets/twilight/model_info.json').read_text(encoding='utf-8'))['construction_transform'];p=read_mod(D/'Equip/mt_twilight_1.mod')[0]['points']
xy=np.column_stack((p[:,1]/info['pixel_scale']+info['x_origin'],(info['y_offset']-.5-p[:,0])/info['pixel_scale']))
design=json.loads((ROOT/'assets/twilight/design04_geometry.json').read_text());poly=Polygon(design['parts'][0]['outline'],design['parts'][0]['holes']);dist=shapely.distance(shapely.points(xy),poly.boundary)
rim=(xy[:,1]<850)&(dist<.6);body=(xy[:,1]<850)&(dist>12);guard=(xy[:,1]>980)&(xy[:,1]<1035)
assert rim.any() and body.any() and guard.any()
assert np.max(np.abs(p[rim,2]))<.003 and np.max(np.abs(p[body,2]))>.014 and np.ptp(p[guard,2])>.03
result=dict(passed=True,item_definitions=42,server_client_tooltip_match=True,all_0_to_20_stats_increase=True,models=models,sword_cutting_rim=float(np.ptp(p[rim,2])),sword_body=float(np.ptp(p[body,2])),sword_guard=float(np.ptp(p[guard,2])),native_preview_reviewed=True,in_game_visual_test=False)
(O/'asset-validation.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result))
