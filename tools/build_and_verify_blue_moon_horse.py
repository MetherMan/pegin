"""Build the Blue Moon horse server in the development VM, install it there, and run the live check.

Same steps as Build-Server.cmd (apply client-overlay to runtime/client, build src/server in the VM,
install into the development server keeping its database, copy the binary to server-bin), followed by
tools/verify_blue_moon_horse_live.py while the VM is still running. The family (outputs) installation
and its accounts are not touched. Close the game before running.
"""
from pathlib import Path
import hashlib, shutil, sys, traceback
ROOT = Path(__file__).resolve().parents[1]
sys.path[:0] = [str(ROOT/'tools')]
import manage


def main():
    ctl = manage.control()
    manage.apply_client()
    ctl.start()
    ok = False
    try:
        binary = manage.build_server(ctl)
        manage.update_server(ctl, binary)
        shutil.copy2(binary, ROOT/'server-bin/LAQIA_GameServer')
        print('SERVER', hashlib.sha256(binary.read_bytes()).hexdigest(), flush=True)
        import verify_blue_moon_horse_live
        verify_blue_moon_horse_live.main(ROOT/'runtime')
        ok = True
    except Exception:
        traceback.print_exc()
    finally:
        if ctl.online() and not ctl.client_running():
            ctl.stop()
    print('RESULT', 'PASS' if ok else 'FAIL')
    return 0 if ok else 1


if __name__ == '__main__':
    sys.exit(main())
