"""Fault isolated fixture processes and inspect the actual Windows minidump streams."""
from pathlib import Path
import hashlib,json,struct,subprocess,time
ROOT=Path(__file__).resolve().parents[1]
folder=ROOT/'.cache/crash-reporter'
dumps=folder/'CrashDumps'
cases=[]
for mode,code in [('',0),('handled',0),('access',0xc0000005),('thread',0xc0000005),('stack',0xc00000fd)]:
    before=set(dumps.glob('*'))
    result=subprocess.run([str(folder/'crash_fixture.exe')]+([mode] if mode else []),cwd=folder,timeout=20)
    assert result.returncode & 0xffffffff==code,(mode,result.returncode)
    created=set(dumps.glob('*'))-before
    if not code:
        assert not created,(mode,created)
    else:
        assert len(created)==2,(mode,created)
        txt=next(p for p in created if p.suffix=='.txt').read_text(encoding='utf-8')
        assert f'exception=0x{code:08X}' in txt and 'minidump_written=1' in txt,txt
        data=next(p for p in created if p.suffix=='.dmp').read_bytes()
        assert data[:4]==b'MDMP'
        streams,directory=struct.unpack_from('<II',data,8)
        index={}
        for i in range(streams):
            kind,size,rva=struct.unpack_from('<III',data,directory+12*i)
            index[kind]=data[rva:rva+size]
        exception=index[6]
        threadid=struct.unpack_from('<I',exception)[0]
        assert struct.unpack_from('<I',exception,8)[0]==code
        assert f'thread={threadid}' in txt
        assert len(index[3])>4 and len(index[4])>4 # thread and module lists
    cases.append(dict(mode=mode or 'normal',exit_code=hex(code),files=len(created)))
report=dict(passed=True,cases=cases,sha256=hashlib.sha256((folder/'LaqiaCrash.dll').read_bytes()).hexdigest(),
    scope='Isolated native x86 fixtures, including real unhandled access violations and stack overflow. Not a reproduction of the reported boss crash.')
(ROOT/'tests/results/crash-reporter.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(report))
