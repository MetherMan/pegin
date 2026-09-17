"""One-time installation/authentication remains an interactive user operation."""
from pathlib import Path
import ctypes, json, os, queue, re, subprocess, sys, threading, time
import urllib.request, urllib.parse, webbrowser
import family_network as network

ROOT=Path(__file__).resolve().parent
INSTALLER='https://dl.tailscale.com/stable/tailscale-setup-1.102.4-amd64.msi'

def download():
    folder=ROOT/'installers';folder.mkdir(exist_ok=True)
    target=folder/INSTALLER.rsplit('/',1)[-1]
    if not target.exists():
        temp=target.with_suffix('.download')
        with urllib.request.urlopen(INSTALLER,timeout=30) as response, temp.open('wb') as out:
            while chunk:=response.read(1024*1024): out.write(chunk)
        temp.replace(target)
    powershell=Path(os.environ.get('SystemRoot',r'C:\Windows'))/'System32/WindowsPowerShell/v1.0/powershell.exe'
    # Windows os.environ normalizes keys to uppercase; the copied dict does not.
    environment={k:v for k,v in os.environ.items() if k.lower()!='psmodulepath'}
    p=subprocess.run([str(powershell),'-NoProfile','-NonInteractive','-ExecutionPolicy','Bypass',
                      '-File',str(ROOT/'verify_tailscale.ps1'),str(target)],capture_output=True,
                     timeout=60,creationflags=subprocess.CREATE_NO_WINDOW,env=environment)
    if p.returncode: raise RuntimeError('공식 설치 파일의 서명을 확인할 수 없습니다. 설치하지 않았습니다.')
    return target

def install(notify):
    try: return network.executable()
    except RuntimeError: pass
    notify('공식 Tailscale 설치 파일을 내려받고 확인하고 있어요…')
    target=download()
    notify('Windows 설치 허용 창을 확인해 주세요…')
    shell=ctypes.WinDLL('shell32',use_last_error=True)
    shell.ShellExecuteW.argtypes=[ctypes.c_void_p,ctypes.c_wchar_p,ctypes.c_wchar_p,ctypes.c_wchar_p,ctypes.c_wchar_p,ctypes.c_int]
    shell.ShellExecuteW.restype=ctypes.c_void_p
    result=shell.ShellExecuteW(None,'runas','msiexec.exe',f'/i "{target}" /passive /norestart',str(ROOT),0)
    if not result or result<=32: raise RuntimeError('설치가 취소됐습니다. 필요할 때 가족 연결 설정을 다시 실행해 주세요.')
    for _ in range(180):
        try:
            path=network.executable();network.cli('status','--json');return path
        except Exception: time.sleep(1)
    raise RuntimeError('설치를 마치지 못했습니다. 설치 창을 확인한 뒤 다시 눌러 주세요.')

def connect_account(notify):
    path=install(notify)
    try: return network.ready()
    except Exception: pass
    notify('브라우저에서 로그인해 주세요. 두 PC를 같은 가족용 계정으로 연결하면 됩니다.')
    p=subprocess.Popen([path,'up','--timeout=180s'],stdout=subprocess.PIPE,stderr=subprocess.STDOUT,
                       text=True,encoding='utf-8',errors='replace',creationflags=subprocess.CREATE_NO_WINDOW)
    try:
        for line in p.stdout:
            for url in re.findall(r'https://[^\s]+',line):
                if urllib.parse.urlsplit(url).hostname=='login.tailscale.com':
                    webbrowser.open(url)
        if p.wait(): raise RuntimeError('아직 로그인이 완료되지 않았습니다. 연결 버튼을 다시 눌러 주세요.')
    finally:
        if p.poll() is None: p.terminate();p.wait()
    return network.ready()

def main(continue_to_game=False):
    import tkinter as tk
    from tkinter import ttk, messagebox
    window=tk.Tk();window.title('라키아 가족 연결 · 처음 한 번');window.geometry('520x330')
    text=tk.StringVar(value='서로 다른 집의 PC를 가족 전용 연결로 묶습니다.\n두 PC 모두 설치하고 같은 Tailscale 계정으로 로그인해 주세요.\n게임 계정은 아버지와 서로 다르게 사용해야 합니다.')
    ttk.Label(window,textvariable=text,wraplength=475,justify='center').pack(padx=20,pady=30,fill='x')
    events=queue.Queue();busy=False
    def work():
        try:
            data=connect_account(events.put)
            events.put(('done','연결됐어요. 이 PC 주소: '+network.ipv4(data['Self'])+'\n\n아버지: 가족 게임 시작\n접속할 사람: 아버지 서버 접속'))
        except Exception as error: events.put(('error',str(error)))
    def begin():
        nonlocal busy
        if busy:return
        busy=True;button.config(state='disabled');threading.Thread(target=work,daemon=False).start()
    button=ttk.Button(window,text='설치하고 연결',command=begin);button.pack(pady=5)
    def reset():
        network.write_local('family-join.json',{})
        messagebox.showinfo('접속할 PC 변경','다음에 아버지 서버 접속을 누르면 PC를 다시 선택할 수 있어요.',parent=window)
    ttk.Button(window,text='접속할 아버지 PC 다시 선택',command=reset).pack(pady=5)
    close_button=ttk.Button(window,text='닫기',command=lambda:window.iconify() if busy else window.destroy())
    close_button.pack(pady=5)
    window.protocol('WM_DELETE_WINDOW',lambda:window.iconify() if busy else window.destroy())
    def poll():
        nonlocal busy
        try:
            while True:
                value=events.get_nowait()
                if isinstance(value,tuple):
                    busy=False;button.config(state='normal');window.deiconify();text.set(value[1])
                    if value[0]=='done' and continue_to_game:close_button.config(text='게임 계속하기')
                else:text.set(value)
        except queue.Empty:pass
        window.after(150,poll)
    window.after(150,poll);window.mainloop()

if __name__=='__main__': main()
