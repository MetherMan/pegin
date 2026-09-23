"""Apply only primordial EXP to the existing family host and running VM table.

This never boots a VM, installs a server binary, changes accounts, or writes a
client table. --apply-vm requires a separately authorized existing-VM boot.
"""
from pathlib import Path
from datetime import datetime
import argparse
import json
import os
import shutil
import sys
import time

from primordial_experience import R, SERVER, desired_exp, patch_text, verify_delta, sha

FAMILY = R.parent / 'https-laqia-co-kr-https-laqia/work/laqia-runtime'
HOST = FAMILY / 'server-local/LAQIA_GameServer/DATA/MOB_DATA.txt'
REMOTE = '/opt/laqia/server/LAQIA_GameServer/DATA/MOB_DATA.txt'
BIN = '/opt/laqia/server/LAQIA_GameServer/LAQIA_GameServer'
SERVICES = ('LAQIA_LoginServer', 'LAQIA_GameServer', 'LAQIA_DataServer')
SAVE_DIGEST = ('set -o pipefail; mariadb-dump --no-create-info --skip-comments --compact '
               'LAQIA_GAMEDB UserTable InvenItems EquipItems UserSkills UserCashMoney | sha256sum')
OUT = R / 'assets/primordial-baphomet/experience-installation.json'


def host_apply(apply=False):
    before = HOST.read_bytes()
    exp = desired_exp(SERVER.read_bytes())
    assert desired_exp(before) == exp, 'Installed basic Baphomet differs from the approved base'
    after = patch_text(before, exp)
    changes = verify_delta(before, after, exp)
    result = dict(path=str(HOST), before_sha256=sha(before), after_sha256=sha(after),
                  rows=changes, changed=before != after, applied=False)
    if not apply or before == after:
        result['applied'] = bool(apply)
        return result
    stamp = datetime.now().strftime('%Y%m%d-%H%M%S-%f')
    backup = FAMILY / 'backups' / ('primordial-exp-' + stamp) / 'MOB_DATA.txt'
    assert HOST.resolve().is_relative_to(FAMILY.resolve())
    assert not HOST.is_symlink()
    backup.parent.mkdir(parents=True, exist_ok=False)
    backup.write_bytes(before)
    assert backup.read_bytes() == before
    staged = HOST.with_name('MOB_DATA.txt.primordial-exp.tmp')
    assert not staged.exists()
    try:
        staged.write_bytes(after)
        assert staged.read_bytes() == after and HOST.read_bytes() == before
        os.replace(staged, HOST)
        assert HOST.read_bytes() == after
    except BaseException:
        shutil.copy2(backup, HOST)
        assert HOST.read_bytes() == before
        raise
    finally:
        if staged.exists():
            staged.unlink()
    result.update(applied=True, backup=str(backup))
    return result


