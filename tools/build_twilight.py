from pathlib import Path
import shutil,subprocess,sys,runpy,json
ROOT=Path(__file__).resolve().parents[1];R=ROOT/'runtime';O=ROOT/'assets/twilight'
sys.path.insert(0,str(R/'pylibs'))
subprocess.run([str(R/'node/node.exe'),str(O/'build_model.mjs')],check=True)
runpy.run_path(str(O/'convert_native.py'),run_name='__main__')
dest=ROOT/'client-overlay'
for p in (O/'payload').glob('mt_twilight_*.mod'):shutil.copy2(p,dest/'Equip'/p.name)
for p in (O/'payload').glob('mt_mat_*.wtm'):shutil.copy2(p,dest/'Texture/Equip'/p.name)
shutil.copy2(O/'payload/mt_twilight_atlas.wtm',dest/'Texture/Equip/mt_twilight_atlas.wtm')
shutil.copy2(O/'payload/mt_mat_00.wtm',dest/'Texture/Body/mt_twilight_icon.wtm')
scale=json.loads((O/'size_spec.json').read_text())['scale'];trace=dest/'Equip/Trace.txt';lines=trace.read_bytes().decode('cp949').splitlines()
for i,line in enumerate(lines):
 if line.split() and line.split()[0] in [f'mt_twilight_{lod}.mod' for lod in [1,2,3]]:
  lines[i]=f'{line.split()[0]} twilight_blue {0.2*scale:.7f} 0.0 0.0 {2.2*scale:.7f} 0.0 0.0'
trace.write_bytes(('\r\n'.join(lines)+'\r\n').encode('cp949'))
print('Twilight rebuilt into client-overlay. Run Update-Workspace.cmd to apply.')
