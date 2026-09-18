"""Encode the approved ImageGen icon at the game's native 28px size."""
from pathlib import Path
from io import BytesIO
import struct,zlib
from PIL import Image
O=Path(__file__).resolve().parent
icon=Image.open(O/'inventory_icon_imagegen.png').convert('RGBA')
background=Image.new('RGBA',icon.size,'black');background.alpha_composite(icon)
icon=background.convert('RGB').resize((28,28),Image.Resampling.LANCZOS)
icon.save(O/'inventory_icon_28.png')
stream=BytesIO();icon.save(stream,format='BMP');raw=stream.getvalue()
(O/'payload').mkdir(exist_ok=True)
(O/'payload/mt_twilight_icon.wtm').write_bytes(b'TEAMMAY\0\0'+struct.pack('<I',len(raw))+zlib.compress(raw,9))
