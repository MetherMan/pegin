"""Add two independent 0..20 enhancement families without rewriting old rows."""
from pathlib import Path
import json,struct,zlib,re
O=Path(__file__).resolve().parent;ROOT=O.parents[1]
FAMILIES=[dict(base=19060,peer=11739,type=12,name='푸른달의 트와일라잇 롱보우',prefix='mt_longbow'),dict(base=19090,peer=11865,type=18,name='푸른달의 트와일라잇 스태프',prefix='mt_staff')]
def rows(raw):return {int(r[0]):r for l in raw.decode('cp949').splitlines() if (r:=l.split('\t'))[0].isdigit()}
source=rows((ROOT/'game-data/DATA/ITEM_DATA.txt').read_bytes());new=[];definitions=[]
for family in FAMILIES:
    for level in range(21):
        r=source[family['peer']+level].copy();reference=source[11424+level];sword=source[19030+level]
        ratio=[int(sword[i])/int(reference[i]) for i in [6,7]]
        r[0]=str(family['base']+level);r[2]=r[3]=(f'+{level} ' if level else '')+family['name'];r[5]='9'
        for i in [6,7,8,9]:r[i]=str(round(int(r[i])*ratio[(i-6)%2]))
        for i in [10,11,12,19,20,21,22,23]+list(range(27,55)):r[i]='0'
        for start in [55,61]:r[start:start+5]=[f"{family['prefix']}_{lod}.mod" for lod in [1,2,3]]+[family['prefix']+'_icon.tga']*2
        new.append(r);definitions.append(dict(id=int(r[0]),level=level,name=r[2],type=int(r[4]),rank=9,min=int(r[6]),max=int(r[7]),magic_min=int(r[8]),magic_max=int(r[9]),weight=0,peer_id=family['peer']+level,sword_ratio=ratio))
newids={int(r[0]) for r in new}
def merge(raw,values):
    before=rows(raw);lines=raw.decode('cp949').splitlines();lines=[l for l in lines if not (l.split('\t')[0].isdigit() and int(l.split('\t')[0]) in newids)]
    last=max(i for i,l in enumerate(lines) if l.split('\t')[0].isdigit());lines[last+1:last+1]=['\t'.join(r) for r in values]
    raw=('\r\n'.join(lines)+'\r\n').encode('cp949');after=rows(raw)
    assert all(after[i]==r for i,r in before.items() if i not in newids)
    return raw
for rel in ['game-data/DATA/ITEM_DATA.txt','client-overlay/Item/ITEM.dat']:
    p=ROOT/rel;p.write_bytes(merge(p.read_bytes(),new))
p=ROOT/'client-overlay/Interface/item.dat';raw=p.read_bytes();ui=[]
for r in new:
    u=r[:5]+['0']+r[5:];u[2]=u[3]=next(f['name'] for f in FAMILIES if f['base']<=int(r[0])<=f['base']+20)
    assert len(('(+20)'+u[2]).encode('cp949'))<32
    ui.append(u)
data=merge(zlib.decompress(raw[20:]),ui);p.write_bytes(raw[:16]+struct.pack('<I',len(data))+zlib.compress(data,9))
p=ROOT/'src/server/LAQIA_GameServer/gm_enhance_data.h';s=p.read_text();entries=[tuple(map(int,g)) for g in re.findall(r'\{(\d+),(\d+),(\d+)\}',s)]
entries=[e for e in entries if e[0] not in newids]+[(f['base']+level,f['base'],level) for f in FAMILIES for level in range(21)]
s=s[:s.index('static const GMEnhanceEntry')]+'static const GMEnhanceEntry gmEnhanceEntries[]={\n'+''.join('{%d,%d,%d},\n'%e for e in sorted(entries))+'};\n';p.write_text(s)
p=ROOT/'src/server/utf8/gm_parser.utf8.h';s=p.read_text(encoding='utf-8');old='gmEqual(t[2],"2")?19030:0';newcode='gmEqual(t[2],"2")?19030:(gmEqual(t[2],"3")?19060:(gmEqual(t[2],"4")?19090:0))'
if old in s:s=s.replace(old,newcode)
assert newcode in s;p.write_text(s,encoding='utf-8');(ROOT/'src/server/LAQIA_GameServer/gm_parser.h').write_bytes(s.encode('cp949'))
p=ROOT/'src/server/utf8/gm_commands.utf8.cpp';s=p.read_text(encoding='utf-8');needle='  SendSystemMsg(p,"/재뽕 테스트 2 = 푸른달의 트와일라잇 양손검 지급 (Rank 9)");'
if '/재뽕 테스트 3 =' not in s:
    assert needle in s;s=s.replace(needle,needle+'\n  SendSystemMsg(p,"/재뽕 테스트 3 = 푸른달의 트와일라잇 롱보우 지급 (Rank 9)");\n  SendSystemMsg(p,"/재뽕 테스트 4 = 푸른달의 트와일라잇 스태프 지급 (Rank 9)");')
p.write_text(s,encoding='utf-8');(ROOT/'src/server/LAQIA_GameServer/gm_commands.cpp').write_bytes(s.encode('cp949'))
(O/'item_definitions.json').write_text(json.dumps(definitions,ensure_ascii=False,indent=2)+'\n',encoding='utf-8')
print(json.dumps([r for r in definitions if r['level'] in [0,20]],ensure_ascii=False))
