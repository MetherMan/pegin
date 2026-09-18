"""Public GitHub updater for the existing 20260915 family installation.

No Git installation, login, database import, or VM replacement is needed.
Only manifest-listed client assets, static server DATA and GameServer change.
"""
from pathlib import Path, PurePosixPath
import argparse, hashlib, json, os, shutil, subprocess, sys, time, uuid
import urllib.request, urllib.error, urllib.parse

REPOSITORY = 'MetherMan/LAQIA-THE-LEGEND'
MANIFEST = 'distribution/update-manifest.json'
SERVER = '/opt/laqia/server/LAQIA_GameServer'

def sha(path):
    with Path(path).open('rb') as f:
        return hashlib.file_digest(f, 'sha256').hexdigest()

def atomic_copy(source, target):
    target = Path(target)
    target.parent.mkdir(parents=True, exist_ok=True)
    temporary = target.with_name(target.name + '.update-' + uuid.uuid4().hex)
    try:
        shutil.copyfile(source, temporary)
        os.replace(temporary, target)
    finally:
        if temporary.exists(): temporary.unlink()

def safe_path(root, relative):
    p = PurePosixPath(relative)
    if not relative or p.is_absolute() or any(x in ('..', '.', '') for x in p.parts) or '\\' in relative or ':' in relative:
        raise ValueError('Unsafe update path: ' + relative)
    root = Path(root).resolve()
    target = (root / relative).resolve()
    if not target.is_relative_to(root): raise ValueError('Update path escaped installation')
    return target

def read_manifest(folder):
    data = json.loads((Path(folder)/MANIFEST).read_text(encoding='utf-8'))
    if data.get('format') != 1 or data.get('repository') != REPOSITORY: raise ValueError('Unknown update manifest')
    seen = set()
    for entry in data['files']:
        kind, name, source = entry['kind'], entry['path'], entry['source']
        safe_path(folder, source)
        safe_path(folder, name)
        if (kind, name.lower()) in seen: raise ValueError('Duplicate update target')
        seen.add((kind, name.lower()))
        if kind == 'client':
            if source != 'client-overlay/'+name: raise ValueError('Invalid client source')
            if name.lower() in ('config.ini','server.ini'): raise ValueError('Personal client settings excluded')
        elif kind == 'server':
            if name != 'LAQIA_GameServer' and not name.startswith('DATA/'): raise ValueError('Only static server files may be updated')
            if name=='DATA/ADMIN_INFO.txt':raise ValueError('Local administrator settings excluded')
            expected = 'server-bin/LAQIA_GameServer' if name == 'LAQIA_GameServer' else 'game-data/'+name
            if source != expected: raise ValueError('Invalid server source')
        elif kind == 'updater':
            if name != 'update/father_update.py' or source != 'distribution/father_update.py': raise ValueError('Invalid updater source')
        else: raise ValueError('Unknown update file category')
        if len(entry['sha256']) != 64 or entry['bytes'] <= 0: raise ValueError('Invalid file metadata')
    return data

def download(url, target, expected=None, length=None):
    request = urllib.request.Request(url, headers={'User-Agent':'LAQIA-Family-Updater','Accept':'application/vnd.github+json'})
    target.parent.mkdir(parents=True,exist_ok=True)
    try:
        with urllib.request.urlopen(request, timeout=60) as response, target.open('wb') as out:
            shutil.copyfileobj(response, out)
    except urllib.error.HTTPError as error:
        if error.code in (401,403,404):
            raise RuntimeError('업데이트에 접근할 수 없습니다. 저장소가 public인지 확인해 주세요. 게임 파일은 아직 변경하지 않았습니다.') from error
        raise
    if expected and (sha(target)!=expected or target.stat().st_size!=length):
        raise RuntimeError('다운로드 파일 검증 실패: '+target.name)

def local_target(root, runtime, entry):
    if entry['kind']=='client': return safe_path(runtime/'client/GameClient',entry['path'])
    if entry['kind']=='updater': return safe_path(root,entry['path'])
    return None

