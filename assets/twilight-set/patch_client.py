"""Guarded additive hooks; preserve existing stack/crash/trail client patches."""
from mesh_tools import *
import pefile,hashlib
from keystone import Ks,KS_ARCH_X86,KS_MODE_32
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32
from unicorn.x86_const import *
O=Path(__file__).resolve().parent
def digest(b):return hashlib.sha256(b).hexdigest()
def align(n,a):return (n+a-1)//a*a

def patch_arrow():
    path=ROOT/'client-overlay/DeicideOnline.exe';report=O/'arrow-hook.json';before=path.read_bytes()
    if report.exists():
        saved=json.loads(report.read_text())
        if digest(before)==saved['sha256']:return test_arrow(before,saved)
        layer=ROOT/'assets/skills140/sixfold-hook.json'
        if layer.exists():
            top=json.loads(layer.read_text())
            if digest(before)==top['sha256'] and top['original_sha256']==saved['sha256']:
                return test_arrow(before,saved)
    pe=pefile.PE(data=before);assert pe.OPTIONAL_HEADER.ImageBase==0x400000 and not hasattr(pe,'DIRECTORY_ENTRY_BASERELOC')
    assert not any(s.Name.rstrip(b'\0')==b'.moon' for s in pe.sections)
    last=pe.sections[-1];rva=align(last.VirtualAddress+max(last.Misc_VirtualSize,last.SizeOfRawData),pe.OPTIONAL_HEADER.SectionAlignment);raw=align(len(before),pe.OPTIONAL_HEADER.FileAlignment)
    header=last.get_file_offset()+40;assert header+40<=pe.OPTIONAL_HEADER.SizeOfHeaders and before[header:header+40]==bytes(40)
    data=bytearray(before);code=bytearray(b'mt_arrow\0');code.extend(bytes(16-len(code)));ks=Ks(KS_ARCH_X86,KS_MODE_32);hooks=[]
    for hook,reg,offset,opcode in [(0x40fa59,'edi',20,0xbf),(0x40fad8,'esi',24,0xbe)]:
        original=bytes([opcode])+struct.pack('<I',0x470ee4);off=pe.get_offset_from_rva(hook-0x400000);assert data[off:off+5]==original
        dest=0x400000+rva+len(code)
        asm=f'pushfd; mov {reg},0x470ee4; cmp dword ptr [esp+{offset}],19060; jb done; cmp dword ptr [esp+{offset}],19080; ja done; mov {reg},{0x400000+rva}; done: popfd; jmp {hook+5}'
        payload=bytes(ks.asm(asm,dest)[0]);data[off:off+5]=b'\xe9'+struct.pack('<i',dest-hook-5);code.extend(payload);code.extend(b'\x90'*(-len(code)%16));hooks.append(dict(va=hook,original=original.hex(),target=dest))
    size=align(len(code),pe.OPTIONAL_HEADER.FileAlignment)
    data[header:header+40]=struct.pack('<8sIIIIIIHHI',b'.moon\0\0\0',len(code),rva,size,raw,0,0,0,0,0x60000020)
    struct.pack_into('<H',data,pe.FILE_HEADER.get_field_absolute_offset('NumberOfSections'),pe.FILE_HEADER.NumberOfSections+1)
    struct.pack_into('<I',data,pe.OPTIONAL_HEADER.get_field_absolute_offset('SizeOfImage'),align(rva+len(code),pe.OPTIONAL_HEADER.SectionAlignment))
    struct.pack_into('<I',data,pe.OPTIONAL_HEADER.get_field_absolute_offset('SizeOfCode'),pe.OPTIONAL_HEADER.SizeOfCode+size)
    struct.pack_into('<I',data,pe.OPTIONAL_HEADER.get_field_absolute_offset('CheckSum'),0)
    data.extend(bytes(raw-len(data)));data.extend(code);data.extend(bytes(size-len(code)))
    result=dict(original_sha256=digest(before),sha256=digest(data),hooks=hooks,range=[19060,19080],previous_sections_preserved=True)
    test_arrow(bytes(data),result);path.write_bytes(data);report.write_text(json.dumps(result,indent=2)+'\n')

