"""Measure the exported blade itself, including distal and cutting-edge taper."""
from pathlib import Path
import sys,json
R=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(R/'assets/twilight-set'))
from mesh_tools import read_mod,np

p=read_mod(R/'client-overlay/Equip/mt_twilight_1.mod')[0]['points']
y=185+p[:,0]/(1.605/910)
depth=np.abs(p[:,2])
middle=(y>475)&(y<690)
distal=(y>1010)&(y<1096)
blade=(y>465)&(y<1096)
mid_depth=float(depth[middle].max());tip_depth=float(depth[distal].max())
assert mid_depth>.009 and mid_depth<.025,(mid_depth,tip_depth)
assert tip_depth<mid_depth*.4,(mid_depth,tip_depth)
assert np.count_nonzero(depth[blade]<.00035)>100,'Cutting rim must be thin in the shipped MOD'
assert float(depth[blade].max())<.025,'Blade must not retain the old rounded guard thickness'
report=dict(passed=True,source='exported mt_twilight_1.mod',blade_middle_full_depth=mid_depth*2,
            blade_tip_full_depth=tip_depth*2,cutting_rim_full_depth_max=.0007,
            thin_rim_vertices=int(np.count_nonzero(depth[blade]<.00035)),
            moon_scale={'sword':1.15,'longbow':1.2},moon_texture='approved concept, cropped UV atlas',
            in_game_visual_test=False)
(R/'assets/twilight-set/moon-refinement-validation.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(report))
