"""Build a hash manifest and the minimal father updater ZIP (no saves/keys)."""
from pathlib import Path
import hashlib,json,zipfile
ROOT=Path(__file__).resolve().parents[1]
def main():
    files=[]
    for kind,folder,prefix in [('client','client-overlay',''),('server','game-data','')]:
        candidates=(ROOT/folder).rglob('*') if kind=='client' else (ROOT/folder/'DATA').glob('*')
        for p in sorted(candidates):
            if not p.is_file():continue
            name=p.relative_to(ROOT/folder).as_posix()
            if kind=='client' and name.lower() in ('config.ini','server.ini'):continue
            if kind=='server' and not name.startswith('DATA/'):continue
            if kind=='server' and name=='DATA/ADMIN_INFO.txt':continue
            files.append(dict(kind=kind,path=name,source=p.relative_to(ROOT).as_posix(),bytes=p.stat().st_size,sha256=hashlib.sha256(p.read_bytes()).hexdigest()))
    for kind,path,source in [('server','LAQIA_GameServer','server-bin/LAQIA_GameServer'),('updater','update/father_update.py','distribution/father_update.py')]:
        p=ROOT/source
        files.append(dict(kind=kind,path=path,source=source,bytes=p.stat().st_size,sha256=hashlib.sha256(p.read_bytes()).hexdigest()))
    manifest=dict(format=1,repository='MetherMan/LAQIA-THE-LEGEND',version='2026-09-18-twilight-enchant20',files=files)
    (ROOT/'distribution/update-manifest.json').write_text(json.dumps(manifest,ensure_ascii=False,indent=2)+'\n',encoding='utf-8')
    output=ROOT.parent/'아버지_자동업뎃.zip'
    cmd='''@echo off
chcp 65001 >nul
cd /d "%~dp0"
if not exist "work\\laqia-runtime\\python\\python.exe" (
 echo [오류] 이 파일은 기존 라키아 폴더 안에 넣어 주세요.
 pause
 exit /b 1
)
"work\\laqia-runtime\\python\\python.exe" -X utf8 "update\\father_update.py" --root "%~dp0."
if errorlevel 1 (
 echo 업데이트가 완료되지 않았습니다. 위 오류를 확인해 주세요.
 pause
 exit /b 1
)
pause
'''.replace('\n','\r\n').encode('utf-8')
    note='''1. 게임을 종료합니다.
2. 이 압축 안의 라키아 폴더를 기존 라키아 폴더에 합쳐 덮어씌웁니다.
   기존 라키아 폴더에는 outputs와 work 폴더가 있습니다.
3. 라키아/자동업데이트.cmd를 더블클릭합니다.
4. 업데이트 완료 문구가 나오면 기존 게임 실행 버튼을 누릅니다.

앞으로도 자동업데이트.cmd만 누르면 됩니다. Git 설치나 GitHub 로그인은 필요 없습니다.
업데이트할 때 MetherMan/LAQIA-THE-LEGEND 저장소가 public 상태여야 합니다.
private이거나 인터넷 연결에 실패하면 업데이트를 중단합니다. 이미 받은 게임은 계속 사용할 수 있습니다.
계정, 캐릭터, 장비, DB, VM 디스크, 가족 연결 설정은 교체하지 않습니다.
변경한 파일은 work/laqia-runtime/backups/auto-update-*에 백업합니다.
'''
    with zipfile.ZipFile(output,'w',zipfile.ZIP_DEFLATED) as z:
        z.writestr('라키아/자동업데이트.cmd',cmd)
        z.write(ROOT/'distribution/father_update.py','라키아/update/father_update.py')
        z.writestr('적용방법.txt',note.encode('utf-8-sig'))
    print(json.dumps(dict(zip=str(output),bytes=output.stat().st_size,manifest_files=len(files),sha256=hashlib.sha256(output.read_bytes()).hexdigest()),ensure_ascii=False))
if __name__=='__main__':main()
