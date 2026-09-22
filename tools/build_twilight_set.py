"""Rebuild the two weapons and their inventory/effect resources."""
from pathlib import Path
import subprocess,sys
ROOT=Path(__file__).resolve().parents[1];O=ROOT/'assets/twilight-set'
for script in ['build_models.py','build_effects_icons.py','register_items.py','patch_client.py','preview_native.py','verify_closed_surfaces.py','verify_assets.py']:
    subprocess.run([sys.executable,str(O/script)],check=True,cwd=ROOT)
subprocess.run([sys.executable,str(ROOT/'tools/build_twilight_icon_frames.py')],check=True,cwd=ROOT)
