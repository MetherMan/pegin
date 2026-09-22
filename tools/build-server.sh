#!/bin/bash
set -eu
cd /opt/laqia/gm-build
flags="-m32 -std=gnu++98 -fpermissive -fno-pie -D_GLIBCXX_USE_CXX11_ABI=0 -I/usr/include/mariadb $(cat flags.txt)"
for unit in gm_commands main player item skill fight dataserver local_rates local_world shop; do
  output="$unit-gm.o"
  test "$unit" != gm_commands || output=gm_commands.o
  g++ $flags -g -finput-charset=CP949 -fexec-charset=CP949 -c "$unit.cpp" -o "$output" > "$unit-development.log" 2>&1 || { tail -70 "$unit-development.log"; exit 1; }
done
# The archived message.cpp contains unrelated unfinished billing code.
# Compile edited handlers only and override their legacy symbols.
python3 - <<'PY'
from pathlib import Path
import re,subprocess
s=Path('message.cpp').read_text(encoding='cp949')
names=['PACKET_EnchantItem','GetItemProc','PACKET_BuyItem','PACKET_OpenGarbage','PACKET_MoveInvenItem']
includes='\n'.join(line for line in s.splitlines() if line.startswith('#include '))
bodies=[]
for name in names:
    match=re.search(r'^(?:BOOL|BYTE) '+name+r'\(',s,re.M)
    if not match:raise RuntimeError('Missing handler '+name)
    start=match.start()
    end=re.search(r'\n(?:BOOL|BYTE|void) \w+\(',s[match.end():])
    bodies.append(s[start:match.end()+end.start() if end else len(s)])
Path('enchant_handler.cpp').write_bytes((includes+'\n'+'\n'.join(bodies)+'\n').encode('cp949'))
symbols=[]
for line in subprocess.check_output(['nm','--defined-only','message.o'],text=True).splitlines():
    symbol=line.split()[-1]
    if any(re.match(r'_Z\d+'+name+r'(?:P|R|v)',symbol) for name in names):symbols.append(symbol)
assert len(symbols)==len(names),(names,symbols)
subprocess.run(['objcopy']+['--weaken-symbol='+symbol for symbol in symbols]+['message.o','message-enchant-base.o'],check=True)
PY
g++ $flags -g -finput-charset=CP949 -fexec-charset=CP949 -c enchant_handler.cpp -o enchant_handler.o > enchant-handler-development.log 2>&1 || { cat enchant-handler-development.log; exit 1; }
# Only the name parser changes; preserve all native spawn-loading behavior.
python3 - <<'PY'
from pathlib import Path
import re,subprocess
for unit,names in [('world',['LoadMapName'])]:
    source=Path(unit+'.cpp').read_text(encoding='cp949')
    includes='\n'.join(line for line in source.splitlines() if line.startswith('#include '))
    bodies=[]
    for name in names:
        match=re.search(r'^(?:BOOL|void) '+name+r'\(',source,re.M)
        assert match,name
        start=source.index('{',match.end())
        # Count braces after masking comments and string/character literals.
        masked=re.sub(r'/\*.*?\*/|//[^\n]*|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'',lambda m:' '*len(m[0]),source[start:],flags=re.S)
        depth=0
        for offset,ch in enumerate(masked):
            depth+=(ch=='{')-(ch=='}')
            if depth==0:break
        assert offset>0 and depth==0,name
        bodies.append(source[match.start():start+offset+1])
    extra='\n'
    Path(unit+'_overlay.cpp').write_bytes((includes+extra+'\n'.join(bodies)+'\n').encode('cp949'))
    symbols=[]
    for line in subprocess.check_output(['nm','--defined-only',unit+'.o'],text=True).splitlines():
        symbol=line.split()[-1]
        if any(re.match(r'_Z\d+'+name+r'(?:P|R|i|v)',symbol) for name in names):symbols.append(symbol)
    assert len(symbols)==len(names),(unit,symbols)
    subprocess.run(['objcopy']+['--weaken-symbol='+s for s in symbols]+[unit+'.o',unit+'-overlay-base.o'],check=True)
PY
for unit in world; do
  g++ $flags -g -finput-charset=CP949 -fexec-charset=CP949 -c "${unit}_overlay.cpp" -o "${unit}_overlay.o" > "${unit}-overlay-development.log" 2>&1 || { cat "${unit}-overlay-development.log"; exit 1; }
done
python3 - <<'PY'
import re,pathlib,subprocess
s=pathlib.Path('makefile').read_text(errors='replace')
objs=re.findall(r'\w+\.o',re.search(r'OBJS\s*=\s*(.*?)(?:\n\s*\n)',s,re.S)[1])
changed=['main','player','item','skill','fight','dataserver','shop']
objs=[o[:-2]+'-gm.o' if o[:-2] in changed else o for o in objs]
objs=['message-enchant-base.o' if o=='message.o' else o for o in objs]
objs=[o[:-2]+'-overlay-base.o' if o=='world.o' else o for o in objs]
subprocess.run(['g++','-m32','-no-pie','-Wl,--wrap=fopen','-o','LAQIA_GameServer.gm']+objs+['world_overlay.o','enchant_handler.o','gm_commands.o','local_rates-gm.o','local_world-gm.o','/opt/laqia/legacy/usr/lib/libmysqlclient.so.15','-lz','-lm'],check=True)
PY
sha256sum LAQIA_GameServer.gm
