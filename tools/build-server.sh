#!/bin/bash
set -eu
cd /opt/laqia/gm-build
flags="-m32 -std=gnu++98 -fpermissive -fno-pie -D_GLIBCXX_USE_CXX11_ABI=0 -I/usr/include/mariadb $(cat flags.txt)"
for unit in gm_commands main player item skill fight dataserver local_rates; do
  output="$unit-gm.o"
  test "$unit" != gm_commands || output=gm_commands.o
  g++ $flags -g -finput-charset=CP949 -fexec-charset=CP949 -c "$unit.cpp" -o "$output" > "$unit-development.log" 2>&1 || { tail -70 "$unit-development.log"; exit 1; }
done
python3 - <<'PY'
import re,pathlib,subprocess
s=pathlib.Path('makefile').read_text(errors='replace')
objs=re.findall(r'\w+\.o',re.search(r'OBJS\s*=\s*(.*?)(?:\n\s*\n)',s,re.S)[1])
changed=['main','player','item','skill','fight','dataserver']
objs=[o[:-2]+'-gm.o' if o[:-2] in changed else o for o in objs]
subprocess.run(['g++','-m32','-no-pie','-o','LAQIA_GameServer.gm']+objs+['gm_commands.o','local_rates-gm.o','/opt/laqia/legacy/usr/lib/libmysqlclient.so.15','-lz','-lm'],check=True)
PY
sha256sum LAQIA_GameServer.gm
