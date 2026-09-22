"""Apply the release to an existing outputs installation and verify its saves."""
from pathlib import Path
import argparse,sys,socket,json,hashlib,shutil
R=Path(__file__).resolve().parents[1];O=R/'assets/skills140'
def main():
 parser=argparse.ArgumentParser();parser.add_argument('--root',type=Path,required=True);args=parser.parse_args()
 root=args.root.resolve();runtime=root/'work/laqia-runtime'
 assert (root/'outputs/Start-Local-Game.cmd').is_file() and (runtime/'local_control.py').is_file()
 for port in [22222,2560,44444]:
  with socket.socket() as s:
   s.settimeout(.2)
   if s.connect_ex(('127.0.0.1',port))==0:raise RuntimeError('A local server is running; close the game first')
 sys.path[:0]=[str(runtime),str(runtime/'pylibs'),str(R/'distribution'),str(R/'tools')]
 import local_control as ctl
 from ssh_vm import connect
 import father_update
 from verify_skill140_live import main as live
 assert Path(ctl.__file__).resolve().parent==runtime
 digest='mariadb-dump --no-create-info --skip-comments --compact LAQIA_GAMEDB UserTable InvenItems EquipItems UserSkills UserCashMoney | sha256sum'
 ctl.start()
 try:
  with connect() as c:before=ctl.command(c,digest).split()[0]
  father_update.apply(root,runtime,R)
  live()
  with connect() as c:
   after=ctl.command(c,digest).split()[0];assert before==after
   server=ctl.command(c,'sha256sum /opt/laqia/server/LAQIA_GameServer/LAQIA_GameServer').split()[0]
  assert server==hashlib.sha256((R/'server-bin/LAQIA_GameServer').read_bytes()).hexdigest()
  manifest=json.loads((R/'distribution/update-manifest.json').read_text(encoding='utf-8'));count=0
  for e in manifest['files']:
   if e['kind']=='client':
    target=runtime/'client/GameClient'/e['path'];assert hashlib.sha256(target.read_bytes()).hexdigest()==e['sha256'],e['path'];count+=1
  result=dict(passed=True,installation='existing outputs/work installation',client_files=count,server_sha256=server,five_save_tables_unchanged=True,live=json.loads((O/'live-validation.json').read_text(encoding='utf-8')))
  (O/'family-installation-validation.json').write_text(json.dumps(result,ensure_ascii=False,indent=2),encoding='utf-8')
  print('PASS actual outputs installation: all payload hashes match; five save tables unchanged',flush=True)
 finally:
  if ctl.online() and not ctl.client_running():ctl.stop()
if __name__=='__main__':main()
