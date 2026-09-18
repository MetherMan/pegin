"""Validate actual native geometry, aperture rays, size and encoded icon."""
from pathlib import Path
from io import BytesIO
import hashlib,json,struct,zlib
import numpy as np
from PIL import Image
from verify_native_animation import read_mod
O=Path(__file__).resolve().parent
info=json.loads((O/'model_info.json').read_text(encoding='utf-8'));spec=json.loads((O/'size_spec.json').read_text());t=info['construction_transform']
parts=read_mod(O/'payload/mt_twilight_1.mod');assert len(parts)==1
part=parts[0];vertices=part['points'];tri=vertices[part['corners']['index']].reshape(-1,3,3)
assert abs(vertices[:,0].max()-spec['reference_grip_to_tip'])<1e-6
assert abs(np.ptp(vertices[:,0])-spec['new_length'])<1e-6
assert np.ptp(vertices[:,0])<spec['reference_length']
assert part['texture']=='mt_twilight_atlas.bmp'

def ray_hits_pixel(x,y):
    point=np.array([t['y_offset']-y*t['pixel_scale']-.5,-(x-t['x_origin'])*t['pixel_scale']])
    a=tri[:,0,:2];v0=tri[:,1,:2]-a;v1=tri[:,2,:2]-a;v2=point-a
    cross=lambda u,v:u[:,0]*v[:,1]-u[:,1]*v[:,0]
    det=cross(v0,v1);valid=np.abs(det)>1e-10;safe=np.where(valid,det,1)
    u=cross(v2,v1)/safe;v=cross(v0,v2)/safe
    return int(np.count_nonzero(valid&(u>=-1e-6)&(v>=-1e-6)&(u+v<=1+1e-6)))

holes={'round_blade_aperture':(486,756),'upper_openwork':(515,400),'crescent_pommel':(486,1360)}
for name,point in holes.items():assert ray_hits_pixel(*point)==0,(name,ray_hits_pixel(*point))
solid={'main_blue_blade':(430,500),'left_auxiliary_cutter':(332,800),'long_right_cutter':(610,550),'middle_right_cutter':(636,695),'short_right_cutter':(592,848),'round_grip':(487,1130)}
for name,point in solid.items():assert ray_hits_pixel(*point)>0,name
for lod in [2,3]:assert (O/f'payload/mt_twilight_{lod}.mod').read_bytes()==(O/'payload/mt_twilight_1.mod').read_bytes()
encoded=(O/'payload/mt_twilight_icon.wtm').read_bytes();raw=zlib.decompress(encoded[13:]);assert len(raw)==struct.unpack_from('<I',encoded,9)[0]
icon=Image.open(BytesIO(raw)).convert('RGB');assert icon.size==(28,28)
assert np.array_equal(np.asarray(icon),np.asarray(Image.open(O/'inventory_icon_28.png').convert('RGB')))
result=dict(passed=True,revision='approved-04',all_three_lods_equal=True,animation_meshes=1,native_triangles=len(tri),open_apertures=list(holes),solid_blades_verified=list(solid),length=float(np.ptp(vertices[:,0])),grip_to_tip=float(vertices[:,0].max()),icon_size=list(icon.size),icon_sha256=hashlib.sha256(encoded).hexdigest(),in_game_visual_test=False)
(O/'design04_validation.json').write_text(json.dumps(result,indent=2)+'\n')
print(json.dumps(result))
