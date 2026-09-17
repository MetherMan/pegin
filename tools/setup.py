"""Initialize a new, private development VM. Existing worlds are never reset."""
from pathlib import Path
from io import BytesIO
import getpass,hashlib,json,logging,os,re,secrets,shutil,socket,subprocess,sys,tarfile,time,uuid
ROOT=Path(__file__).resolve().parents[1];R=ROOT/'runtime'
sys.path.insert(0,str(R/'pylibs'));sys.path.insert(0,str(R))
import paramiko,pycdlib
logging.getLogger('paramiko.transport').setLevel(logging.CRITICAL)
FLAGS=getattr(subprocess,'CREATE_NO_WINDOW',0)
def copy_tools():
 for p in (ROOT/'tools/runtime').glob('*.py'):shutil.copy2(p,R/p.name)
def connect():
 c=paramiko.SSHClient();known=R/'vm-known-hosts'
 if known.exists():c.load_host_keys(str(known))
 c.set_missing_host_key_policy(paramiko.AutoAddPolicy())
 c.connect('127.0.0.1',port=22222,username='root',key_filename=str(R/'vm-ssh-key'),timeout=8,allow_agent=False,look_for_keys=False)
 c.save_host_keys(str(known));return c
def run(c,command,timeout=900):
 _,o,e=c.exec_command(command,timeout=timeout);text=o.read().decode(errors='replace');err=e.read().decode(errors='replace')
 if o.channel.recv_exit_status():raise RuntimeError((err or text)[-6000:])
 return text
def put_bytes(s,path,data):
 with s.open(path,'wb') as f:f.write(data)
