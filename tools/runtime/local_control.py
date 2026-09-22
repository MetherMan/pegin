from pathlib import Path
import sys, subprocess, time, socket, json, logging, re
from ssh_vm import connect
from portable_support import client_running,client_drive,release_drive,preflight
ROOT=Path(__file__).resolve().parent
SERVICES='LAQIA_DataServer LAQIA_GameServer LAQIA_LoginServer'
logging.basicConfig(filename=str(ROOT/'local-control.log'), encoding='utf-8', level=logging.WARNING)
def command(c,cmd):
    _,out,err=c.exec_command(cmd,timeout=30)
    result=out.read().decode(errors='replace')
    error=err.read().decode(errors='replace')
    if out.channel.recv_exit_status(): raise RuntimeError(error or result)
    return result
def vm_status():
    try:
        with connect() as c:
            command(c,"test -f /root/laqia-server-prepared || { echo 'VM SSH connected, but the existing server preparation marker is missing.'; exit 1; }")
        return True, ''
    except Exception as error:
        return False, type(error).__name__ + ': ' + str(error)

def online():
    return vm_status()[0]

def boot_diagnostic(reason):
    parts=['VM 준비 확인 실패: '+reason]
    for name in ('server-start.log','vm-stderr.log','vm-console.log'):
        path=ROOT/name
        try:
            with path.open('rb') as stream:
                stream.seek(max(0,path.stat().st_size-6000))
                tail=stream.read().decode('utf-8',errors='replace')
            tail=re.sub(r'\x1b\[[0-?]*[ -/]*[@-~]','',tail).strip()
            if tail:parts.append('['+name+']\n'+tail)
        except OSError:pass
    report='\n\n'.join(parts)
    target=ROOT/'vm-start-diagnostic.txt'
    try:target.write_text(report+'\n',encoding='utf-8')
    except OSError:pass
    return ('게임 서버가 준비되지 않았습니다. 게임 파일과 DB는 아직 변경하지 않았습니다.\n'
            '마지막 확인: '+reason+'\n진단 파일: '+str(target)+'\n'+report[-2400:])

def wait_for_vm(timeout=360):
    start_time=time.monotonic();deadline=start_time+timeout;notice=start_time
    reason='VM boot is still in progress'
    while time.monotonic()<deadline:
        ready,reason=vm_status()
        if ready:return
        now=time.monotonic()
        if now>=notice:
            print('서버 부팅 확인 중... '+str(int(now-start_time))+'초 / '+str(timeout)+'초',flush=True)
            notice=now+15
        if now<deadline:time.sleep(min(2,deadline-now))
    raise RuntimeError(boot_diagnostic(reason))
def port_open(port):
    try:
        with socket.create_connection(('127.0.0.1',port),timeout=.5):return True
    except OSError:return False
def start():
    preflight(client=False)
    print('기존 게임 서버를 준비합니다. 느린 PC는 최대 6분 정도 걸릴 수 있습니다.', flush=True)
    if not online():
        if port_open(44444) or port_open(22222):
            # A previous timeout may leave the original VM still booting.
            print('이미 시작 중인 서버를 기다립니다. 두 번째 VM은 실행하지 않습니다.',flush=True)
        else:
            boot_log=ROOT/'server-start.log'
            with boot_log.open('wb') as output:
                launched=subprocess.run([str(ROOT/'python/python.exe'),str(ROOT/'start_vm.py')],
                                        stdout=output,stderr=subprocess.STDOUT,creationflags=subprocess.CREATE_NO_WINDOW)
            if launched.returncode:
                raise RuntimeError(boot_diagnostic('QEMU 시작 프로그램 종료 코드 '+str(launched.returncode)))
        wait_for_vm()
    with connect() as c:
        command(c,'systemctl start '+SERVICES)
        print(command(c,'systemctl is-active '+SERVICES))
    for port in (8030,2560):
        for attempt in range(30):
            try:
                with socket.create_connection(('127.0.0.1',port),timeout=1): break
            except OSError:
                if attempt==29: raise
                time.sleep(1)
    print('Local server ready. Owner account: developer (use your configured password).')
def launch_client(host='127.0.0.1'):
    preflight(server=False)
    if client_running(): raise RuntimeError('게임이 이미 실행 중입니다. 작업 표시줄의 게임 창을 열어 주세요.')
    # Only validated numeric IPv4 addresses reach the legacy INI parser.
    socket.inet_pton(socket.AF_INET,host)
    client=ROOT/'client'
    ini=client/'GameClient/server.ini'
    original=ini.read_bytes()
    updated,count=re.subn(rb'((?:login_server|game_server[1-6])\s*=\s*)"[^"]*"',
                         lambda m:m[1]+b'"'+host.encode('ascii')+b'"',original)
    if count!=7: raise RuntimeError('server.ini의 서버 주소 7개를 확인할 수 없습니다.')
    ini.write_bytes(updated)
    drive,created=client_drive(client)
    try:
        process=subprocess.Popen([drive+'\\GameClient\\DeicideOnline.exe'],cwd=drive+'\\GameClient',
                                 stdin=subprocess.DEVNULL,stdout=subprocess.DEVNULL,stderr=subprocess.DEVNULL)
    except BaseException:
        if created:release_drive(drive,client)
        raise
    process.laqia_created_drive=drive if created else None
    return process

def release_client_drive(process):
    drive=getattr(process,'laqia_created_drive',None)
    if isinstance(drive,str) and process.poll() is not None:release_drive(drive,ROOT/'client')

def game():
    # The desktop launcher owns the lifecycle lock and watches the actual EXE.
    from family_launcher import main
    main('local')
def stop():
    if not online():
        print('Server is not reachable; no process was force-stopped.')
        return
    with connect() as c:
        # GameServer SIGTERM saves every connected character and flushes DataServer.
        # Keep DataServer alive until those final writes have been consumed.
        command(c,'systemctl stop LAQIA_LoginServer && systemctl stop LAQIA_GameServer && sleep 3 && systemctl stop LAQIA_DataServer')
        command(c,'mariadb-dump --databases LoginAccount LAQIA_GAMEDB LAQIA_ITEMLOG LAQIA_CASH DAUM_BILLING > /root/laqia-backup.sql')
        folder=ROOT/'backups'; folder.mkdir(exist_ok=True)
        with c.open_sftp() as s:
            s.get('/root/laqia-backup.sql',str(folder/('local-'+time.strftime('%Y%m%d-%H%M%S')+'.sql')))
        command(c,'systemd-run --on-active=2 /sbin/poweroff')
    print('Saved database backup and requested clean VM shutdown.')
    for _ in range(60):
        if not port_open(22222) and not port_open(44444):
            print('Server shutdown complete.');return
        time.sleep(1)
    raise RuntimeError('종료 요청 후 서버가 아직 응답합니다. 강제 종료하지 않았습니다.')
def status():
    with connect() as c:
        print(command(c,'systemctl is-active '+SERVICES))
        print(command(c,"mysql -N -e 'SELECT COUNT(*) FROM LAQIA_GAMEDB.UserTable'"))
def manual_stop():
    from family_network import SessionLock,Hosting,read_local
    with SessionLock():
        if client_running(): raise RuntimeError('게임 창을 닫으면 서버도 자동으로 종료됩니다. 먼저 게임을 종료해 주세요.')
        host=Hosting();host.ports=read_local('family-serve-state.json').get('ports',[])
        try:
            if host.ports:host.close()
        finally:stop()
if __name__=='__main__':
    action=sys.argv[1] if len(sys.argv)>1 else 'status'
    {'start':start,'game':game,'stop':manual_stop,'status':status}[action]()
