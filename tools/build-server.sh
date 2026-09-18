#!/bin/bash
set -eu
cd /opt/laqia/gm-build
flags="-m32 -std=gnu++98 -fpermissive -fno-pie -D_GLIBCXX_USE_CXX11_ABI=0 -I/usr/include/mariadb $(cat flags.txt)"
for unit in gm_commands main player item skill fight dataserver local_rates; do
  output="$unit-gm.o"
  test "$unit" != gm_commands || output=gm_commands.o
  g++ $flags -g -finput-charset=CP949 -fexec-charset=CP949 -c "$unit.cpp" -o "$output" > "$unit-development.log" 2>&1 || { tail -70 "$unit-development.log"; exit 1; }
done
# The archived message.cpp contains unrelated unfinished billing code.
# Compile only the edited enchant packet handler and override its legacy symbol.
python3 - <<'PY'
from pathlib import Path
s=Path('message.cpp').read_text(encoding='cp949')
a=s.index('BOOL PACKET_EnchantItem(')
b=s.index('\nBOOL ',a+1)
includes='\n'.join(line for line in s[:a].splitlines() if line.startswith('#include '))
Path('enchant_handler.cpp').write_bytes((includes+'\n'+s[a:b]+'\n').encode('cp949'))
PY
g++ $flags -g -finput-charset=CP949 -fexec-charset=CP949 -c enchant_handler.cpp -o enchant_handler.o > enchant-handler-development.log 2>&1 || { cat enchant-handler-development.log; exit 1; }
objcopy --weaken-symbol=_Z18PACKET_EnchantItemP15descriptor_data message.o message-enchant-base.o
python3 - <<'PY'
import re,pathlib,subprocess
s=pathlib.Path('makefile').read_text(errors='replace')
objs=re.findall(r'\w+\.o',re.search(r'OBJS\s*=\s*(.*?)(?:\n\s*\n)',s,re.S)[1])
changed=['main','player','item','skill','fight','dataserver']
objs=[o[:-2]+'-gm.o' if o[:-2] in changed else o for o in objs]
objs=['message-enchant-base.o' if o=='message.o' else o for o in objs]
subprocess.run(['g++','-m32','-no-pie','-o','LAQIA_GameServer.gm']+objs+['enchant_handler.o','gm_commands.o','local_rates-gm.o','/opt/laqia/legacy/usr/lib/libmysqlclient.so.15','-lz','-lm'],check=True)
PY
sha256sum LAQIA_GameServer.gm
