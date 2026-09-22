"""Build the level-140 server and check its actual GM warehouse inventory."""
import manage,shutil,sys,struct,time,uuid,socket
def main():
 for port in [22222,2560,44444]:
  with socket.socket() as s:
   s.settimeout(.2)
   if s.connect_ex(('127.0.0.1',port))==0:raise RuntimeError('Close the local game/server first')
 ctl=manage.control();manage.apply_client();ctl.start()
 try:
  binary=manage.build_server(ctl);manage.update_server(ctl,binary);shutil.copy2(binary,manage.ROOT/'server-bin/LAQIA_GameServer')
  from verify_skill140_live import main as verify
  verify()
 finally:
  if ctl.online() and not ctl.client_running():ctl.stop()
if __name__=='__main__':main()
