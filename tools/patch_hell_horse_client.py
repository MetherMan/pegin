"""Guarded hell-horse model/motion hooks and space-preserving map names."""
from pathlib import Path
import json, struct, sys
ROOT=Path(__file__).resolve().parents[1]
sys.path[:0]=[str(ROOT/'runtime/pylibs'),str(ROOT/'.cache/weapon-build-deps'),str(ROOT/'tools')]
from pe_hooks import Hooks
from patch_stack_client import machine
from unicorn.x86_const import *

def build():
    out=ROOT/'assets/hell-horse';out.mkdir(exist_ok=True)
    path=ROOT/'client-overlay/DeicideOnline.exe'
    h=Hooks(path,out/'client-hooks.json',b'.hell')
    model=h.base+h.rva+len(h.code);h.code.extend(b'mt_hellhorse\0')
    scan=h.base+h.rva+len(h.code);h.code.extend(b' %63[^\r\n]\0')
    # Original table has only three entries; safely handle type 4 and invalid types.
    h.hook(0x422220,'568b742408',
        f'mov eax,[esp+4]; cmp eax,4; je hell; cmp eax,1; jb fallback; cmp eax,3; ja fallback; '
        'dec eax; shl eax,8; add eax,0x472194; ret 4; '
        f'hell: mov eax,{model}; ret 4; fallback: mov eax,0x472594; ret 4',
        'four mount models, bounds checked')
    h.replace(0x40878b,'83f803',bytes.fromhex('83f804'),'hell horse uses original horse animation mode')
    # Rider mode is MODE_VEHICLE(2)+type; use original horse rider mode for type 4.
    h.hook(0x4054e8,'8b7c240c8906',
        'mov edi,[esp+0xc]; cmp di,6; jne original; mov edi,3; original: mov [esi],eax',
        'hell-horse rider uses original horse posture')
    h.replace(0x42c6e0,'6878054700',b'\x68'+struct.pack('<I',scan),'full map name including spaces')
    data,report=h.finish();tests=[]
    for kind,want in [(0,'horse_1'),(1,'horse_1'),(2,'horse_2'),(3,'horse_3'),(4,'mt_hellhorse'),(5,'horse_1'),(0xffffffff,'horse_1')]:
        u,_=machine(data);sp=0x201f000;stop=0x201d000
        u.mem_write(sp,struct.pack('<II',stop,kind));u.emu_start(0x422220,stop,count=500)
        got=bytes(u.mem_read(u.reg_read(UC_X86_REG_EAX),32)).split(b'\0')[0].decode()
        assert got==want,(kind,got,want)
        assert u.reg_read(UC_X86_REG_ESP)==sp+8
        tests.append(dict(kind=kind,model=got))
    for kind in range(6):
        u,_=machine(data);u.mem_write(0x201f000,struct.pack('<II',0x201d000,kind));u.emu_start(0x408780,0x201d000,count=100)
        assert u.reg_read(UC_X86_REG_EAX)&0xffff==1
    for mode in range(7):
        u,_=machine(data);u.mem_write(0x201f00c,struct.pack('<I',mode));u.reg_write(UC_X86_REG_ESI,0x2001000);u.reg_write(UC_X86_REG_EAX,0x1234)
        u.emu_start(0x4054e8,0x4054ee,count=100)
        assert u.reg_read(UC_X86_REG_EDI)==(3 if mode==6 else mode)
        assert bytes(u.mem_read(0x2001000,4))==struct.pack('<I',0x1234)
    # The emitted format must consume spaces, stop at CR/LF, and bound the buffer.
    assert bytes(u.mem_read(scan,12)).startswith(b' %63[^\r\n]\0')
    report['validation']=dict(model_cases=tests,horse_animation_modes=6,rider_modes=7)
    staged=ROOT/'.cache/hell-horse-client';staged.mkdir(exist_ok=True)
    (staged/path.name).write_bytes(data)
    (out/'client-hooks.json').write_text(json.dumps(report,indent=2)+'\n')
    path.write_bytes(data)
    print(json.dumps(report['validation']))

if __name__=='__main__':build()
