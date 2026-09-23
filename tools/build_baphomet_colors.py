"""Package the skin editor and encode its saved atlas; keep native geometry/stats unchanged."""
from pathlib import Path
from io import BytesIO
import argparse,hashlib,json,shutil,struct,sys,zlib
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'runtime/pylibs'))
from PIL import Image
ASSET=ROOT/'assets/primordial-baphomet';TOOL=ROOT/'client-overlay/Tools/BaphometColors'

def main():
 parser=argparse.ArgumentParser();parser.add_argument('--initialize',action='store_true');args=parser.parse_args()
 base=Image.open(ASSET/'body-red-features-imagegen.png').convert('RGB').resize((1024,1024),Image.Resampling.LANCZOS)
 base.save(TOOL/'base-body.png')
 if args.initialize:
  base.save(ASSET/'body-atlas.png')
  (ASSET/'skin-settings.json').write_text(json.dumps(dict(color='#8A958D',strength=0,brightness=1),indent=2)+'\n')
 shutil.copy2(ASSET/'skin-settings.json',TOOL/'settings.json')
 for name in ['blade-atlas.png','cm_0018-original.png','cm_0018_it-original.png']:
  shutil.copy2(ASSET/name,TOOL/name)
 data=json.loads((ASSET/'model.json').read_text());data['textures']['primordial'][0]='base-body.png'
 (TOOL/'model.json').write_text(json.dumps(data,separators=(',',':')))
 im=Image.open(ASSET/'body-atlas.png').convert('RGB');assert im.size==(1024,1024)
 buf=BytesIO();im.save(buf,format='BMP');raw=buf.getvalue()
 target=ROOT/'client-overlay/Texture/Monster/mt_prime_body.wtm'
 target.write_bytes(b'TEAMMAY\0\0'+struct.pack('<I',len(raw))+zlib.compress(raw,9))
 print(json.dumps(dict(texture_sha256=hashlib.sha256(target.read_bytes()).hexdigest(),editor=str(TOOL/'Open.cmd'))))
if __name__=='__main__':main()