def fetch(root, runtime):
    staging = root/'update/downloads'/uuid.uuid4().hex
    staging.mkdir(parents=True)
    info = staging/'revision.json'
    download('https://api.github.com/repos/'+REPOSITORY+'/commits/main',info)
    revision = json.loads(info.read_text())['sha']
    if len(revision)!=40 or any(c not in '0123456789abcdef' for c in revision): raise ValueError('Invalid revision')
    base = 'https://raw.githubusercontent.com/'+REPOSITORY+'/'+revision+'/'
    download(base+MANIFEST, staging/MANIFEST)
    manifest = read_manifest(staging)
    print('최신 버전 확인: '+revision[:12],flush=True)
    cache = root/'update/cache'
    cache.mkdir(parents=True,exist_ok=True)
    downloaded=0
    for e in manifest['files']:
        target = safe_path(staging,e['source'])
        cached = cache/e['sha256']
        local = local_target(root,runtime,e)
        if local and local.is_file() and sha(local)==e['sha256']:
            atomic_copy(local,target)
        elif cached.is_file() and sha(cached)==e['sha256']:
            atomic_copy(cached,target)
        else:
            print('받는 중: '+e['path'],flush=True)
            download(base+urllib.parse.quote(e['source'],safe='/'),target,e['sha256'],e['bytes'])
            atomic_copy(target,cached);downloaded+=1
    manifest['revision']=revision
    (staging/MANIFEST).write_text(json.dumps(manifest,ensure_ascii=False,indent=2),encoding='utf-8')
    print('다운로드 검증 완료: '+str(downloaded)+'개 파일',flush=True)
    return staging

class LocalTransaction:
    def __init__(self, backup):
        self.backup=backup;self.entries=[]
    def put(self, source, target):
        target=Path(target)
        existed=target.exists();saved=self.backup/str(len(self.entries))
        if existed: atomic_copy(target,saved)
        self.entries.append((target,saved,existed))
        self.backup.mkdir(parents=True,exist_ok=True)
        (self.backup/'files.json').write_text(json.dumps([(str(a),str(b),c) for a,b,c in self.entries],ensure_ascii=False,indent=2),encoding='utf-8')
        atomic_copy(source,target)
    def rollback(self):
        for target,saved,existed in reversed(self.entries):
            if existed: atomic_copy(saved,target)
            elif target.exists(): target.unlink()

