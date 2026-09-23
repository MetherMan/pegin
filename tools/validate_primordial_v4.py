"""Read native Baphomet v4 bytes and check size, rigid grip and texture packing."""
from pathlib import Path
from collections import Counter
from io import BytesIO, StringIO
import contextlib, copy, hashlib, json, math, sys, zlib

R=Path(__file__).resolve().parents[1]
sys.path[:0]=[str(R/'runtime/pylibs'),str(R/'client-overlay/Tools/SkillColors')]
from PIL import Image
from native_actor import model
from primordial_geometry import BASELINE_SCALE,TARGET_SCALE,GRIP_PIVOT,HAND_BONE,scaled_animation
from sculpt_primordial_baphomet import face,tube,main as rebuild
from antler_revision import build_cleaver
from build_primordial_baphomet import without_variants,wad_read,wad_write

O=R/'assets/primordial-baphomet';D=R/'client-overlay';C=R/'runtime/client/GameClient'
FACTOR=TARGET_SCALE/BASELINE_SCALE
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def distance(a,b):return max(abs(x-y) for x,y in zip(a,b))

def inverse(values):
    m=[[values[j*4+i] for j in range(4)]+[float(i==j) for j in range(4)] for i in range(4)]
    for j in range(4):
        pivot=max(range(j,4),key=lambda i:abs(m[i][j]));m[pivot],m[j]=m[j],m[pivot]
        divisor=m[j][j];assert abs(divisor)>1e-12
        m[j]=[v/divisor for v in m[j]]
        for i in range(4):
            if i!=j:
                f=m[i][j];m[i]=[a-f*b for a,b in zip(m[i],m[j])]
    return [row[4:] for row in m]

def skin(point,bone,frame):
    inv=inverse(bone['rest']);p=list(point)+[1]
    local=[sum(row[j]*p[j] for j in range(4)) for row in inv]
    pose=bone['poses'][frame];x,y,z,w=-pose[3],-pose[4],-pose[5],pose[6]
    q=[[1-2*(y*y+z*z),2*(x*y-z*w),2*(x*z+y*w)],
       [2*(x*y+z*w),1-2*(x*x+z*z),2*(y*z-x*w)],
       [2*(x*z-y*w),2*(y*z+x*w),1-2*(x*x+y*y)]]
    return [sum(row[j]*local[j] for j in range(3))+pose[i] for i,row in enumerate(q)]

def roll(p):return [2*GRIP_PIVOT[0]-p[0],2*GRIP_PIVOT[1]-p[1],p[2]]
def at_scale(p):return [v*FACTOR for v in p]
def pixel(x,y):return [GRIP_PIVOT[0]+(y-418)*.0033,GRIP_PIVOT[1],(x-1200)*.0033]

