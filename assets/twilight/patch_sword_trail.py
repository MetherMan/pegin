"""Preserve Twilight's selected trail when character initialization creates it.

DeicideOnline.exe selects an equipped weapon in 0x40dd50, called at 0x40a981.
Weapon loading calls TraceEffect::SetType at 0x40bb5a, before the character's
trace exists. Render.dll::SetType (0x100288e0) returns false in that case.
The later Create at 0x40ab30 used the hard-coded white '검흔', discarding the
selection. Use twilight_blue at Create when the equipped trace entry says so.
Other weapons and the normal attack start/stop events are unchanged.
"""
from pathlib import Path
import struct,json,hashlib,sys
O=Path(__file__).resolve().parent;ROOT=O.parents[1]
sys.path.insert(0,str(ROOT/'runtime/pylibs'))
import pefile
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32
from unicorn.x86_const import *

HOOK=0xab29;CAVE=0x69020;RESUME=0xab2e;DEFAULT=0x70964
def jump(src,dst):return b'\xe9'+struct.pack('<i',dst-src-5)
code=bytearray(b'\x9c\x8b\x8d\x0c\x03\x00\x00\x85\xc9')
branches=[]
def fallback_if(op):
    code.extend(op);branches.append(len(code));code.append(0)
fallback_if(b'\x74')
code+=b'\x8b\x49\x18\x85\xc9';fallback_if(b'\x74')
for off,word in [(0,b'twil'),(4,b'ight'),(8,b'_blu')]:
    code+=(b'\x81\x39' if off==0 else b'\x81\x79'+bytes([off]))+word
    fallback_if(b'\x75')
code+=b'\x66\x81\x79\x0c\x65\x00';fallback_if(b'\x75')
code+=b'\xeb\x00';success_branch=len(code)-1
fallback=len(code)
code+=b'\xe8\x00\x00\x00\x00\x59' # PIC default string address
pc=CAVE+fallback+5
code+=b'\x81\xc1'+struct.pack('<i',DEFAULT-pc)
selected=len(code)
code+=b'\x9d\x51' # restore flags and push selected name
code+=jump(CAVE+len(code),RESUME)
for pos in branches:code[pos]=fallback-pos-1
code[success_branch]=selected-success_branch-1

source=ROOT/'client-overlay/DeicideOnline.exe'
before=source.read_bytes();b=bytearray(before);pe=pefile.PE(data=before)
assert pe.OPTIONAL_HEADER.ImageBase==0x400000
assert not hasattr(pe,'DIRECTORY_ENTRY_BASERELOC')
hookoff=pe.get_offset_from_rva(HOOK);caveoff=pe.get_offset_from_rva(CAVE)
original=b'\x68'+struct.pack('<I',0x400000+DEFAULT)
patched_jump=jump(HOOK,CAVE)
if b[hookoff:hookoff+5]==original:
    assert b[caveoff:caveoff+len(code)]==bytes(len(code))
    b[hookoff:hookoff+5]=patched_jump;b[caveoff:caveoff+len(code)]=code
elif b[hookoff:hookoff+5]==patched_jump:
    assert b[caveoff:caveoff+len(code)]==code
else:raise AssertionError('Unrecognized client hook: do not overwrite other changes')
section=pe.sections[0]
struct.pack_into('<I',b,section.get_file_offset()+8,section.SizeOfRawData)

# Execute the real patched x86 instructions through the existing MOV ECX,EAX.
# Test null/stale/nonmatching entries, the full exact key, and register/stack
# invariants at the original virtual call boundary. The executable is fixed-base.
def run(data,name,has_record=True):
    image=pefile.PE(data=bytes(data));u=Uc(UC_ARCH_X86,UC_MODE_32)
    u.mem_map(0x400000,0x100000);u.mem_write(0x400000,image.get_memory_mapped_image())
    u.mem_map(0x2000000,0x10000)
    char,record,string,stack=0x2000100,0x2001000,0x2002000,0x200f000
    u.mem_write(char+0x30c,struct.pack('<I',record if has_record else 0))
    u.mem_write(record+0x18,struct.pack('<I',string if name is not None else 0))
    if name is not None:u.mem_write(string,name+b'\x00')
    u.mem_write(stack,struct.pack('<I',12345)) # actor ID already pushed
    for reg,value in [(UC_X86_REG_EBP,char),(UC_X86_REG_ESP,stack),(UC_X86_REG_EAX,0x76543210),(UC_X86_REG_EDX,0x12345678),(UC_X86_REG_ECX,12345),(UC_X86_REG_EFLAGS,0x246)]:u.reg_write(reg,value)
    u.emu_start(0x400000+HOOK,0x40ab30,count=120)
    address=struct.unpack('<I',u.mem_read(stack-4,4))[0]
    result=bytes(u.mem_read(address,32)).split(b'\x00')[0]
    assert u.reg_read(UC_X86_REG_ESP)==stack-4
    assert struct.unpack('<I',u.mem_read(stack,4))[0]==12345
    assert u.reg_read(UC_X86_REG_EAX)==u.reg_read(UC_X86_REG_ECX)==0x76543210
    assert u.reg_read(UC_X86_REG_EDX)==0x12345678
    assert u.reg_read(UC_X86_REG_EFLAGS)==0x246
    return result

unpatched=bytearray(b);unpatched[hookoff:hookoff+5]=original
white='검흔'.encode('cp949')
assert run(unpatched,b'twilight_blue')==white # reproduces the original failure
cases=[(None,False),(None,True),(b'other',True),(white,True),(b'twilight_blue_extra',True),(b'twilight_blu',True),(b'twilight_blue',True)]
for name,has_record in cases:
    assert run(b,name,has_record)==(b'twilight_blue' if has_record and name==b'twilight_blue' else white)
# Verify the pre-existing re-equip call still selects the stored trail name.
assert b[pe.get_offset_from_rva(0xbb3c):pe.get_offset_from_rva(0xbb5d)]==before[pe.get_offset_from_rva(0xbb3c):pe.get_offset_from_rva(0xbb5d)]
allowed=set(range(hookoff,hookoff+5))|set(range(caveoff,caveoff+len(code)))|set(range(section.get_file_offset()+8,section.get_file_offset()+12))
assert all(i in allowed for i,(old,new) in enumerate(zip(before,b)) if old!=new)
P=O/'payload';P.mkdir(exist_ok=True);(P/'DeicideOnline.exe').write_bytes(b)
report=dict(passed=True,original_failure_reproduced=True,creation_cases=len(cases),initial_equipped_twilight='twilight_blue',other_weapons='unchanged default',re_equip_selection_unchanged=True,stack_and_flags_preserved=True,attack_event_timing_unchanged=True,visual_in_game_checked=False,sha256=hashlib.sha256(b).hexdigest())
(O/'trail_initialization_validation.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(report))
