from pathlib import Path
import sys,zlib,io
R=Path(__file__).resolve().parents[1];sys.path.insert(0,str(R/'runtime/pylibs'))
from PIL import Image,ImageDraw
C=R/'runtime/client/GameClient';O=R/'assets/skills140';O.mkdir(exist_ok=True)
rows=[l.split('\t') for l in (R/'game-data/DATA/ITEM_DATA.txt').read_bytes().decode('cp949').splitlines() if l.split('\t')[0].isdigit()]
names=['블랙나이트양손검','레드나이트양손검','화이트헌터롱보우','달의정령스태프']
files=[('original '+str(r[0]),C/'Item'/Path(r[58]).with_suffix('.wtm')) for r in rows if r[2] in names]
files += [('twilight '+k,R/f'client-overlay/Item/mt_{k}_icon.wtm') for k in ['twilight','longbow','staff']]
sheet=Image.new('RGB',(180*len(files),210),'#333333');d=ImageDraw.Draw(sheet)
for i,(name,p) in enumerate(files):
 print(name,p.name)
 im=Image.open(io.BytesIO(zlib.decompress(p.read_bytes()[13:]))).convert('RGB')
 im.save(O/(p.stem+'.png'));sheet.paste(im.resize((168,168),Image.Resampling.NEAREST),(i*180,30));d.text((i*180,5),name)
sheet.save(O/'icon-audit.png')
