set -eu
cd /opt/laqia/gm-build
flags="-m32 -std=gnu++98 -fpermissive -fno-pie -D_GLIBCXX_USE_CXX11_ABI=0 -I/usr/include/mysql $(cat flags.txt)"
g++ $flags -no-pie abi_probe.cpp -o abi_probe
./abi_probe > abi-new.txt
g++ $flags -g -finput-charset=CP949 -fexec-charset=CP949 -c gm_commands.cpp -o gm_commands.o > gm-build.log 2>&1 || { grep 'error:' gm-build.log; exit 1; }
g++ $flags -g -finput-charset=CP949 -fexec-charset=CP949 -c main.cpp -o main-gm.o > main-build.log 2>&1 || { grep 'error:' main-build.log; exit 1; }
g++ $flags -g -finput-charset=CP949 -fexec-charset=CP949 -c player.cpp -o player-gm.o > player-build.log 2>&1 || { grep 'error:' player-build.log; exit 1; }
g++ $flags -g -finput-charset=CP949 -fexec-charset=CP949 -c item.cpp -o item-gm.o > item-build.log 2>&1 || { grep 'error:' item-build.log; exit 1; }
g++ $flags -g -finput-charset=CP949 -fexec-charset=CP949 -c skill.cpp -o skill-gm.o > skill-build.log 2>&1 || { cat skill-build.log; exit 1; }
g++ $flags -g -finput-charset=CP949 -fexec-charset=CP949 -c fight.cpp -o fight-gm.o > fight-build.log 2>&1 || { cat fight-build.log; exit 1; }
g++ $flags -g -finput-charset=CP949 -fexec-charset=CP949 -c dataserver.cpp -o dataserver-gm.o > dataserver-build.log 2>&1 || { cat dataserver-build.log; exit 1; }
g++ $flags -g -finput-charset=CP949 -fexec-charset=CP949 -c local_rates.cpp -o local_rates-gm.o > local_rates-build.log 2>&1 || { cat local_rates-build.log; exit 1; }
python3 - <<'PY'
import re,pathlib,subprocess
s=pathlib.Path('makefile').read_text(errors='replace');objs=re.findall(r'\w+\.o',re.search(r'OBJS\s*=\s*(.*?)(?:\n\s*\n)',s,re.S)[1])
objs=[{'main.o':'main-gm.o','player.o':'player-gm.o','item.o':'item-gm.o','skill.o':'skill-gm.o','fight.o':'fight-gm.o','dataserver.o':'dataserver-gm.o'}.get(o,o) for o in objs]
cmd=['g++','-m32','-no-pie','-o','LAQIA_GameServer.gm']+objs+['gm_commands.o','local_rates-gm.o','/opt/laqia/legacy/usr/lib/libmysqlclient.so.15','-lz','-lm']
p=subprocess.run(cmd,capture_output=True,text=True);pathlib.Path('link.log').write_text(p.stdout+p.stderr);print(p.stdout+p.stderr);raise SystemExit(p.returncode)
PY
file LAQIA_GameServer.gm
