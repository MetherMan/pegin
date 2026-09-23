"""Replace moving hell-horse dust with native short-lived embers; no new timers."""
from pathlib import Path
import hashlib,json,struct,sys
R=Path(__file__).resolve().parents[1]
sys.path[:0]=[str(R/'runtime/pylibs'),str(R/'.cache/weapon-build-deps'),str(R/'tools')]
from pe_hooks import Hooks
from patch_stack_client import machine
from unicorn.x86_const import *

def main():
    d=R/'client-overlay';o=R/'assets/hell-horse'
    raw=(R/'runtime/client/GameClient/Effect/particle.ptc').read_bytes()
    header=30;stride=761;count=struct.unpack_from('<i',raw,26)[0]
    assert raw[:22]==b'#W3DParticleInfo File\0'[:22] and len(raw)==header+count*stride
    rows=[raw[header+i*stride:header+(i+1)*stride] for i in range(count)]
    source=next(r for r in rows if r[:125].split(b'\0')[0].decode('cp949')=='불')
    ember=bytearray(source);ember[:125]=b'mt_hell_hoof'.ljust(125,b'\0')
    # 2 particles per movement update, 550ms lifetime. The native integration
    # leaves emitted particles in world space, trailing behind the horse.
    struct.pack_into('<4i5f2i',ember,125,100,2,33,550,.032,0,0,.00012,.00001,0,12)
    for i in range(12):
        fade=(1-i/11)**1.3
        struct.pack_into('<7f',ember,425+i*28,255*fade,145*fade,28*fade,255*fade,65*fade,8*fade,.075*(1-i/14))
    data=bytearray(raw);struct.pack_into('<i',data,26,count+1);data.extend(ember)
    (d/'Effect/particle.ptc').write_bytes(data)
    h=Hooks(d/'DeicideOnline.exe',o/'spark-hook.json',b'.hfire')
    name=h.base+h.rva;h.code.extend(b'mt_hell_hoof\0')
    # Call is inside CVehicle::Update STATE_MOVE. Nonmoving states never enter
    # this block. Preserve the world virtual call, position and stack cleanup.
    h.hook(0x4223b9,'ff9290000000',
        'push eax; call here; here: pop eax; sub eax,22; '
        'cmp dword ptr [ebx+0x24],4; jne original; mov dword ptr [esp+4],eax; '
        'original: pop eax; call dword ptr [edx+0x90]', 'hell horse movement embers')
    output,report=h.finish();cases=[]
    for base,kind in [(base,kind) for base in (0x400000,0x600000) for kind in (0,1,2,3,4,5,0xffffffff)]:
        delta=base-h.base;u,_=machine(output,base);obj=0x2001000;vt=0x2003000;stub=0x2004000;sp=0x201f000
        u.mem_write(obj+0x24,struct.pack('<I',kind));u.mem_write(vt+0x90,struct.pack('<I',stub))
        # Record the actual two arguments and return using the native ABI.
        u.mem_write(stub,h.asm('mov eax,[esp+4]; mov [0x2005000],eax; mov eax,[esp+8]; mov [0x2005004],eax; mov eax,1; ret 8',stub))
        u.mem_write(sp,struct.pack('<2I',0x4725b8+delta,obj+0x44))
        u.reg_write(UC_X86_REG_EBX,obj);u.reg_write(UC_X86_REG_EDX,vt);u.reg_write(UC_X86_REG_ECX,0x2007000)
        u.emu_start(0x4223b9+delta,0x4223bf+delta,count=80)
        got,pos=struct.unpack('<2I',u.mem_read(0x2005000,8))
        assert got==((name if kind==4 else 0x4725b8)+delta) and pos==obj+0x44
        assert u.reg_read(UC_X86_REG_ESP)==sp+8 and u.reg_read(UC_X86_REG_EBX)==obj
        cases.append(dict(base=base,mount=kind,hell_sparks=kind==4))
    # Original state gate remains byte-for-byte intact.
    pe=h.pe;off=pe.get_offset_from_rva(0x4223a3-h.base)
    assert output[off:off+11]==h.original[off:off+11]
    report['validation']=dict(cases=cases,movement_gate_unchanged=True,original_particle_records_unchanged=True,particle_lifetime_ms=550,particles_per_update=2,in_game_visual_test=False)
    assert bytes(data[30:len(raw)])==raw[30:]
    (o/'spark-hook.json').write_text(json.dumps(report,indent=2)+'\n')
    (d/'DeicideOnline.exe').write_bytes(output)
    print(json.dumps(report['validation']))

if __name__=='__main__':main()