def test_arrow(data,result):
    pe=pefile.PE(data=data);mapped=pe.get_memory_mapped_image();cases=0
    for item in [0,11739,19030,19059,*range(19060,19081),19081,19090,19110,65535]:
        u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x400000,0x100000);u.mem_write(0x400000,mapped);u.mem_map(0x2000000,0x10000)
        char,buf,sp,stop=0x2000100,0x2002000,0x200f000,0x400010
        u.mem_write(char+0x300,struct.pack('<3I',buf,0,31));u.mem_write(buf-1,b'\0');u.mem_write(sp,struct.pack('<2I',stop,item))
        regs=[(UC_X86_REG_EBX,0x1111),(UC_X86_REG_EBP,0x2222),(UC_X86_REG_ESI,0x3333),(UC_X86_REG_EDI,0x4444)]
        for reg,v in regs:u.reg_write(reg,v)
        u.reg_write(UC_X86_REG_ECX,char);u.reg_write(UC_X86_REG_ESP,sp);u.emu_start(0x40fa50,stop,count=200)
        text=bytes(u.mem_read(buf,20)).split(b'\0')[0];want=b'mt_arrow' if 19060<=item<=19080 else '단궁기본'.encode('cp949')
        assert text==want,(item,text,want);assert u.reg_read(UC_X86_REG_ESP)==sp+8
        for reg,v in regs:assert u.reg_read(reg)==v
        cases+=1
    result.update(executed_full_assignment_cases=cases,stack_and_callee_saved_registers_preserved=True)
    print('Arrow full native function cases:',cases)

def patch_colors():
    path=ROOT/'client-overlay/Engine.dll';before=path.read_bytes();old=b'\x66\x81\x3f'+struct.pack('<H',19030)+b'\x72\x07\x66\x81\x3f'+struct.pack('<H',19050)
    new=old[:-2]+struct.pack('<H',19110)
    assert before.count(old) in [0,3]
    data=before.replace(old,new);assert data.count(new)==3
    # The contiguous 19030..19110 interval is reserved for the Twilight family.
    pe=pefile.PE(data=data);cases=0
    for base in [0x10000000,0x3500000]:
        p=pefile.PE(data=data);p.relocate_image(base);mapped=p.get_memory_mapped_image()
        for item in [11424,19000,19029,19030,19050,19060,19080,19090,19110,19111]:
            for rank in [1,6,9,20]:
                u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(base,0x100000);u.mem_write(base,mapped);u.mem_map(0x2000000,0x2000)
                u.reg_write(UC_X86_REG_EDI,0x2000000);u.reg_write(UC_X86_REG_ESP,0x2001f00);u.mem_write(0x2000000,struct.pack('<H',item));u.mem_write(0x2000084,struct.pack('<H',rank))
                u.emu_start(base+0x23ccc,base+0x23d28,count=100)
                default=0xffc2c2c2 if rank==1 else 0xffff0000;want=0xff72cfff if 19030<=item<=19110 else default
                assert struct.unpack('<I',u.mem_read(base+0x45e60,4))[0]==want
                for start,end,reg in [(0x23e5e,0x23e63,UC_X86_REG_EAX),(0x23f54,0x23f5a,UC_X86_REG_ECX)]:
                    stack=u.reg_read(UC_X86_REG_ESP);flags=u.reg_read(UC_X86_REG_EFLAGS);u.emu_start(base+start,base+end,count=100)
                    assert u.reg_read(reg)==want and u.reg_read(UC_X86_REG_ESP)==stack and u.reg_read(UC_X86_REG_EFLAGS)==flags
                cases+=1
    path.write_bytes(data);(O/'tooltip-validation.json').write_text(json.dumps(dict(passed=True,cases=cases,rank_color='#72CFFF',name_color='#72CFFF',reserved_family_range=[19030,19110],two_load_addresses=True,sha256=digest(data)),indent=2)+'\n')
    p=ROOT/'src/client/engine/Src/W3DItemInfoMgr.cpp';s=p.read_bytes().decode('cp949').replace('wItemNum <= 19050','wItemNum <= 19110');p.write_bytes(s.encode('cp949'))
    p=ROOT/'assets/twilight/patch_tooltip.py';s=p.read_text(encoding='utf-8').replace('19050','19110').replace('19051','19111');p.write_text(s,encoding='utf-8')
    print('Tooltip relocation/name/rank cases:',cases)

if __name__=='__main__':
    patch_arrow();patch_colors()
    p=ROOT/'src/client/gameclient/Src/Character.cpp';s=p.read_bytes().decode('cp949');s=s.replace('m_sArrow = "단궁기본";','m_sArrow = (nBowIndex >= 19060 && nBowIndex <= 19080) ? "mt_arrow" : "단궁기본";');p.write_bytes(s.encode('cp949'))
