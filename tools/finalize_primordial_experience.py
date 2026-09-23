"""Read loaded Baphomet EXP, then restore the pre-task family VM's off state."""
from pathlib import Path
import json
import shlex
import socket
import sys
import time

R = Path(__file__).resolve().parents[1]
F = R.parent / 'https-laqia-co-kr-https-laqia/work/laqia-runtime'
sys.path.insert(0, str(F / 'pylibs'))
import paramiko

report_path = R / 'assets/primordial-baphomet/experience-installation.json'
report = json.loads(report_path.read_text(encoding='utf-8'))
before = json.loads((R / '.cache/primordial-experience-v14/vm-before.json').read_text(encoding='utf-8'))
assert before['vm_before'] == 'off' and report['vm']['passed']
c = paramiko.SSHClient()
c.load_host_keys(str(F / 'vm-known-hosts'))
c.set_missing_host_key_policy(paramiko.RejectPolicy())
c.connect('127.0.0.1', port=22222, username='root', key_filename=str(F / 'vm-ssh-key'),
          timeout=8, banner_timeout=8, auth_timeout=8, allow_agent=False, look_for_keys=False)

def cmd(command):
    _, out, err = c.exec_command(command, timeout=40)
    value, error = out.read().decode(), err.read().decode()
    if out.channel.recv_exit_status():
        raise RuntimeError(error or value)
    return value.strip()

try:
    assert not cmd("ss -Htn state established 'sport = :2560'"), 'A game session is connected'
    pid = cmd('systemctl show -p MainPID --value LAQIA_GameServer')
    assert pid.isdigit() and int(pid) > 1
    args = ['timeout', '20', 'gdb', '-q', '-nx', '-batch', '-p', pid, '-ex', 'set pagination off']
    for ident in (44, 80, 280, 380, 480, 580, 680):
        args += ['-ex', f'printf "EXP {ident} %d\\n", g_MOBINFO[{ident}]->exp']
    args += ['-ex', 'detach']
    raw = cmd(' '.join(shlex.quote(a) for a in args))
    loaded = {int(cells[1]): int(cells[2]) for line in raw.splitlines()
              if len(cells := line.split()) == 3 and cells[0] == 'EXP'}
    assert loaded == {44: 14328, **{i: 28656 for i in (80, 280, 380, 480, 580, 680)}}, loaded
    report['loaded_server_experience'] = loaded
    # Return only the existing, previously-off VM to its original lifecycle state.
    cmd('systemctl stop LAQIA_LoginServer && systemctl stop LAQIA_GameServer && sleep 3 && systemctl stop LAQIA_DataServer')
    digest = cmd('set -o pipefail; mariadb-dump --no-create-info --skip-comments --compact LAQIA_GAMEDB UserTable InvenItems EquipItems UserSkills UserCashMoney | sha256sum').split()[0]
    assert digest == report['vm']['save_tables_before']
    report['save_tables_after_clean_stop'] = digest
    cmd('sync && systemd-run --on-active=2 /sbin/poweroff')
    report['clean_shutdown_requested'] = True
finally:
    c.close()
    report_path.write_text(json.dumps(report, ensure_ascii=False, indent=2) + '\n', encoding='utf-8')

def is_open(port):
    try:
        with socket.create_connection(('127.0.0.1', port), timeout=.3):
            return True
    except OSError:
        return False

for _ in range(50):
    ports = {p: is_open(p) for p in (22222, 44444, 8030, 2560)}
    if not any(ports.values()):
        report.update(vm_restored_to_off=True, ports_after=ports, passed=True)
        report_path.write_text(json.dumps(report, ensure_ascii=False, indent=2) + '\n', encoding='utf-8')
        print(json.dumps(dict(passed=True, loaded_experience=loaded, five_save_tables_unchanged=True,
                             vm_restored_to_off=True), ensure_ascii=False))
        break
    time.sleep(1)
else:
    raise RuntimeError('Clean poweroff requested but the existing VM remains reachable; no forced termination was attempted.')
