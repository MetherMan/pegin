from pathlib import Path
import shutil,subprocess,sys,runpy,json
ROOT=Path(__file__).resolve().parents[1];R=ROOT/'runtime';O=ROOT/'assets/twilight'
active=ROOT/'assets/twilight-set/active-design.json'
if active.exists() and json.loads(active.read_text())['design'] in ['shattered-moon','noble-blue-moon']:
    builder='tools/build_noble_moon.py' if json.loads(active.read_text())['design']=='noble-blue-moon' else 'tools/build_shattered_moon.py'
    for script in [builder,'assets/twilight-set/preview_native.py','assets/twilight-set/verify_closed_surfaces.py']:
        subprocess.run([sys.executable,str(ROOT/script)],check=True,cwd=ROOT)
    sys.exit(0)
sys.path.insert(0,str(R/'pylibs'))
subprocess.run([sys.executable,str(ROOT/'assets/twilight-set/build_sword_volume.py')],check=True)
subprocess.run([str(R/'node/node.exe'),str(O/'build_model.mjs')],check=True)
runpy.run_path(str(O/'convert_native.py'),run_name='__main__')
runpy.run_path(str(O/'build_icon.py'),run_name='__main__')
dest=ROOT/'client-overlay'
for p in (O/'payload').glob('mt_twilight_*.mod'):shutil.copy2(p,dest/'Equip'/p.name)
for p in (O/'payload').glob('mt_mat_*.wtm'):shutil.copy2(p,dest/'Texture/Equip'/p.name)
shutil.copy2(O/'payload/mt_twilight_atlas.wtm',dest/'Texture/Equip/mt_twilight_atlas.wtm')
shutil.copy2(O/'payload/mt_twilight_icon.wtm',dest/'Item/mt_twilight_icon.wtm')
shutil.copy2(O/'payload/mt_mat_00.wtm',dest/'Texture/Body/mt_twilight_icon.wtm')
scale=json.loads((O/'size_spec.json').read_text())['scale'];trace=dest/'Equip/Trace.txt';lines=trace.read_bytes().decode('cp949').splitlines()
for i,line in enumerate(lines):
 if line.split() and line.split()[0] in [f'mt_twilight_{lod}.mod' for lod in [1,2,3]]:
  lines[i]=f'{line.split()[0]} twilight_blue {0.2*scale:.7f} 0.0 0.0 {2.2*scale:.7f} 0.0 0.0'
trace.write_bytes(('\r\n'.join(lines)+'\r\n').encode('cp949'))
print('Twilight rebuilt into client-overlay. Run Update-Workspace.cmd to apply.')
subprocess.run([sys.executable,str(ROOT/'tools/build_twilight_icon_frames.py')],check=True,cwd=ROOT)
