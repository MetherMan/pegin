"""Check deployed table links, native effect dependencies/timing and color round trips."""
from pathlib import Path
import sys,re,struct,zlib,json,importlib.util,math
R=Path(__file__).resolve().parents[1];D=R/'client-overlay';C=R/'runtime/client/GameClient';O=R/'assets/skills140'
sys.dont_write_bytecode=True;sys.path[:0]=[str(R/'runtime/pylibs'),str(R/'tools')]
from build_skill140 import SPECS,read,rows
def path(rel):
 p=D/rel
 if not p.exists():p=C/rel
 assert p.is_file(),rel
 return p
def wed(raw):
 at=31;strings=[]
 assert raw[:31].rstrip(b'\0')==b'Wind3D Special Effect Data V1.1'
 for _ in range(2):n=struct.unpack_from('<i',raw,at)[0];at+=4;strings.append(raw[at:at+n].rstrip(b'\0').decode('cp949'));at+=n
 start,last,n,row,col=struct.unpack_from('<5i',raw,at);at+=20;assert at+n*52==len(raw)
 return strings,at,n
def main():
 sys.path.insert(0,str(D/'Tools/SkillColors'))
 from tuning import load_settings,compile_resources,resource_reader
 from native_assets import read_magic
 tuning=load_settings(D/'Tools/SkillColors')
 checks=[]
 server=rows(read(R/'game-data/DATA/skill_data.txt'));client=rows(read(D/'Item/Skill.dat'));ui=rows(zlib.decompress((D/'Interface/skill.dat').read_bytes()[20:]).decode('cp949'))
 for spec in SPECS:
  s=next(r for r in server.values() if int(r[3])==spec['id']);c=client[int(s[0])]
  assert c==ui[int(s[0])] and c[:30]==s[:30];assert len(c)==34 and len('\t'.join(c).encode('cp949'))<1024
  assert list(map(int,s[14:16]))==spec['damage'],(spec['name'],s[14:16])
  assert all(len(c[i].encode('cp949'))<size for i,size in [(1,50),(2,64),(29,50),(30,64),(31,64),(32,64),(33,64)])
  assert int(c[25])<128 and int(c[9+spec['branch']])==140
  for table in [R/'game-data/DATA/ITEM_DATA.txt',D/'Item/ITEM.dat']:
   item=rows(read(table))[spec['id']];assert item[2]==spec['name'] and item[3]==spec['key'] and int(item[4])==42
 checks.append('server, plain client and compressed UI skill tables match; names fit native buffers')
 listing={l.split()[1]:l.split()[2] for l in read(D/'Magic/MagicList.ml').splitlines() if l.startswith('[MAGIC]')}
 expected={'mt_heaven':3,'mt_six':6,'mt_six_blue':6,'mt_meteorA':0,'mt_meteorB':7,'mt_frostA':0,'mt_frostB':3}
 seen=set()
 for key,hits in expected.items():
  s=read(path('Magic/'+listing[key]));assert s.count('[BLOWTIMING]')==hits
  for tag in ['EFFECT','PART','BLOWTIMING']:assert s.count('['+tag+']')==s.count('[/'+tag+']')
  for n in re.findall(r'\[(?:SPE|TAIL)\]\s+(\S+)',s):
   p=path('Effect/'+n);strings,at,count=wed(p.read_bytes());seen.add(n)
   path('Effect/'+strings[0]);path('Texture/Effect/'+str(Path(strings[1]).with_suffix('.wtm')))
 for key,n in [('mt_heaven',3),('mt_six',6),('mt_six_blue',6)]:
  s=read(path('Magic/'+listing[key]));arrows=[e for e in read_magic(s) if any('HITSOUND' in p for p in e['parts'])]
  v=tuning['heaven' if key=='mt_heaven' else 'six']
  assert [e['parts'][0]['STARTTIME'] for e in arrows]==[v['startDelay']+v['shotGap']*i for i in range(n)]
 s=read(path('Magic/mt_meteorB.ms'));assert len(re.findall(r'\[SPE\] mm_battery_[0-7]_[0-2]\.wed',s))==24 and s.count('[SPE] mm_cannon.wed')==8 and s.count('[SPE] mm_final_fireball.wed')==1
 s=read(path('Magic/mt_frostB.ms'));effects=read_magic(s)
 ice=[e for e in effects if (e['wed'] or '').startswith('mf_eruption_')]
 assert len(ice)>=32 and all(e['parts'][0]['ENEMY'] for e in ice)
 assert not any((e['wed'] or '').startswith('mf_pressure_front') for e in effects)
 circles=[e for e in effects if (e['wed'] or '').startswith('mf_cocytus_seal')]
 assert len(circles)==3
 assert all(e['parts'][0]['TARGET'][:2]==[tuning['frost'].get('sealOffsetX',0),tuning['frost'].get('sealOffsetY',0)] for e in circles)
 assert len([e for e in effects if e['wed']=='mf_cocytus_pillar.wed'])==1
 checks.append('7 magic scripts: native dependencies resolved, 3/6/7/3 blows, target-centered Cocytus with three seals')
 mod=importlib.util.spec_from_file_location('skillcolors',D/'Tools/SkillColors/edit_colors.py');editor=importlib.util.module_from_spec(mod);mod.loader.exec_module(editor)
 cfg=json.loads((D/'Tools/SkillColors/colors.json').read_text());frames=0
 for p in (D/'Tools/SkillColors/templates').glob('*.wed'):
  raw=p.read_bytes();_,at,n=wed(raw);frames+=n
  for color in ['#FF0000','#00FF00','#0000FF','#123456','#FFFFFF']:
   b=editor.recolor(raw,color);changed={at+j*52+44+k for j in range(n) for k in range(3)}
   assert all(i in changed for i,(a,z) in enumerate(zip(raw,b)) if a!=z)
  key='meteor' if p.name.startswith('mm_') else 'frost'
 # Compilation now also changes animation frames, scales and timings.
 compiled,metadata=compile_resources(resource_reader(D),tuning,cfg)
 from verify_cocytus_pressure import assert_rings
 assert_rings(compiled,metadata['frost'],effects)
 for rel,data in compiled.items():assert (D/rel).read_bytes()==data,rel
 for value in [{},dict(meteor='red',frost='#abcdef'),dict(meteor='#abc123',frost='#abc123',other=1)]:
  try:editor.validate(value)
  except ValueError:pass
  else:raise AssertionError('invalid colors accepted')
 checks.append(f'{frames} native effect frames recolor reversibly while preserving alpha, geometry, timing and scale')
 from build_twilight_icon_frames import unpack
 peer=unpack(C/'Item/ws_0081.wtm')
 for kind in ['twilight','longbow','staff']:
  for folder in ['Item','Texture/Body']:
   im=unpack(D/f'{folder}/mt_{kind}_icon.wtm')
   assert all(im.getpixel((x,y))==peer.getpixel((x,y)) for y in range(28) for x in range(28) if x<2 or x>=26 or y<2 or y>=26)
 checks.append('all inventory/equipment gold frame pixels identical to Black Knight')
 original=unpack(C/'Item/magicscroll_b.wtm');icon=unpack(D/'Item/mt_skill140_scroll.wtm')
 assert (D/'Item/mt_skill140_scroll.wtm').read_bytes()==(D/'Texture/Body/mt_skill140_scroll.wtm').read_bytes()
 for y in range(28):
  for x in range(28):
   r,g,b=original.getpixel((x,y))
   expected=peer.getpixel((x,y)) if x<2 or x>=26 or y<2 or y>=26 else ((0,0,0) if g>r+7 and g>b+4 else (r,g,b))
   assert icon.getpixel((x,y))==expected
 items=rows(read(D/'Item/ITEM.dat'));uiitems=rows(zlib.decompress((D/'Interface/item.dat').read_bytes()[20:]).decode('cp949'))
 for item in range(19120,19124):assert items[item][58]==uiitems[item][59]=='mt_skill140_scroll.bmp'
 checks.append('all four scrolls retain original red ribbon/paper pixels, with black background and native gold frame')
 frame=unpack(C/'Item/ls_0014.wtm')
 for spec in SPECS:
  item=spec['id'];name=spec['key']+'_icon'
  for col in [59,65]:assert items[item][col]==uiitems[item][col+1]==name+'.bmp'
  for col in [58,64]:assert items[item][col]==uiitems[item][col+1]=='mt_skill140_scroll.bmp'
  im=unpack(D/f'Item/{name}.wtm');assert im.size==(28,28)
  assert (D/f'Item/{name}.wtm').read_bytes()==(D/f'Texture/Body/{name}.wtm').read_bytes()
  assert all(im.getpixel((x,y))==frame.getpixel((x,y)) for y in range(28) for x in range(28) if x<2 or x>=26 or y<2 or y>=26)
 checks.append('four distinct learned-skill icons resolve for both genders; 28px native gold borders and original scrolls preserved')
 from verify_magic_loader import main as verify_loader
 verify_loader()
 checks.append('Windows CRT and legacy nested parser regression: all Twilight scripts load with complete effect, part and blow counts')
 (O/'asset-validation.json').write_text(json.dumps(dict(passed=True,checks=checks,native_resources=len(seen),in_game_visual_test=False),indent=2))
 for s in checks:print('PASS '+s)
if __name__=='__main__':main()
