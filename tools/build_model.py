from pathlib import Path
import shutil,subprocess,sys
ROOT=Path(__file__).resolve().parents[1];R=ROOT/'runtime';O=ROOT/'assets/blood-knight/game_registration'
sys.path.insert(0,str(R/'pylibs'))
import runpy
subprocess.run([str(R/'node/node.exe'),str(O/'build_game_model.mjs')],check=True)
runpy.run_path(str(O/'convert_native.py'),run_name='__main__')
dest=ROOT/'client-overlay'
for p in (O/'payload').glob('*.mod'):shutil.copy2(p,dest/'Equip'/p.name)
for p in (O/'payload').glob('*.wtm'):shutil.copy2(p,dest/'Texture/Equip'/p.name)
shutil.copy2(O/'payload/bn_surface.wtm',dest/'Texture/Body/bn_blood.wtm')
print('Model rebuilt into client-overlay. Review with git diff, then run Update-Workspace.cmd to apply.')