def add(t,p,name):t.add(p,arcname=name,recursive=False)
def main():
 copy_tools()
 from manage import apply_client
 apply_client()
 if (R/'.setup-complete').exists():
  print('Existing VM and characters preserved. Run Update-Workspace.cmd to apply source updates.');return
 # A local state file is private and ignored by Git. Passwords never enter the repo.
 state=R/'setup-private.json'
 if state.exists():settings=json.loads(state.read_text())
 else:
  pwd=os.environ.get('LAQIA_DEV_PASSWORD') or getpass.getpass('Choose a NEW local game password for developer (4-24 ASCII letters/digits): ')
  if not re.fullmatch(r'[A-Za-z0-9]{4,24}',pwd):raise ValueError('Use 4-24 ASCII letters/digits for the legacy login client.')
  settings={'db_password':secrets.token_hex(16),'game_password':pwd,'instance':str(uuid.uuid4())};state.write_text(json.dumps(settings))
 if not (R/'vm-ssh-key').exists():
  key=paramiko.RSAKey.generate(3072);key.write_private_key_file(str(R/'vm-ssh-key'));(R/'vm-ssh-key.pub').write_text(key.get_name()+' '+key.get_base64())
 if not (R/'seed.iso').exists():
  user='#cloud-config\ndisable_root: false\nssh_pwauth: false\nssh_authorized_keys:\n  - '+(R/'vm-ssh-key.pub').read_text()+'\n'
  meta='instance-id: laqia-dev-'+settings['instance']+'\nlocal-hostname: laqia-dev\n'
  iso=pycdlib.PyCdlib();iso.new(interchange_level=3,joliet=3,vol_ident='cidata',rock_ridge='1.09')
  for name,content in [('user-data',user),('meta-data',meta)]:
   b=content.encode();iso.add_fp(BytesIO(b),len(b),iso_path='/'+name.upper().replace('-','_')+';1',rr_name=name,joliet_path='/'+name)
  iso.write(str(R/'seed.iso'));iso.close()
 if not (R/'laqia-vm.qcow2').exists():
  subprocess.run([str(R/'qemu/qemu-img.exe'),'create','-f','qcow2','-F','qcow2','-b','debian-base.qcow2','laqia-vm.qcow2','16G'],cwd=R,check=True,creationflags=FLAGS)
 # Never start a second VM while another game's ports are in use.
 c=None
 try:c=connect()
 except (OSError,paramiko.SSHException):pass
 if c is None:
  for port in [22222,44444]:
   with socket.socket() as s:
    if s.connect_ex(('127.0.0.1',port))==0:raise RuntimeError('Another VM is using port '+str(port)+'. Close that game first.')
  subprocess.run([sys.executable,str(R/'start_vm.py')],cwd=R,check=True,creationflags=FLAGS)
  print('Waiting for first boot...',flush=True)
  for _ in range(120):
   try:c=connect();break
   except (OSError,paramiko.SSHException):time.sleep(2)
  else:raise RuntimeError('Fresh VM SSH unavailable; see runtime/vm-console.log')
 try:
  run(c,'mkdir -p /root/laqia-setup')
  # Bundles contain only static maps, program binaries and newly generated local settings.
  server_tar=ROOT/'.cache/server-setup.tar.gz';server_tar.parent.mkdir(exist_ok=True)
  with tarfile.open(server_tar,'w:gz') as tar:
   for p in (R/'server').rglob('*'):
    if p.is_file():add(tar,p,p.relative_to(R/'server').as_posix())
   for p in (ROOT/'server-bin').iterdir():add(tar,p,p.name+'/'+p.name)
   for p in (ROOT/'game-data/DATA').rglob('*'):
    if p.is_file():add(tar,p,'LAQIA_GameServer/DATA/'+p.relative_to(ROOT/'game-data/DATA').as_posix())
   for p in (ROOT/'server-config').glob('*/*.INI'):
    data=p.read_bytes().replace(b'@LOCAL_DB_PASSWORD@',settings['db_password'].encode());info=tarfile.TarInfo(p.parent.name+'/'+p.name);info.size=len(data);tar.addfile(info,BytesIO(data))
  build_tar=ROOT/'.cache/build-setup.tar.gz'
  with tarfile.open(build_tar,'w:gz') as tar:
   for p in (R/'legacy-objects').glob('*.o'):add(tar,p,p.name)
   for p in (ROOT/'src/server/LAQIA_GameServer').glob('*'):
    if p.is_file():add(tar,p,p.name)
  dbs=['LoginAccount','LAQIA_GAMEDB','LAQIA_ITEMLOG','LAQIA_CASH','DAUM_BILLING'];sql="SET sql_mode='';\n"
  for host in ['localhost','127.0.0.1']:
   sql+="CREATE USER IF NOT EXISTS 'laqia'@'"+host+"' IDENTIFIED BY '"+settings['db_password']+"';\n"
   for db in dbs:sql+="GRANT ALL ON `"+db+"`.* TO 'laqia'@'"+host+"';\n"
  sql+="INSERT IGNORE INTO LoginAccount.g4_member(mb_id,mb_password,mb_name,mb_nick) VALUES('developer',PASSWORD('"+settings['game_password']+"'),'Developer','Developer');\n"
  sql+="INSERT INTO LAQIA_GAMEDB.UserTable(id,name,mapNum,posX,posY,hp,mp,max_hp,max_mp,str_point,int_point,dex_point,charPos,lastSkill) SELECT 'developer','Developer',3,248,264,500,100,500,100,20,20,20,0,201 WHERE NOT EXISTS (SELECT 1 FROM LAQIA_GAMEDB.UserTable WHERE id='developer');\n"
  with c.open_sftp() as s:
   for p,name in [(server_tar,'server.tar.gz'),(build_tar,'build.tar.gz'),(R/'libmysqlclient15.deb','libmysqlclient15.deb'),(ROOT/'server-config/schema.sql','schema.sql'),(ROOT/'tools/provision.sh','provision.sh')]:s.put(str(p),'/root/laqia-setup/'+name)
   for p in (ROOT/'server-config/systemd').glob('*.service'):s.put(str(p),'/root/laqia-setup/'+p.name)
   put_bytes(s,'/root/laqia-setup/local-account.sql',sql.encode())
  print('Installing Linux dependencies and creating an empty development database. This may take several minutes...',flush=True)
  # Keep terminal progress available without leaking configuration/password content.
  i,o,e=c.exec_command('bash /root/laqia-setup/provision.sh > /root/laqia-setup/provision.log 2>&1',timeout=1800)
  while not o.channel.exit_status_ready():
   time.sleep(15);print('Provisioning development VM...',flush=True)
  if o.channel.recv_exit_status():raise RuntimeError(run(c,'tail -70 /root/laqia-setup/provision.log'))
  assert run(c,'systemctl is-active LAQIA_DataServer LAQIA_GameServer LAQIA_LoginServer').split()==['active']*3
  (R/'.setup-complete').write_text('developer\n');print('Ready. Local account: developer; use the password you chose.',flush=True)
 finally:c.close()
 import local_control
 local_control.stop()
if __name__=='__main__':main()
