"""Build a hash manifest and the minimal father updater ZIP (no saves/keys)."""
from pathlib import Path
import argparse,hashlib,json,zipfile
ROOT=Path(__file__).resolve().parents[1]
def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--version',default='2026-09-18-twilight-trail-init')
    parser.add_argument('--manifest-only',action='store_true')
    parser.add_argument('--output',type=Path,default=ROOT/'dist/아버지_자동업뎃.zip')
    args=parser.parse_args()
    files=[]
    for kind,folder,prefix in [('client','client-overlay',''),('server','game-data','')]:
        candidates=(ROOT/folder).rglob('*') if kind=='client' else (ROOT/folder/'DATA').glob('*')
        for p in sorted(candidates):
            if not p.is_file():continue
            if '__pycache__' in p.parts or p.suffix.lower() in ('.pyc','.pyo','.tmp','.log'):continue
            name=p.relative_to(ROOT/folder).as_posix()
            if kind=='client' and name.lower() in ('config.ini','server.ini'):continue
            if kind=='server' and not name.startswith('DATA/'):continue
            if kind=='server' and name=='DATA/ADMIN_INFO.txt':continue
            files.append(dict(kind=kind,path=name,source=p.relative_to(ROOT).as_posix(),bytes=p.stat().st_size,sha256=hashlib.sha256(p.read_bytes()).hexdigest()))
    for kind,path,source in [('server','LAQIA_GameServer','server-bin/LAQIA_GameServer'),('updater','update/father_update.py','distribution/father_update.py')]:
        p=ROOT/source
        files.append(dict(kind=kind,path=path,source=source,bytes=p.stat().st_size,sha256=hashlib.sha256(p.read_bytes()).hexdigest()))
    runtime_files=[]
    for name in ['local_control.py','ssh_vm.py','start_vm.py']:
        p=ROOT/'tools/runtime'/name
        runtime_files.append(dict(kind='runtime',path=name,source=p.relative_to(ROOT).as_posix(),
                                  bytes=p.stat().st_size,sha256=hashlib.sha256(p.read_bytes()).hexdigest()))
    manifest=dict(format=1,repository='MetherMan/pegin',version=args.version,files=files,runtime_files=runtime_files)
    (ROOT/'distribution/update-manifest.json').write_text(json.dumps(manifest,ensure_ascii=False,indent=2)+'\n',encoding='utf-8')
    if args.manifest_only:
        print(json.dumps(dict(version=args.version,manifest_files=len(files))))
        return
    output=args.output
    output.parent.mkdir(parents=True,exist_ok=True)
    cmd='''@echo off
chcp 65001 >nul
cd /d "%~dp0"
if not exist "work\\laqia-runtime\\python\\python.exe" (
 echo [오류] outputs와 work 두 폴더가 함께 보이는 곳에 넣어 주세요.
 echo outputs 안이나 work 안에 넣으면 안 됩니다. 적용방법.txt를 확인해 주세요.
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
    note='''[VM did not become ready 오류가 난 경우 / 처음 설치]

복구용 수정본입니다. 기존 자동업데이트는 서버가 켜져야 자신을 갱신하므로,
이 오류가 난 PC에는 아래 두 항목을 한 번 직접 덮어써야 합니다.
게임과 업데이트 창을 닫고 복사하세요. VM 디스크·계정·저장 데이터는 교체하지 않습니다.

1. 게임을 끕니다.

2. 아버지_자동업뎃.zip의 압축을 풀고, 그 안의 '라키아' 폴더를 엽니다.
   여기 있는 아래 두 개만 복사합니다.

   - 자동업데이트.cmd 파일
   - update 폴더

3. 원래 게임이 있는 '라키아-아버지용-최종-20260915' 폴더를 엽니다.
   그 안의 '라키아' 폴더를 한 번 더 엽니다.
   'outputs'와 'work' 두 폴더가 함께 보이면 맞는 곳입니다.

   outputs나 work 안으로 들어가지 마세요!
   바로 이 화면의 빈 곳에, 복사한 두 개를 붙여넣으세요.
   같은 파일이 있다고 나오면 '덮어쓰기' 또는 '파일 바꾸기'를 선택하세요.

4. 붙여넣고 나면 아래처럼 네 개가 같은 위치에 있어야 합니다.

   라키아-아버지용-최종-20260915
   └─ 라키아
      ├─ outputs             ← 원래 있던 폴더
      ├─ work                ← 원래 있던 폴더
      ├─ update              ← 방금 넣은 폴더
      └─ 자동업데이트.cmd    ← 방금 넣은 파일. 이것을 더블클릭!

5. '자동업데이트.cmd'를 더블클릭합니다.
   '업데이트 완료!'가 나오면 창을 닫고, 평소 쓰던 버튼으로 게임을 켭니다.

[다음부터는]
게임을 끈 뒤 '자동업데이트.cmd'만 더블클릭하면 됩니다.
인터넷에 연결되어 있어야 합니다. 계정·캐릭터·장비는 그대로 유지됩니다.
'업데이트 중단'이 나오면 창의 내용을 아들에게 알려 주세요.
서버 준비가 계속 실패하면 work/laqia-runtime/vm-start-diagnostic.txt를 함께 확인해 주세요.
'''
    with zipfile.ZipFile(output,'w',zipfile.ZIP_DEFLATED) as z:
        z.writestr('라키아/자동업데이트.cmd',cmd)
        z.write(ROOT/'distribution/father_update.py','라키아/update/father_update.py')
        z.writestr('적용방법.txt',note.encode('utf-8-sig'))
    print(json.dumps(dict(zip=str(output),bytes=output.stat().st_size,manifest_files=len(files),sha256=hashlib.sha256(output.read_bytes()).hexdigest()),ensure_ascii=False))
if __name__=='__main__':main()
