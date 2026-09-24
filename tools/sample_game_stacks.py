"""Bounded sampling profiler for the 32-bit game; no code or data injection.

Briefly pauses the main thread only while reading its context and 2 KiB stack,
then resumes it in a finally block. Records raw addresses, not guessed frames.
Run outside an isolated desktop to access the game's window and thread.
"""
import argparse
import ctypes as C
from ctypes import wintypes as W
from datetime import datetime
import json
from pathlib import Path
import struct
import time

import watch_game_performance as w


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--pid', type=int, required=True)
    parser.add_argument('--seconds', type=int, default=60)
    args = parser.parse_args()
    if not 1 <= args.seconds <= 120:
        parser.error('seconds must be between 1 and 120')
    windows = w.windows(args.pid)
    if not windows:
        raise RuntimeError('Game window unavailable; no thread was suspended.')
    owner = W.DWORD()
    tid = w.window_pid(int(windows[0]['hwnd'], 16), C.byref(owner))
    open_thread = w.api(w.K, 'OpenThread', W.HANDLE, W.DWORD, W.BOOL, W.DWORD)
    suspend = w.api(w.K, 'SuspendThread', W.DWORD, W.HANDLE)
    resume = w.api(w.K, 'ResumeThread', W.DWORD, W.HANDLE)
    context = w.api(w.K, 'Wow64GetThreadContext', W.BOOL, W.HANDLE, C.c_void_p)
    read = w.api(w.K, 'ReadProcessMemory', W.BOOL, W.HANDLE, C.c_void_p,
                 C.c_void_p, C.c_size_t, C.POINTER(C.c_size_t))
    thread = open_thread(0x004A, False, tid)
    process = w.open_process(0x410, False, args.pid)
    if not thread or not process:
        error = C.get_last_error()
        if thread: w.close(thread)
        if process: w.close(process)
        raise C.WinError(error)
    folder = w.R / 'runtime/diagnostics'
    folder.mkdir(exist_ok=True)
    output = folder / datetime.now().strftime('stacks-%Y%m%d-%H%M%S.jsonl')
    modules = w.modules(args.pid)
    try:
        with output.open('x', encoding='utf-8', buffering=1) as log:
            log.write(json.dumps(dict(event='start', pid=args.pid, tid=tid,
                                      modules=modules, seconds=args.seconds))+'\n')
            print(str(output), flush=True)
            deadline = time.monotonic()+args.seconds
            while time.monotonic() < deadline:
                ctx = C.create_string_buffer(716)
                struct.pack_into('<I', ctx, 0, 0x10007)
                stack = C.create_string_buffer(2048)
                count = C.c_size_t()
                before = time.perf_counter()
                if suspend(thread) == 0xFFFFFFFF:
                    log.write(json.dumps(dict(event='thread_unavailable',error=C.get_last_error()))+'\n')
                    break
                try:
                    if not context(thread, ctx):
                        raise C.WinError(C.get_last_error())
                    ebp, eip, esp = (struct.unpack_from('<I',ctx,k)[0] for k in (180,184,196))
                    read(process,esp,stack,len(stack),C.byref(count))
                finally:
                    if resume(thread) == 0xFFFFFFFF:
                        raise C.WinError(C.get_last_error())
                pause_ms = (time.perf_counter()-before)*1000
                data = stack.raw[:count.value]
                words = list(struct.unpack('<'+'I'*(len(data)//4),data[:len(data)//4*4]))
                log.write(json.dumps(dict(event='sample',time=datetime.now().isoformat(),
                    eip=eip,esp=esp,ebp=ebp,pause_ms=round(pause_ms,3),stack=words))+'\n')
                time.sleep(.1)
    finally:
        w.close(thread)
        w.close(process)


if __name__ == '__main__':
    main()
