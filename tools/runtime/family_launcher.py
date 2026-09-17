"""One session owns its game process. Character selection is not process exit."""
from pathlib import Path
import logging, os, queue, subprocess, sys, threading, time, traceback
import family_network as network
import local_control as control

ROOT=Path(__file__).resolve().parent

def run_session(mode,notify,peer=None):
    with network.SessionLock():
        if control.client_running(): raise RuntimeError('게임 창이 이미 실행 중입니다. 먼저 실행한 게임을 사용해 주세요.')
        hosting=None
        process=None
        local_ready=False
        try:
            if mode=='join':
                notify('아버지 서버에 연결하고 있어요…')
                host=network.resolve_peer(peer)
                network.probe(host)
            else:
                if mode=='host': network.ready()
                notify('서버를 켜고 있어요. 처음에는 1~2분 걸릴 수 있어요…')
                control.start();local_ready=True
                host='127.0.0.1'
                if mode=='host':
                    hosting=network.Hosting()
                    address=hosting.start()
                    notify('가족 접속 주소: '+address)
            notify('게임을 실행하고 있어요…')
            process=control.launch_client(host)
            # Never wait on a .cmd wrapper: this is the actual DeicideOnline process.
            time.sleep(3)
            if process.poll() is not None:
                raise RuntimeError('게임이 실행 직후 종료됐습니다. 압축이 모두 풀렸는지 확인해 주세요. 자세한 기록: family-launcher.log')
            notify('PLAYING')
            code=process.wait()
            if code!=0: raise RuntimeError(f'게임이 종료됐습니다 (코드 {code}). 게임 로그를 확인해 주세요.')
        finally:
            # A guest never has a local_ready flag and cannot stop either server.
            # Save the game even when disabling the private mapping fails.
            try:
                if hosting: hosting.close()
            finally:
                if local_ready:
                    notify('캐릭터를 저장하고 서버를 끄고 있어요…')
                    try:control.stop()
                    finally:
                        if process is not None:control.release_client_drive(process)
                elif process is not None:control.release_client_drive(process)

def main(mode):
    import tkinter as tk
    from tkinter import messagebox, simpledialog, ttk
    if mode in ('host','join'):
        try:network.ready()
        except Exception:
            # A father's first launch leads straight into the one-time setup.
            from family_setup import main as setup
            setup(continue_to_game=True)
    window=tk.Tk();window.title('라키아 가족 게임');window.geometry('460x170')
    window.resizable(False,False)
    label=tk.StringVar(value='게임을 준비하고 있어요…')
    ttk.Label(window,textvariable=label,wraplength=420,anchor='center').pack(pady=(35,16),fill='x')
    bar=ttk.Progressbar(window,mode='indeterminate');bar.pack(padx=40,fill='x');bar.start()
    # Closing the progress window must not kill the save/stop supervisor.
    window.protocol('WM_DELETE_WINDOW',window.iconify)
    peer=None
    if mode in ('host','join'):
        try: network.ready()
        except Exception as error:
            messagebox.showerror('가족 연결 설정 필요',str(error),parent=window)
            window.destroy();return
    if mode=='join':
        config=network.read_local('family-join.json')
        peer=config.get('peer')
        if not peer:
            peers=list((network.ready().get('Peer') or {}).values())
            examples='\n'.join(p.get('HostName','')+'  '+network.ipv4(p) for p in peers[:10])
            peer=simpledialog.askstring('아버지 PC 선택',
                 '처음 한 번만 아버지 PC의 이름 또는 100.x 주소를 입력해 주세요.\n'+examples,
                 initialvalue=network.ipv4(peers[0]) if len(peers)==1 else '',parent=window)
            if not peer: window.destroy();return
            try: network.resolve_peer(peer.strip())
            except Exception as error:
                messagebox.showerror('연결 확인',str(error),parent=window);window.destroy();return
            peer=peer.strip();network.write_local('family-join.json',{'peer':peer})
    events=queue.Queue()
    def worker():
        try:
            run_session(mode,events.put,peer)
            events.put(('done',None))
        except BaseException as error:
            logging.exception('Family game session failed')
            events.put(('error',str(error)))
    def poll():
        try:
            while True:
                event=events.get_nowait()
                if isinstance(event,tuple):
                    if event[0]=='error':
                        window.deiconify();messagebox.showerror('실행 확인',event[1],parent=window)
                    window.destroy();return
                if event=='PLAYING': window.withdraw()
                else:
                    label.set(event)
                    if '저장하고' in event: window.deiconify()
        except queue.Empty: pass
        window.after(100,poll)
    threading.Thread(target=worker,daemon=False).start()
    window.after(100,poll);window.mainloop()

if __name__=='__main__':
    log=(ROOT/'family-launcher.log').open('a',encoding='utf-8',buffering=1)
    sys.stdout=sys.stderr=log
    try: main(sys.argv[1] if len(sys.argv)>1 else 'local')
    except BaseException:
        traceback.print_exc()
        import ctypes
        ctypes.windll.user32.MessageBoxW(None,'실행기를 열지 못했습니다. family-launcher.log를 확인해 주세요.','라키아',16)
