"""Encode the approved ImageGen icon at the game's native 28px size."""
from pathlib import Path
from io import BytesIO
import struct,zlib,subprocess,sys
O=Path(__file__).resolve().parent

def main():
    root=O.parents[1]
    if (root/'assets/twilight-set/icons-current/manifest.json').is_file():
        subprocess.run([sys.executable,str(root/'tools/build_current_weapon_icons.py')],check=True,cwd=root)
        return
    from PIL import Image
    icon=Image.open(O/'inventory_icon_imagegen.png').convert('RGBA')
    background=Image.new('RGBA',icon.size,'black');background.alpha_composite(icon)
    icon=background.convert('RGB').resize((28,28),Image.Resampling.LANCZOS)
    icon.save(O/'inventory_icon_28.png')
    stream=BytesIO();icon.save(stream,format='BMP');raw=stream.getvalue()
    (O/'payload').mkdir(exist_ok=True)
    (O/'payload/mt_twilight_icon.wtm').write_bytes(b'TEAMMAY\0\0'+struct.pack('<I',len(raw))+zlib.compress(raw,9))

if __name__=='__main__':main()
