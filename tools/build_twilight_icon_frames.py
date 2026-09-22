"""Reuse the established native item frame, without inventing a new UI border."""
from pathlib import Path
import sys,io,struct,zlib,json
R=Path(__file__).resolve().parents[1];sys.path.insert(0,str(R/'runtime/pylibs'))
from PIL import Image
def unpack(p):return Image.open(io.BytesIO(zlib.decompress(p.read_bytes()[13:]))).convert('RGB')
def pack(im,p):
 b=io.BytesIO();im.save(b,format='BMP');raw=b.getvalue();p.write_bytes(b'TEAMMAY\0\0'+struct.pack('<I',len(raw))+zlib.compress(raw,9))
def main():
 peer=unpack(R/'runtime/client/GameClient/Item/ws_0081.wtm');assert peer.size==(28,28)
 report=[]
 for kind in ['twilight','longbow','staff']:
  p=R/f'client-overlay/Item/mt_{kind}_icon.wtm';im=unpack(p);assert im.size==peer.size
  for y in range(28):
   for x in range(28):
    if x<2 or x>=26 or y<2 or y>=26:im.putpixel((x,y),peer.getpixel((x,y)))
  for folder in ['Item','Texture/Body']:pack(im,R/f'client-overlay/{folder}/mt_{kind}_icon.wtm')
  im.save(R/f'assets/skills140/mt_{kind}_icon.png');report.append(dict(item=kind,source='ws_0081.wtm',border_width=2,identical_border=True))
 (R/'assets/skills140/icon-frame-validation.json').write_text(json.dumps(report,indent=2))
 print('All three icons use the identical Black Knight 2-pixel gold frame')
if __name__=='__main__':main()