def vm_apply(result):
    sys.path.insert(0, str(FAMILY / 'pylibs'))
    import paramiko
    c = paramiko.SSHClient()
    c.load_host_keys(str(FAMILY / 'vm-known-hosts'))
    c.set_missing_host_key_policy(paramiko.RejectPolicy())
    c.connect('127.0.0.1', port=22222, username='root', key_filename=str(FAMILY / 'vm-ssh-key'),
              timeout=8, banner_timeout=8, auth_timeout=8, allow_agent=False, look_for_keys=False)
    def cmd(value):
        _, stdout, stderr = c.exec_command(value, timeout=60)
        output, error = stdout.read().decode(), stderr.read().decode()
        if stdout.channel.recv_exit_status():
            raise RuntimeError(error or output)
        return output.strip()
    result.update(remote=REMOTE, accounts_or_database_modified=False)
    before = None
    backup = None
    replaced = False
    stage_created = False
    stage = REMOTE + '.primordial-exp.tmp'
    active = []
    try:
        assert not cmd("ss -Htn state established 'sport = :2560'"), 'A game session is connected'
        states = {s: cmd('systemctl is-active ' + s + ' || true') for s in SERVICES}
        active = [s for s, state in states.items() if state == 'active']
        result['services_before'] = states
        result['server_sha256'] = cmd('sha256sum ' + BIN).split()[0]
        assert result['server_sha256'] == sha((R / 'server-bin/LAQIA_GameServer').read_bytes()), 'Unexpected installed server binary'
        result['save_tables_before'] = cmd(SAVE_DIGEST).split()[0]
        # Stop front-door and game first; keep DataServer available for flush.
        for service in SERVICES:
            if service in active:
                cmd('systemctl stop ' + service)
                if service == 'LAQIA_GameServer':
                    time.sleep(3)
        with c.open_sftp() as s:
            before = s.open(REMOTE, 'rb').read()
            exp = desired_exp(SERVER.read_bytes())
            assert desired_exp(before) == exp, 'VM basic Baphomet differs from the approved base'
            after = patch_text(before, exp)
            result['rows'] = verify_delta(before, after, exp)
            result.update(before_sha256=sha(before), after_sha256=sha(after))
            stamp = datetime.now().strftime('%Y%m%d-%H%M%S-%f')
            backup = '/opt/laqia/update-backups/primordial-exp-' + stamp
            cmd('mkdir -p ' + backup + ' && cp -p ' + REMOTE + ' ' + backup + '/MOB_DATA.txt')
            assert sha(s.open(backup + '/MOB_DATA.txt', 'rb').read()) == sha(before)
            attrs = s.stat(REMOTE)
            assert not cmd('test -e ' + stage + ' && echo exists || true')
            with s.open(stage, 'wb') as target:
                stage_created = True
                target.write(after)
            s.chmod(stage, attrs.st_mode & 0o7777)
            s.chown(stage, attrs.st_uid, attrs.st_gid)
            assert sha(s.open(stage, 'rb').read()) == sha(after)
            assert sha(s.open(REMOTE, 'rb').read()) == sha(before)
            s.posix_rename(stage, REMOTE)
            replaced = True
            assert sha(s.open(REMOTE, 'rb').read()) == sha(after)
        for service in reversed(SERVICES):
            if service in active:
                cmd('systemctl start ' + service)
        time.sleep(2)
        result['services_after'] = {s: cmd('systemctl is-active ' + s + ' || true') for s in SERVICES}
        assert result['services_after'] == states, 'Service state changed'
        assert cmd('sha256sum ' + BIN).split()[0] == result['server_sha256']
        result['save_tables_after'] = cmd(SAVE_DIGEST).split()[0]
        assert result['save_tables_before'] == result['save_tables_after'], 'Saved character/inventory tables changed'
        result.update(passed=True, backup=backup + '/MOB_DATA.txt', only_remote_payload_written=REMOTE,
                      five_save_tables_unchanged=True, binary_card_growth_fixes_preserved=True)
        return result
    except BaseException as error:
        result.update(passed=False, error=str(error), backup=backup)
        try:
            if replaced and backup:
                cmd('systemctl stop LAQIA_GameServer')
                cmd('cp -p ' + backup + '/MOB_DATA.txt ' + REMOTE)
                assert cmd('sha256sum ' + REMOTE).split()[0] == sha(before)
                result['rolled_back'] = True
            for service in reversed(SERVICES):
                if service in active:
                    cmd('systemctl start ' + service)
        except BaseException as recovery_error:
            result['recovery_error'] = str(recovery_error)
        raise
    finally:
        if stage_created:
            try:
                # Only the exact staging file created by this invocation.
                cmd('if test -f ' + stage + '; then rm -- ' + stage + '; fi')
            except BaseException as cleanup_error:
                result['stage_cleanup_error'] = str(cleanup_error)
        c.close()


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--apply-host', action='store_true')
    parser.add_argument('--apply-vm', action='store_true')
    args = parser.parse_args()
    result = dict(client_installation_delegated_to_final_visual_installer=True)
    try:
        result['host'] = host_apply(args.apply_host)
        if args.apply_host:
            OUT.write_text(json.dumps(result, ensure_ascii=False, indent=2) + '\n', encoding='utf-8')
        if args.apply_vm:
            result['vm'] = {}
            vm_apply(result['vm'])
    except BaseException as error:
        result.update(passed=False, error=str(error))
        raise
    finally:
        if args.apply_host or args.apply_vm:
            OUT.write_text(json.dumps(result, ensure_ascii=False, indent=2) + '\n', encoding='utf-8')
    print(json.dumps(result, ensure_ascii=False))
