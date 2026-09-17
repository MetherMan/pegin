from pathlib import Path
import hashlib,json,shutil,subprocess,sys,time
ROOT=Path(__file__).resolve().parents[1];R=ROOT/'runtime'
sys.path.insert(0,str(R));sys.path.insert(0,str(R/'pylibs'))
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def apply_client():
 from portable_support import client_running
 if client_running():raise RuntimeError('Close the game before applying files.')
 state=R/'applied-client.json';old=json.loads(state.read_text()) if state.exists() else {};new={}
 source=ROOT/'client-overlay';dest=R/'client/GameClient'
 files=[p for p in source.rglob('*') if p.is_file()]
 for p in files:
  rel=p.relative_to(source).as_posix();target=dest/rel;h=sha(p)
  if rel in ('config.ini','server.ini'):continue
  if rel in old and target.exists() and sha(target) not in [old[rel],h]:raise RuntimeError('Local runtime edit detected; copy it into client-overlay before syncing: '+rel)
 for p in files:
  rel=p.relative_to(source).as_posix();target=dest/rel
  if rel in ('config.ini','server.ini') and target.exists():continue
  target.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(p,target);new[rel]=sha(p)
 state.write_text(json.dumps(new,indent=2))
def control():
 for p in (ROOT/'tools/runtime').glob('*.py'):shutil.copy2(p,R/p.name)
 import local_control
 return local_control
def update_server(ctl,built=None):
 from ssh_vm import connect
 with connect() as c:
  cmd=lambda s:ctl.command(c,s)
  if cmd("ss -Htn state established 'sport = :2560'").strip():raise RuntimeError('Players are connected; close the game before updating the server.')
  files={p.relative_to(ROOT/'game-data').as_posix() : p for p in (ROOT/'game-data').rglob('*') if p.is_file()}
  binary=built or ROOT/'server-bin/LAQIA_GameServer';files['LAQIA_GameServer']=binary
  desired={n:sha(p) for n,p in files.items()};state=R/'applied-server.json'
  if state.exists() and json.loads(state.read_text())==desired:return
  remote='/opt/laqia/server/LAQIA_GameServer';backup='/opt/laqia/update-backups/'+time.strftime('%Y%m%d-%H%M%S')
  cmd('mkdir -p '+backup+' && cp -a '+remote+'/DATA '+backup+'/ && cp '+remote+'/LAQIA_GameServer '+backup+'/')
  cmd('systemctl stop LAQIA_GameServer')
  try:
   with c.open_sftp() as s:
    for rel,p in files.items():
     # Production DATA subdirectories already originate from the static map bundle.
     parent=remote+'/'+str(Path(rel).parent).replace('\\','/')
     import shlex
     cmd('mkdir -p '+shlex.quote(parent));s.put(str(p),remote+'/'+rel)
    s.chmod(remote+'/LAQIA_GameServer',0o755)
   cmd('chown -R laqia:laqia '+remote+' && systemctl start LAQIA_GameServer');time.sleep(2)
   if cmd('systemctl is-active LAQIA_GameServer').strip()!='active':raise RuntimeError('Updated server failed to start')
  except BaseException:
   cmd('cp -a '+backup+'/DATA/. '+remote+'/DATA/; cp '+backup+'/LAQIA_GameServer '+remote+'/LAQIA_GameServer; systemctl start LAQIA_GameServer');raise
  state.write_text(json.dumps(desired,indent=2))
def build_server(ctl):
 from ssh_vm import connect
 with connect() as c:
  with c.open_sftp() as s:
   for p in (ROOT/'src/server/LAQIA_GameServer').iterdir():
    if p.is_file():s.put(str(p),'/opt/laqia/gm-build/'+p.name)
   for p in (ROOT/'src/server/utf8').iterdir():
    if p.is_file():
     name=p.name.replace('.utf8.','.');data=p.read_text(encoding='utf-8-sig').encode('cp949')
     with s.open('/opt/laqia/gm-build/'+name,'wb') as f:f.write(data)
   s.put(str(ROOT/'tools/build-server.sh'),'/opt/laqia/gm-build/build-development.sh')
  _,out,err=c.exec_command('bash /opt/laqia/gm-build/build-development.sh',timeout=900)
  result=out.read().decode(errors='replace');error=err.read().decode(errors='replace')
  if out.channel.recv_exit_status():raise RuntimeError(error or result)
  print(result)
  out=R/'build/LAQIA_GameServer';out.parent.mkdir(exist_ok=True)
  with c.open_sftp() as s:s.get('/opt/laqia/gm-build/LAQIA_GameServer.gm',str(out))
 return out
def main(action):
 if not (R/'.setup-complete').exists():raise RuntimeError('Run Initialize.cmd first.')
 ctl=control();apply_client()
 if action=='client':print('Client resources applied.');return
 ctl.start()
 try:
  binary=build_server(ctl) if action=='build' else None
  update_server(ctl,binary)
  if action=='play':
   import family_launcher
   family_launcher.main('local')
  else:print('Workspace synchronized; character database preserved.')
 finally:
  if ctl.online() and not ctl.client_running():ctl.stop()
if __name__=='__main__':main(sys.argv[1] if len(sys.argv)>1 else 'sync')