def apply(root, runtime, source):
    import shlex
    manifest=read_manifest(source)
    for e in manifest['files']:
        p=safe_path(source,e['source'])
        if not p.is_file() or p.stat().st_size!=e['bytes'] or sha(p)!=e['sha256']:
            raise RuntimeError('적용 전 파일 검증 실패: '+e['path'])
    sys.path[:0]=[str(runtime),str(runtime/'pylibs')]
    import local_control as ctl
    from ssh_vm import connect
    from family_network import SessionLock
    with SessionLock():
        if ctl.client_running(): raise RuntimeError('게임을 종료한 뒤 다시 눌러 주세요.')
        was_online=ctl.online();started=False
        stamp=time.strftime('%Y%m%d-%H%M%S')+'-'+uuid.uuid4().hex[:8]
        backup=runtime/'backups'/('auto-update-'+stamp)
        local=LocalTransaction(backup/'files')
        try:
            ctl.start();started=True
            with connect() as c:
                cmd=lambda text:ctl.command(c,text)
                if cmd("ss -Htn state established 'sport = :2560'").strip():
                    raise RuntimeError('접속한 플레이어가 있습니다. 모두 종료한 뒤 업데이트해 주세요.')
                server_entries=[e for e in manifest['files'] if e['kind']=='server']
                with c.open_sftp() as s:
                    changed=[]
                    for e in server_entries:
                        try:
                            with s.open(SERVER+'/'+e['path'],'rb') as f:
                                h=hashlib.sha256()
                                while block:=f.read(1024*1024): h.update(block)
                            same=h.hexdigest()==e['sha256']
                        except IOError: same=False
                        if not same: changed.append(e)
                remote_backup='/opt/laqia/update-backups/'+stamp
                stage='/opt/laqia/update-stage/'+stamp
                remote_touched=False
                try:
                    if changed:
                        print('서버 변경 파일 검증 및 백업 중...',flush=True)
                        cmd('mkdir -p '+stage+' '+remote_backup)
                        with c.open_sftp() as s:
                            for e in changed:
                                remote=stage+'/'+e['path']
                                cmd('mkdir -p '+shlex.quote(str(PurePosixPath(remote).parent)))
                                s.put(str(safe_path(source,e['source'])),remote)
                                if cmd('sha256sum '+shlex.quote(remote)).split()[0]!=e['sha256']:raise RuntimeError('서버 전송 검증 실패')
                        cmd('cp -a '+SERVER+'/DATA '+remote_backup+'/ && cp -p '+SERVER+'/LAQIA_GameServer '+remote_backup+'/')
                        # Stop the game before modifying static files. DataServer/DB remain intact.
                        cmd('systemctl stop LAQIA_GameServer');remote_touched=True
                        for e in changed:
                            dest=SERVER+'/'+e['path']
                            cmd('mkdir -p '+shlex.quote(str(PurePosixPath(dest).parent))+' && cp '+shlex.quote(stage+'/'+e['path'])+' '+shlex.quote(dest))
                        cmd('chmod 755 '+SERVER+'/LAQIA_GameServer && chown -R laqia:laqia '+SERVER+'/DATA '+SERVER+'/LAQIA_GameServer')
                        cmd('systemctl start LAQIA_GameServer');time.sleep(3)
                        if cmd('systemctl is-active LAQIA_GameServer').strip()!='active':raise RuntimeError('새 서버 시작 실패')
                        if not ctl.port_open(2560):raise RuntimeError('새 서버 접속 포트 확인 실패')
                    for e in manifest['files']:
                        dest=local_target(root,runtime,e)
                        if dest and (not dest.is_file() or sha(dest)!=e['sha256']):
                            local.put(safe_path(source,e['source']),dest)
                    # The old family bundle keeps a local static-server mirror as well.
                    mirror=runtime/'server-local/LAQIA_GameServer'
                    if mirror.is_dir():
                        for e in server_entries:
                            dest=safe_path(mirror,e['path'])
                            if not dest.is_file() or sha(dest)!=e['sha256']:local.put(safe_path(source,e['source']),dest)
                    result={'version':manifest['version'],'revision':manifest.get('revision'),'server_files_changed':len(changed),'local_files_changed':len(local.entries),'database_replaced':False,'backup':str(backup)}
                    state=root/'update/installed.json';state.parent.mkdir(parents=True,exist_ok=True)
                    state.write_text(json.dumps(result,ensure_ascii=False,indent=2),encoding='utf-8')
                    print(json.dumps(result,ensure_ascii=False),flush=True)
                except BaseException:
                    try:local.rollback()
                    finally:
                        if remote_touched:
                            cmd('systemctl stop LAQIA_GameServer')
                            # Preserve the failed static data for diagnosis; never touch SQL data.
                            cmd('mv '+SERVER+'/DATA '+remote_backup+'/failed-DATA && cp -a '+remote_backup+'/DATA '+SERVER+'/ && cp -p '+remote_backup+'/LAQIA_GameServer '+SERVER+'/LAQIA_GameServer && systemctl start LAQIA_GameServer')
                    raise
        finally:
            if started and not was_online and ctl.online() and not ctl.client_running():ctl.stop()
    print('업데이트 완료! 기존 실행 버튼으로 게임을 켜 주세요. 계정·캐릭터·장비는 유지했습니다.',flush=True)

def main():
    parser=argparse.ArgumentParser()
    parser.add_argument('--root',type=Path,required=True)
    parser.add_argument('--runtime',type=Path)
    parser.add_argument('--source',type=Path)
    parser.add_argument('--download-only',action='store_true')
    args=parser.parse_args()
    root=args.root.resolve();runtime=(args.runtime or root/'work/laqia-runtime').resolve()
    if not (runtime/'python/python.exe').is_file() or not (runtime/'local_control.py').is_file():
        raise RuntimeError('라키아 폴더 안에 덮어씌워 주세요. work/laqia-runtime 폴더를 찾을 수 없습니다.')
    source=args.source or fetch(root,runtime)
    if not args.download_only:apply(root,runtime,source)

if __name__=='__main__':
    sys.stdout.reconfigure(encoding='utf-8')
    try:main()
    except Exception as error:
        print('\n업데이트 중단: '+str(error),flush=True)
        print('실패 상태를 성공으로 처리하지 않았습니다. 창의 내용을 확인해 주세요.',flush=True)
        sys.exit(1)
