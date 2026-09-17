"""Windows process and path support for a copied, self-contained game folder."""
from pathlib import Path
import ctypes,subprocess,tempfile
from ctypes import wintypes
ROOT=Path(__file__).resolve().parent
FLAGS=subprocess.CREATE_NO_WINDOW

def client_running():
    class Entry(ctypes.Structure):
        _fields_=[('size',wintypes.DWORD),('usage',wintypes.DWORD),('pid',wintypes.DWORD),('heap',ctypes.c_size_t),('module',wintypes.DWORD),('threads',wintypes.DWORD),('parent',wintypes.DWORD),('priority',wintypes.LONG),('flags',wintypes.DWORD),('exe',wintypes.WCHAR*260)]
    k=ctypes.WinDLL('kernel32',use_last_error=True);k.CreateToolhelp32Snapshot.restype=wintypes.HANDLE
    k.Process32FirstW.argtypes=[wintypes.HANDLE,ctypes.POINTER(Entry)];k.Process32NextW.argtypes=k.Process32FirstW.argtypes;k.CloseHandle.argtypes=[wintypes.HANDLE]
    h=k.CreateToolhelp32Snapshot(2,0)
    if h==ctypes.c_void_p(-1).value:raise ctypes.WinError(ctypes.get_last_error())
    try:
        e=Entry();e.size=ctypes.sizeof(e);ok=k.Process32FirstW(h,ctypes.byref(e))
        if not ok:raise ctypes.WinError(ctypes.get_last_error())
        while ok:
            if e.exe.lower()=='deicideonline.exe':return True
            ok=k.Process32NextW(h,ctypes.byref(e))
        return False
    finally:k.CloseHandle(h)

def device_target(letter):
    k=ctypes.WinDLL('kernel32',use_last_error=True)
    k.QueryDosDeviceW.argtypes=[wintypes.LPCWSTR,wintypes.LPWSTR,wintypes.DWORD]
    buffer=ctypes.create_unicode_buffer(32768)
    if k.QueryDosDeviceW(letter,buffer,len(buffer)):return buffer.value
    error=ctypes.get_last_error()
    if error in (2,3):return None
    raise ctypes.WinError(error)

def client_drive(client):
    """Reuse our mapping or choose an unused letter; never replace another drive."""
    client=Path(client).resolve();letters=[c+':' for c in 'QRSTUVWXYZPONMLKJIHGFED']
    targets={letter:device_target(letter) for letter in letters}
    for letter,target in targets.items():
        if target and target.startswith('\\??\\') and Path(target[4:]).resolve()==client:return letter,False
    for letter,target in targets.items():
        if target is None:
            p=subprocess.run(['subst.exe',letter,str(client)],capture_output=True,creationflags=FLAGS)
            if p.returncode:continue
            actual=device_target(letter)
            if actual and actual.startswith('\\??\\') and Path(actual[4:]).resolve()==client:return letter,True
    if any(target is None for target in targets.values()):
        raise RuntimeError('게임 폴더를 가상 드라이브에 연결하지 못했습니다. Windows의 실행 권한이나 보안 프로그램 차단 내역을 확인해 주세요.')
    raise RuntimeError('게임 경로에 사용할 빈 드라이브 문자가 없습니다. 사용하지 않는 가상 드라이브를 해제한 뒤 실행해 주세요.')

def release_drive(letter,client):
    target=device_target(letter)
    if target and target.startswith('\\??\\') and Path(target[4:]).resolve()==Path(client).resolve():
        subprocess.run(['subst.exe',letter,'/D'],check=True,capture_output=True,creationflags=FLAGS)

def preflight(root=ROOT,client=True,server=True):
    root=Path(root)
    files=['python/python.exe','python/pythonw.exe','python/python312.dll','ssh_vm.py','pylibs/paramiko/__init__.py']
    if client:files += ['client/GameClient/'+n for n in ['DeicideOnline.exe','Engine.dll','Render.dll','Common.dll','Media.dll','Sound.dll','UInterface.dll','mss32.dll','server.ini','start.lua','Item/ITEM.dat','Item/Skill.dat','Interface/ui.ini']]
    if server:files += ['laqia-vm.qcow2','debian-base.qcow2','seed.iso','vm-ssh-key','qemu/qemu-system-x86_64.exe','qemu/share/bios-256k.bin','start_vm.py']
    missing=[name for name in files if not (root/name).is_file() or (root/name).stat().st_size==0]
    if missing:raise RuntimeError('필수 파일이 없거나 비어 있습니다. 압축을 모두 풀어 주세요.\n'+'\n'.join(missing[:8]))
    try:
        with tempfile.TemporaryFile(dir=root):pass
    except OSError as error:raise RuntimeError('게임 폴더에 저장할 수 없습니다. 다운로드 또는 문서 폴더에 압축을 풀어 주세요.') from error
