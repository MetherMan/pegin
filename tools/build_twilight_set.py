"""Rebuild the two weapons and their inventory/effect resources."""
from pathlib import Path
import subprocess,sys,json
ROOT=Path(__file__).resolve().parents[1];O=ROOT/'assets/twilight-set'
for script in ['build_models.py','build_effects_icons.py','register_items.py','patch_client.py','preview_native.py','verify_closed_surfaces.py','verify_assets.py']:
    target=O/script
    if script=='build_models.py' and (O/'active-design.json').exists():
        design=json.loads((O/'active-design.json').read_text())['design']
        if design=='shattered-moon':target=ROOT/'tools/build_shattered_moon.py'
        elif design=='noble-blue-moon':target=ROOT/'tools/build_noble_moon.py'
    subprocess.run([sys.executable,str(target)],check=True,cwd=ROOT)
subprocess.run([sys.executable,str(ROOT/'tools/build_twilight_icon_frames.py')],check=True,cwd=ROOT)
