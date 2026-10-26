"""Build the server with the 천상의 날개 weapons in the development VM and test it live.

Same steps as Build-Server.cmd (apply client-overlay to runtime/client, build src/server in the VM,
install into the development server keeping its database, copy the binary to server-bin), then
tools/verify_celestial_weapons_live.py and the earlier live checks as regression tests. The stack
test's first pickup check depends on inventory order (which of two stacks a card joins), so it gets
one retry. The family (outputs) installation and its accounts are not touched. Close the game first.
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
        import verify_celestial_weapons_live, verify_bluemoon_armor_potion_live, verify_blue_moon_horse_live
        import verify_stacks_live, verify_twilight_set_live
        verify_celestial_weapons_live.main(ROOT/'runtime')
        verify_bluemoon_armor_potion_live.main(ROOT/'runtime')
        verify_blue_moon_horse_live.main(ROOT/'runtime')
        for attempt in range(2):
            try:
                verify_stacks_live.main()
                break
            except AssertionError:
                if attempt:
                    raise
                traceback.print_exc()
                print('RETRY stacks', flush=True)
        verify_twilight_set_live.main()
        ok = True
    except Exception:
        traceback.print_exc()
    finally:
        if ctl.online() and not ctl.client_running():
            ctl.stop()
    print('RESULT', 'PASS' if ok else 'FAIL', flush=True)
    return 0 if ok else 1


if __name__ == '__main__':
    sys.exit(main())