def main():
    paths=[D/'Monster/mt_prime_baphomet.mod',D/'Texture/Monster/mt_prime_body.wtm',D/'Texture/Monster/mt_prime_cleaver.wtm']
    paths+=sorted((D/'Monster/Animation').glob('mt_prime_*.ani'))
    paths += [O/'model.json',O/'body-cyclops-atlas.png',O/'cleaver-atlas.png']
    protected=[D/'Monster/monster.wad',D/'Monster/monster.dat',D/'Monster/MobInfo.dat',R/'game-data/DATA/MOB_DATA.txt',R/'game-data/DATA/MobItemLoseTable.txt']
    protected_hashes={str(p.relative_to(R)):sha(p) for p in protected}
    before={str(p.relative_to(R)):sha(p) for p in paths}
    with contextlib.redirect_stdout(StringIO()):rebuild()
    after={str(p.relative_to(R)):sha(p) for p in paths}
    assert before==after,'Second build changed resources (accumulating scale or nondeterminism)'
    assert protected_hashes=={str(p.relative_to(R)):sha(p) for p in protected},'Geometry mode modified a table'
    chunks=model((D/'Monster/mt_prime_baphomet.mod').read_bytes())
    original=model((C/'Monster/p-warrior.mod').read_bytes())
    data=json.loads((O/'model.json').read_text(encoding='utf-8'))
    assert data['primordial']==json.loads(json.dumps(chunks))
    body_error=max(distance(q,[x*TARGET_SCALE for x in p]) for p,q in zip(original[0]['points'],chunks[0]['points'][:809]))
    assert body_error<1e-5
    assert chunks[0]['bones'][:809]==original[0]['bones']
    assert set(chunks[1]['bones'])=={HAND_BONE}
    baseline=model((O/'sculpt-baseline.mod').read_bytes())
    blade=copy.deepcopy(baseline[1]);build_cleaver(blade,face,tube,roll_degrees=0)
    expected=copy.deepcopy(blade);build_cleaver(expected,face,tube,roll_degrees=180)
    roll_error=max(distance(roll(p),q) for p,q in zip(blade['points'],expected['points']))
    assert roll_error<1e-10
    assert all(p[2]==q[2] for p,q in zip(blade['points'],expected['points']))
    assert roll(GRIP_PIVOT)==list(GRIP_PIVOT)
    assert all(a[1]==-b[1] and a[2]==-b[2] and a[3]==b[3] for a,b in zip(blade['corners'],expected['corners']))
    # Welded position edge counts verify the complete new sword is closed.
    edges=Counter()
    for k in range(0,len(chunks[1]['corners']),3):
        pts=[tuple(round(x,6) for x in chunks[1]['points'][c[0]]) for c in chunks[1]['corners'][k:k+3]]
        for a,b in zip(pts,pts[1:]+pts[:1]):edges[tuple(sorted((a,b)))]+=1
    boundaries=sum(v==1 for v in edges.values());nonmanifold=sum(v>2 for v in edges.values())
    assert boundaries==nonmanifold==0
    animation_checks={};motion_checks={}
    for name,clips in data['clips'].items():
        raw=(C/'Monster/Animation'/name).read_bytes()
        result,old,new=scaled_animation(raw,TARGET_SCALE)
        output='mt_prime_'+name.removeprefix('p-warrior_bla_')
        assert result==(D/'Monster/Animation'/output).read_bytes()
        assert new==clips['primordial'] and old==clips['original']
        animation_checks[name]=dict(frames=new['frames'],bones=29,rotations_exact=True,source_translation_scale=TARGET_SCALE,sha256=sha(D/'Monster/Animation'/output))
    for label,motion in data['motions'].items():
        name=motion['clip'];raw=(C/'Monster/Animation'/name).read_bytes()
        old=scaled_animation(raw,BASELINE_SCALE)[2];new=data['clips'][name]['primordial']
        old_bone=old['bones'][HAND_BONE];new_bone=new['bones'][HAND_BONE]
        errors=[];axis_errors=[];surface_errors=[];before_edge_y=[];after_edge_y=[]
        # Two matched cross-section points on the actual artwork outline.
        edge=pixel(505,644);spine=pixel(505,407)
        for frame in range(motion['first'],motion['last']+1):
            old_grip=skin(GRIP_PIVOT,old_bone,frame);new_grip=skin(at_scale(GRIP_PIVOT),new_bone,frame)
            errors.append(distance(new_grip,at_scale(old_grip)))
            axis_point=[GRIP_PIVOT[0],GRIP_PIVOT[1],-3.5]
            axis_errors.append(distance(skin(at_scale(axis_point),new_bone,frame),skin(at_scale(roll(axis_point)),new_bone,frame)))
            a,b=skin(at_scale(edge),new_bone,frame),skin(at_scale(spine),new_bone,frame)
            c,d=skin(at_scale(roll(edge)),new_bone,frame),skin(at_scale(roll(spine)),new_bone,frame)
            before_edge_y.append(a[1]-b[1]);after_edge_y.append(c[1]-d[1])
            for p,q in zip(blade['points'][::113],expected['points'][::113]):
                # Rolling before skinning must be the rigid bone-space roll.
                surface_errors.append(distance(skin(at_scale(q),new_bone,frame),skin(at_scale(roll(p)),new_bone,frame)))
        assert max(errors)<1e-5 and max(axis_errors)<1e-10 and max(surface_errors)<1e-10
        assert all(abs(a+b)<1e-6 for a,b in zip(before_edge_y,after_edge_y))
        motion_checks[label]=dict(frames_checked=len(errors),grip_scale_error=max(errors),long_axis_movement_after_roll=max(axis_errors),rigid_blade_roll_error=max(surface_errors),first_frame=motion['first'],edge_minus_spine_world_y_before=before_edge_y[0],edge_minus_spine_world_y_after=after_edge_y[0],edge_y_range_after=[min(after_edge_y),max(after_edge_y)])
    assert motion_checks['idle']['edge_minus_spine_world_y_before']>0
    assert motion_checks['idle']['edge_minus_spine_world_y_after']<0
    texture_checks=[]
    for source,preview,native,width in [('body-matte-v4-imagegen.png','body-cyclops-atlas.png','mt_prime_body.wtm',1024),('../visual-refresh-20260923/baphomet-antler-cleaver-concept.png','cleaver-atlas.png','mt_prime_cleaver.wtm',1536)]:
        im=Image.open(O/preview).convert('RGB')
        expected_face=Image.open(O/source).convert('RGB').resize((width,1024),Image.Resampling.LANCZOS)
        expected_horn=Image.open(O/'horn-material-matte-v4-imagegen.png').convert('RGB').resize((2048-width,1024),Image.Resampling.LANCZOS)
        native_im=Image.open(BytesIO(zlib.decompress((D/'Texture/Monster'/native).read_bytes()[13:]))).convert('RGB')
        assert im.tobytes()==native_im.tobytes()
        assert im.crop((0,0,width,1024)).tobytes()==expected_face.tobytes()
        assert im.crop((width,0,2048,1024)).tobytes()==expected_horn.tobytes()
        texture_checks.append(dict(native=native,source=source,source_to_atlas_exact=True,wtm_to_png_exact=True))
    # Exercise safe replacement helpers against current, already-populated tables.
    strings,rows=wad_read((D/'Monster/monster.wad').read_bytes())
    assert wad_write(strings,rows)==(D/'Monster/monster.wad').read_bytes()
    test=['44\tkeep','80\told','280\told','777\tkeep','header']
    assert without_variants(test)==['44\tkeep','777\tkeep','header']
    report=dict(passed=True,revision='matte-down-v4',scale=TARGET_SCALE,baseline_scale=BASELINE_SCALE,sculpt_scale_factor=FACTOR,
                native_body_scale_max_error=body_error,hand_bone=HAND_BONE,grip_pivot_baseline=GRIP_PIVOT,grip_pivot_current=at_scale(GRIP_PIVOT),
                blade_roll_degrees=180,blade_roll_axis=[0,0,1],blade_not_reversed=True,rest_blade_roll_error=roll_error,
                closed_cleaver=dict(boundary_edges=boundaries,nonmanifold_edges=nonmanifold,triangles=len(chunks[1]['corners'])//3),
                motions=motion_checks,animations=animation_checks,texture_checks=texture_checks,
                repeat_build_identical=True,protected_tables_unchanged=protected_hashes,resource_hashes=after,
                in_game_visual_test=False,note='Grip preservation is measured relative to the uniformly resized hand. All motion rotations are original. The legacy original body is not asserted watertight; new cleaver is closed.')
    (O/'v4-geometry-validation.json').write_text(json.dumps(report,ensure_ascii=False,indent=2)+'\n',encoding='utf-8')
    print(json.dumps(dict(passed=True,revision=report['revision'],scale=TARGET_SCALE,motions=motion_checks,closed_cleaver=report['closed_cleaver'],repeat_build_identical=True),ensure_ascii=False))

if __name__=='__main__':main()
