"""Build/apply the verified local set, run integration, then stop our server."""
import shutil,socket
import manage
from verify_twilight_set_live import main as verify
def main():
    # This is the user's repository-local QEMU, never a remote server. Refuse
    # any already running instance so this workflow cannot interrupt a game.
    disk=(manage.R/'laqia-vm.qcow2').resolve();assert disk.is_relative_to(manage.R.resolve())
    for port in [22222,2560,44444]:
        with socket.socket() as s:
            s.settimeout(.2)
            if s.connect_ex(('127.0.0.1',port))==0:raise RuntimeError('A local game/server is already running; close it before integration.')
    ctl=manage.control();manage.apply_client();ctl.start()
    try:
        binary=manage.build_server(ctl);manage.update_server(ctl,binary)
        shutil.copy2(binary,manage.ROOT/'server-bin/LAQIA_GameServer')
        verify()
    finally:
        if ctl.online() and not ctl.client_running():ctl.stop()
if __name__=='__main__':main()
