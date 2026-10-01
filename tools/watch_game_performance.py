"""Read-only, bounded diagnostics for the existing DeicideOnline process.

Records CPU, private/working memory, handle and I/O counts once per second.
Writes a normal stack minidump after 30 seconds and when Windows marks the
game window hung. Does not start, stop, inject into, or change the game.
An isolated desktop may expose counters but deny window/module/dump access;
the access_limited event explicitly reports that reduced diagnostic coverage.
"""
from pathlib import Path
from datetime import datetime
import argparse
import ctypes as C
from ctypes import wintypes as W
import json
import msvcrt
import os
import time

R = Path(__file__).resolve().parents[1]
K = C.WinDLL('kernel32', use_last_error=True)
U = C.WinDLL('user32', use_last_error=True)
P = C.WinDLL('psapi', use_last_error=True)
S = C.c_size_t
Q = C.c_ulonglong

class ProcessEntry(C.Structure):
    _fields_ = [('size', W.DWORD), ('usage', W.DWORD), ('pid', W.DWORD),
                ('heap', S), ('module', W.DWORD), ('threads', W.DWORD),
                ('parent', W.DWORD), ('priority', W.LONG), ('flags', W.DWORD),
                ('exe', W.WCHAR*260)]

class ModuleEntry(C.Structure):
    _fields_ = [('size', W.DWORD), ('id', W.DWORD), ('pid', W.DWORD),
                ('global_usage', W.DWORD), ('process_usage', W.DWORD),
                ('base', C.c_void_p), ('bytes', W.DWORD), ('module', W.HMODULE),
                ('name', W.WCHAR*256), ('path', W.WCHAR*260)]

class Memory(C.Structure):
    _fields_ = [('cb', W.DWORD), ('faults', W.DWORD)] + [
        (n, S) for n in ('peak_working', 'working', 'peak_paged', 'paged',
                        'peak_nonpaged', 'nonpaged', 'pagefile', 'peak_pagefile', 'private')]

class IO(C.Structure):
    _fields_ = [(n, Q) for n in ('read_ops','write_ops','other_ops','read_bytes','write_bytes','other_bytes')]

def api(lib, name, result, *args):
    f = getattr(lib, name); f.restype = result; f.argtypes = args; return f

snapshot = api(K, 'CreateToolhelp32Snapshot', W.HANDLE, W.DWORD, W.DWORD)
close = api(K, 'CloseHandle', W.BOOL, W.HANDLE)
first = api(K, 'Process32FirstW', W.BOOL, W.HANDLE, C.POINTER(ProcessEntry))
next_process = api(K, 'Process32NextW', W.BOOL, W.HANDLE, C.POINTER(ProcessEntry))
module_first = api(K, 'Module32FirstW', W.BOOL, W.HANDLE, C.POINTER(ModuleEntry))
module_next = api(K, 'Module32NextW', W.BOOL, W.HANDLE, C.POINTER(ModuleEntry))
open_process = api(K, 'OpenProcess', W.HANDLE, W.DWORD, W.BOOL, W.DWORD)
memory_info = api(P, 'GetProcessMemoryInfo', W.BOOL, W.HANDLE, C.POINTER(Memory), W.DWORD)
process_times = api(K, 'GetProcessTimes', W.BOOL, W.HANDLE, C.POINTER(W.FILETIME), C.POINTER(W.FILETIME), C.POINTER(W.FILETIME), C.POINTER(W.FILETIME))
handle_count = api(K, 'GetProcessHandleCount', W.BOOL, W.HANDLE, C.POINTER(W.DWORD))
io_counters = api(K, 'GetProcessIoCounters', W.BOOL, W.HANDLE, C.POINTER(IO))
exit_code = api(K, 'GetExitCodeProcess', W.BOOL, W.HANDLE, C.POINTER(W.DWORD))
window_pid = api(U, 'GetWindowThreadProcessId', W.DWORD, W.HWND, C.POINTER(W.DWORD))
is_visible = api(U, 'IsWindowVisible', W.BOOL, W.HWND)
is_hung = api(U, 'IsHungAppWindow', W.BOOL, W.HWND)
CALLBACK = C.WINFUNCTYPE(W.BOOL, W.HWND, W.LPARAM)
enum_windows = api(U, 'EnumWindows', W.BOOL, CALLBACK, W.LPARAM)

def processes():
    h = snapshot(2,0)
    if h == C.c_void_p(-1).value: raise C.WinError(C.get_last_error())
    try:
        e = ProcessEntry(); e.size = C.sizeof(e); ok = first(h,C.byref(e))
        while ok:
            if e.exe.lower() == 'deicideonline.exe': yield e.pid
            ok = next_process(h,C.byref(e))
    finally: close(h)

def modules(pid):
    h = snapshot(0x18,pid)
    if h == C.c_void_p(-1).value: return []
    try:
        e = ModuleEntry(); e.size = C.sizeof(e); result = []
        ok = module_first(h,C.byref(e))
        while ok:
            result.append(dict(name=e.name,path=e.path,base=hex(e.base or 0),bytes=e.bytes))
            ok = module_next(h,C.byref(e))
        return result
    finally: close(h)

