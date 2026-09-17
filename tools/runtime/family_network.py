"""Private family connections. Never publish SSH, MariaDB, or a public Funnel."""
from pathlib import Path
import ctypes, ipaddress, json, os, shutil, socket, subprocess, time, winreg

ROOT=Path(__file__).resolve().parent
PORTS=(8030,2560)
NETWORK=ipaddress.ip_network('100.64.0.0/10')
FLAGS=subprocess.CREATE_NO_WINDOW

def machine_id():
    with winreg.OpenKey(winreg.HKEY_LOCAL_MACHINE,r'SOFTWARE\Microsoft\Cryptography',0,
                        winreg.KEY_READ|winreg.KEY_WOW64_64KEY) as key:
        return winreg.QueryValueEx(key,'MachineGuid')[0]

def read_local(name):
    path=ROOT/name
    if not path.exists(): return {}
    data=json.loads(path.read_text(encoding='utf-8'))
    return data if data.get('machine')==machine_id() else {}

def write_local(name,data):
    target=ROOT/name
    temp=target.with_suffix('.tmp')
    temp.write_text(json.dumps(dict(data,machine=machine_id()),ensure_ascii=False,indent=2),encoding='utf-8')
    temp.replace(target)

def executable():
    path=Path(os.environ.get('ProgramFiles',r'C:\Program Files'))/'Tailscale/tailscale.exe'
    found=str(path) if path.is_file() else shutil.which('tailscale')
    if not found: raise RuntimeError('Tailscale 설치가 필요합니다. 가족 연결 설정 버튼을 실행해 주세요.')
    return found

def cli(*args):
    p=subprocess.run([executable(),*args],capture_output=True,encoding='utf-8',errors='replace',
                     timeout=20,creationflags=FLAGS)
    if p.returncode: raise RuntimeError(p.stderr.strip() or p.stdout.strip() or 'Tailscale 명령 실패')
    return p.stdout

def ready():
    data=json.loads(cli('status','--json'))
    if data.get('BackendState')!='Running':
        raise RuntimeError('Tailscale 로그인이 필요합니다. 가족 연결 설정 버튼을 실행해 주세요.')
    return data

def ipv4(device):
    for value in device.get('TailscaleIPs',[]):
        address=ipaddress.ip_address(value)
        if address.version==4 and address in NETWORK: return str(address)
    raise RuntimeError('사설 연결 주소를 찾지 못했습니다.')

def resolve_peer(value):
    data=ready()
    peers=list((data.get('Peer') or {}).values())
    for peer in peers:
        names=[peer.get('HostName',''),peer.get('DNSName','').rstrip('.'),*peer.get('TailscaleIPs',[])]
        if value.lower().rstrip('.') in [name.lower() for name in names]:
            if not peer.get('Online'): raise RuntimeError('아버지 PC가 연결되어 있지 않습니다. 아버지가 게임을 켠 뒤 다시 눌러 주세요.')
            return ipv4(peer)
    raise RuntimeError('연결된 가족 PC 목록에 없는 주소입니다. 두 PC를 같은 Tailscale 계정으로 연결하거나 기기 공유를 먼저 완료해 주세요.')

def probe(host):
    for port in PORTS:
        try:
            with socket.create_connection((host,port),timeout=5): pass
        except OSError as error:
            raise RuntimeError('아버지 서버가 아직 열리지 않았습니다. 아버지가 가족 게임 시작을 누르고 로딩을 마친 뒤 다시 눌러 주세요.') from error

def serve_config():
    return json.loads(cli('serve','status','--json') or '{}') or {}

class Hosting:
    """Remember only our two port mappings; preserve unrelated Serve settings."""
    def __init__(self): self.ports=[]
    def start(self):
        address=ipv4(ready()['Self'])
        stale=read_local('family-serve-state.json').get('ports',[])
        config=serve_config()
        # Refuse an existing/public mapping, including nested foreground handlers.
        if config.get('AllowFunnel') and any(config['AllowFunnel'].values()):
            raise RuntimeError('이 PC의 공개 Funnel 설정을 먼저 확인해야 합니다. 가족 게임 포트를 열지 않았습니다.')
        for port in PORTS:
            if any(str(port) in (entry.get('TCP') or {}) for entry in (config.get('Foreground') or {}).values()):
                raise RuntimeError(f'{port} 포트를 다른 Tailscale 서비스가 사용 중입니다.')
            current=(config.get('TCP') or {}).get(str(port))
            if current and not (port in stale and current=={'TCPForward':f'127.0.0.1:{port}'}):
                raise RuntimeError(f'{port} 포트를 다른 Tailscale 서비스가 사용 중입니다.')
        try:
            for port in PORTS:
                # Record before mutation so a killed launcher can recover its own port.
                self.ports.append(port)
                write_local('family-serve-state.json',{'ports':self.ports})
                cli('serve','--bg',f'--tcp={port}',f'tcp://127.0.0.1:{port}')
                assert (serve_config().get('TCP') or {}).get(str(port))=={'TCPForward':f'127.0.0.1:{port}'}
        except BaseException:
            self.close();raise
        write_local('family-host-address.json',{'address':address})
        return address
    def close(self):
        failures=[]
        for port in list(self.ports):
            try:
                current=(serve_config().get('TCP') or {}).get(str(port))
                if current=={'TCPForward':f'127.0.0.1:{port}'}:
                    cli('serve','--bg',f'--tcp={port}','off')
                self.ports.remove(port)
            except Exception as error: failures.append(str(error))
        write_local('family-serve-state.json',{'ports':self.ports})
        if failures: raise RuntimeError('\n'.join(failures))

class SessionLock:
    def __enter__(self):
        k=ctypes.WinDLL('kernel32',use_last_error=True)
        k.CreateMutexW.argtypes=[ctypes.c_void_p,ctypes.c_int,ctypes.c_wchar_p]
        k.CreateMutexW.restype=ctypes.c_void_p
        self.kernel=k
        self.handle=k.CreateMutexW(None,True,r'Local\LaqiaFamilyGameSession')
        if not self.handle: raise ctypes.WinError(ctypes.get_last_error())
        if ctypes.get_last_error()==183:
            self.__exit__(None,None,None)
            raise RuntimeError('게임이 실행 중이거나 서버를 저장하고 있습니다. 잠시 기다려 주세요.')
        return self
    def __exit__(self,*args):
        self.kernel.CloseHandle.argtypes=[ctypes.c_void_p]
        self.kernel.CloseHandle(self.handle)
