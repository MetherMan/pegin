"""Export persistent account/character databases, independently of server binaries."""
from pathlib import Path
import datetime,hashlib,json,os,queue,sys,tempfile,threading,traceback,uuid,zipfile
from contextlib import contextmanager
import family_network as network
import local_control as control
from ssh_vm import connect
ROOT=Path(__file__).resolve().parent
DATABASES=['LoginAccount','LAQIA_GAMEDB','LAQIA_ITEMLOG','LAQIA_CASH','DAUM_BILLING']
SERVICES=['LAQIA_DataServer','LAQIA_GameServer','LAQIA_LoginServer']

@contextmanager
def export_workdir(parent):
    # Inherit the selected folder's Windows ACL; Python's 0700 temp directories
    # can exclude the restricted token used by launchers and validation tools.
    folder=Path(parent)/('laqia-character-export-'+uuid.uuid4().hex)
    folder.mkdir()
    try:yield folder
    finally:
        for name in ['characters.sql','backup.zip']:
            (folder/name).unlink(missing_ok=True)
        folder.rmdir()

def export_characters(destination,notify=lambda text:None):
    destination=Path(destination).resolve()
    if destination.is_relative_to(ROOT.parent.parent):
        raise RuntimeError('기존 게임 폴더를 지워도 백업이 남도록 바탕화면이나 별도 폴더를 선택해 주세요.')
    if destination.suffix.lower()!='.zip':raise RuntimeError('백업 파일 이름은 .zip으로 끝나야 합니다.')
    if destination.exists():raise RuntimeError('같은 이름의 파일이 있습니다. 다른 이름으로 저장해 주세요.')
    if not destination.parent.is_dir():raise RuntimeError('저장할 폴더를 찾을 수 없습니다.')
    with network.SessionLock():
        if control.client_running():raise RuntimeError('게임을 종료하고 저장 창이 사라진 뒤 캐릭터 데이터 추출을 실행해 주세요.')
        control.preflight(client=False)
        started=not control.online();previous=[];quiesced=False
        try:
            if started:
                notify('저장 데이터를 읽기 위해 서버를 준비하고 있어요…');control.start()
            with connect() as c:
                cmd=lambda text:control.command(c,text)
                if cmd("ss -Htn state established 'sport = :2560'").strip():
                    raise RuntimeError('서버에 접속한 사람이 있습니다. 모두 게임을 종료한 뒤 추출해 주세요.')
                statuses=cmd('systemctl is-active '+' '.join(SERVICES)+' || true').splitlines()
                previous=[name for name,status in zip(SERVICES,statuses) if status=='active']
                # Stop game writes before the consistent database snapshot.
                quiesced=True
                cmd('systemctl stop LAQIA_LoginServer && systemctl stop LAQIA_GameServer && sleep 2 && systemctl stop LAQIA_DataServer')
                notify('계정·캐릭터·아이템·장비·캐시 데이터를 추출하고 있어요…')
                remote='/root/laqia-character-export-'+uuid.uuid4().hex+'.sql'
                try:
                    cmd('mariadb-dump --lock-all-tables --routines --events --triggers --hex-blob --databases '+' '.join(DATABASES)+' > '+remote)
                    counts=cmd('mariadb -N -B LAQIA_GAMEDB -e "SELECT COUNT(*) FROM UserTable; SELECT COUNT(*) FROM InvenItems; SELECT COUNT(*) FROM EquipItems;"').splitlines()
                    with export_workdir(destination.parent) as folder:
                        temp=Path(folder);sql=temp/'characters.sql'
                        with c.open_sftp() as s:s.get(remote,str(sql))
                        data=sql.read_bytes()
                        if len(data)<1000 or not all(('`'+name+'`').encode() in data for name in DATABASES):raise RuntimeError('백업 내용 검증에 실패했습니다. 기존 데이터는 그대로 유지됩니다.')
                        meta=dict(format='laqia-character-backup',version=1,created_utc=datetime.datetime.now(datetime.timezone.utc).isoformat(),databases=DATABASES,sql_sha256=hashlib.sha256(data).hexdigest(),counts=dict(zip(['characters','inventory_items','equipped_items'],map(int,counts))))
                        archive=temp/'backup.zip'
                        with zipfile.ZipFile(archive,'w',compression=zipfile.ZIP_DEFLATED,compresslevel=6) as z:
                            z.write(sql,'characters.sql');z.writestr('backup-info.json',json.dumps(meta,ensure_ascii=False,indent=2))
                            z.writestr('복원 안내.txt','라키아 캐릭터 데이터 백업\n\n계정, 캐릭터, 아이템, 착용 장비, 길드, 퀘스트, 캐시 등 DB 데이터를 포함합니다.\n게임 프로그램은 포함하지 않습니다. 새 버전의 서버에 DB만 복원할 때 사용하는 파일입니다.\n다음 업데이트 때 이 ZIP을 함께 보관하거나 전달해 주세요. ZIP 파일만 게임 폴더에 넣는 것으로 자동 복원되지는 않습니다.\n추출은 기존 캐릭터를 삭제하거나 초기화하지 않습니다.\n')
                        with zipfile.ZipFile(archive) as z:
                            assert z.testzip() is None
                            assert hashlib.sha256(z.read('characters.sql')).hexdigest()==meta['sql_sha256']
                        archive.rename(destination)
                finally:
                    with c.open_sftp() as s:
                        try:s.remove(remote)
                        except FileNotFoundError:pass
                if not started and previous:
                    cmd('systemctl start '+' '.join(previous));quiesced=False
            return meta
        finally:
            if started and control.online():
                notify('추출을 마쳤어요. 서버를 정리하고 있어요…');control.stop()
            elif quiesced and previous:
                with connect() as c:control.command(c,'systemctl start '+' '.join(previous))

