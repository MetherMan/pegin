from pathlib import Path
import sys,re,struct,zlib
R=Path(__file__).resolve().parents[1];C=R/'runtime/client/GameClient'
sys.path[:0]=[str(R/'runtime/pylibs')]
def read(p):return p.read_bytes().decode('cp949')
items={int(r[0]):r for l in read(R/'game-data/DATA/ITEM_DATA.txt').splitlines() if (r:=l.split('\t'))[0].isdigit()}
for n in [11424,11739,11865,19030,19060,19090,12044,12045,12048,10104,10105,10106,10107,10108,10109,10111,12091]:
 print('ITEM',n,items[n][2:6],items[n][55:])
skills={int(r[3]):r for l in read(R/'game-data/DATA/skill_data.txt').splitlines() if (r:=l.split('\t'))[0].isdigit()}
magic={r[1]:r[2] for l in read(C/'Magic/MagicList.ml').splitlines() if (r:=l.split()) and r[0]=='[MAGIC]'}
anim=read(C/'Skill/Skill.dat')
for n in [12044,12045,12048,10104,10105,10106,10107,10108,10109,10111,12091]:
 s=skills[n];print('SKILL',s)
 a=next((a for a in anim.split('[SKILL]') if re.search(r'\[NAME\]\s*'+re.escape(items[n][3])+r'\s',a)),None)
 print('ANIMATION',a)
 keys=[items[n][3]+'A',items[n][3]+'B'] if n<12000 or n==12091 else [re.search(r'\[EFFECT\]\s+(\S+)',a).group(1)] if a else []
 for key in keys:
  if key in magic:print('MAGIC',key,magic[key],read(C/'Magic'/magic[key]))