def windows(pid):
    result = []
    @CALLBACK
    def visit(hwnd, _):
        owner = W.DWORD(); window_pid(hwnd,C.byref(owner))
        if owner.value == pid and is_visible(hwnd):
            result.append(dict(hwnd=hex(hwnd),hung=bool(is_hung(hwnd))))
        return True
    enum_windows(visit,0)
    return result

def read_sample(handle,pid):
    m = Memory(); m.cb = C.sizeof(m)
    if not memory_info(handle,C.byref(m),m.cb): raise C.WinError(C.get_last_error())
    times = [W.FILETIME() for _ in range(4)]
    if not process_times(handle,*[C.byref(t) for t in times]): raise C.WinError(C.get_last_error())
    ticks = lambda t: (t.dwHighDateTime<<32)|t.dwLowDateTime
    count = W.DWORD(); handle_count(handle,C.byref(count))
    io = IO(); io_counters(handle,C.byref(io))
    return dict(private_mib=round(m.private/1048576,2),working_mib=round(m.working/1048576,2),
                peak_private_mib=round(m.peak_pagefile/1048576,2),page_faults=m.faults,
                cpu_seconds=(ticks(times[2])+ticks(times[3]))/1e7,handles=count.value,
                io_read_bytes=io.read_bytes,io_write_bytes=io.write_bytes,windows=windows(pid))

def dump(handle,pid,path):
    dbg = C.WinDLL('dbghelp',use_last_error=True)
    write = api(dbg,'MiniDumpWriteDump',W.BOOL,W.HANDLE,W.DWORD,W.HANDLE,W.DWORD,
                C.c_void_p,C.c_void_p,C.c_void_p)
    # Thread contexts/stacks and memory map, without a full process-memory dump.
    with path.open('xb') as f:
        ok = write(handle,pid,msvcrt.get_osfhandle(f.fileno()),0x1820,None,None,None)
        error = C.get_last_error() if not ok else 0
    return dict(path=str(path),success=bool(ok),error=error,bytes=path.stat().st_size)

def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--seconds',type=int,default=900)
    parser.add_argument('--self-test',action='store_true')
    args=parser.parse_args()
    assert 1 <= args.seconds <= 3600
    folder=R/'runtime/diagnostics'/datetime.now().strftime('lag-%Y%m%d-%H%M%S')
    folder.mkdir(parents=True,exist_ok=False)
    if args.self_test:
        pid=os.getpid(); h=open_process(0x410,False,pid)
        try:
            sample=read_sample(h,pid); assert sample['private_mib'] > 0
            assert any(m['name'].lower()=='python.exe' for m in modules(pid))
            print(json.dumps(dict(passed=True,sample=sample)))
        finally: close(h)
        return
    (R/'runtime/diagnostics/latest-watch.json').write_text(json.dumps(dict(folder=str(folder),watcher_pid=os.getpid(),seconds=args.seconds)),encoding='utf-8')
    handles={}; previous={}; attached={}; dump_count={}; last_dump={}; hung_count={}
    started=time.monotonic()
    with (folder/'samples.jsonl').open('a',encoding='utf-8',buffering=1) as log:
        def event(kind,**fields):
            log.write(json.dumps(dict(time=datetime.now().isoformat(),event=kind,**fields),ensure_ascii=False)+'\n')
        event('watch_started',watcher_pid=os.getpid(),seconds=args.seconds)
        print(str(folder),flush=True)
        try:
            while time.monotonic()-started < args.seconds:
                now=time.monotonic()
                for pid in processes():
                    if pid not in handles:
                        h=open_process(0x410,False,pid)
                        if not h:
                            event('open_failed',pid=pid,error=C.get_last_error());continue
                        handles[pid]=h;attached[pid]=now;dump_count[pid]=0;last_dump[pid]=0;hung_count[pid]=0
                        loaded_modules=modules(pid)
                        event('attached',pid=pid,modules=loaded_modules)
                        if not loaded_modules:
                            event('access_limited',pid=pid,detail='Counters available; module access failed. Window hang detection and dumps may be unavailable from this desktop/token.')
                for pid,h in list(handles.items()):
                    code=W.DWORD();exit_code(h,C.byref(code))
                    if code.value != 259:
                        event('exited',pid=pid,code=hex(code.value));close(h);del handles[pid];continue
                    try: sample=read_sample(h,pid)
                    except OSError as error:
                        event('sample_failed',pid=pid,error=str(error));continue
                    if pid in previous:
                        then,cpu=previous[pid];sample['cpu_one_core_percent']=round(100*(sample['cpu_seconds']-cpu)/(now-then),2)
                    previous[pid]=(now,sample['cpu_seconds'])
                    event('sample',pid=pid,**sample)
                    hung_count[pid]=hung_count[pid]+1 if any(w['hung'] for w in sample['windows']) else 0
                    baseline=dump_count[pid]==0 and now-attached[pid]>=30
                    stalled=hung_count[pid]>=2 and now-last_dump[pid]>=12
                    if dump_count[pid]<4 and (baseline or stalled):
                        reason='hung' if stalled else 'baseline'
                        result=dump(h,pid,folder/f'{pid}-{dump_count[pid]}-{reason}.dmp')
                        dump_count[pid]+=1;last_dump[pid]=time.monotonic()
                        event('dump',pid=pid,reason=reason,**result)
                time.sleep(1)
        finally:
            for h in handles.values():close(h)
            event('watch_finished')

if __name__=='__main__':main()