def main():
    import tkinter as tk
    from tkinter import ttk,filedialog,messagebox
    root=tk.Tk();root.title('라키아 캐릭터 데이터 추출');root.geometry('510x240');root.resizable(False,False)
    text=tk.StringVar(value='업데이트 전에 계정·캐릭터·아이템·장비를 ZIP으로 저장합니다.\n게임과 저장 창을 먼저 종료해 주세요.\n서버를 여는 아버지 PC에서 실행하세요.')
    ttk.Label(root,textvariable=text,wraplength=465,justify='center').pack(padx=20,pady=24,fill='x')
    events=queue.Queue();busy=False
    def worker(path):
        try:export_characters(path,events.put);events.put(('done',path))
        except BaseException as error:traceback.print_exc();events.put(('error',str(error)))
    def begin():
        nonlocal busy
        desktop=Path.home()/'Desktop'
        folder=desktop if desktop.exists() else Path.home()
        filename='라키아 캐릭터 백업 '+datetime.datetime.now().strftime('%Y-%m-%d %H%M%S')+'.zip'
        path=filedialog.asksaveasfilename(parent=root,title='게임 폴더 밖에 백업 저장',initialdir=folder,initialfile=filename,defaultextension='.zip',filetypes=[('캐릭터 백업 ZIP','*.zip')])
        if not path:return
        busy=True;button.config(state='disabled');threading.Thread(target=worker,args=(path,),daemon=False).start()
    button=ttk.Button(root,text='캐릭터 데이터 추출',command=begin);button.pack(pady=5)
    ttk.Button(root,text='닫기',command=lambda:root.iconify() if busy else root.destroy()).pack(pady=5)
    root.protocol('WM_DELETE_WINDOW',lambda:root.iconify() if busy else root.destroy())
    def poll():
        nonlocal busy
        try:
            while True:
                value=events.get_nowait()
                if isinstance(value,tuple):
                    busy=False;button.config(state='normal');root.deiconify()
                    if value[0]=='done':
                        text.set('추출 완료! 이 ZIP을 다음 업데이트 때 보관해 주세요.\n'+value[1])
                        messagebox.showinfo('추출 완료','백업 ZIP을 만들었습니다.\n기존 캐릭터는 그대로 유지됩니다.\n\n'+value[1],parent=root)
                    else:messagebox.showerror('추출 확인',value[1],parent=root);text.set('추출하지 못했습니다. 안내 내용을 확인하고 다시 실행해 주세요.')
                else:text.set(value)
        except queue.Empty:pass
        root.after(100,poll)
    root.after(100,poll);root.mainloop()

if __name__=='__main__':
    log=(ROOT/'character-export.log').open('a',encoding='utf-8',buffering=1);sys.stdout=sys.stderr=log
    main()
