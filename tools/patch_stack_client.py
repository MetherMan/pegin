"""Stage guarded 16-bit item-count patches; never install an unverified pair."""
from pathlib import Path
import json,struct,sys
ROOT=Path(__file__).resolve().parents[1]
sys.path[:0]=[str(ROOT/'runtime/pylibs'),str(ROOT/'tools')]
from pe_hooks import Hooks
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
import pefile

OUT=ROOT/'.cache/stack16-client'
OUT.mkdir(parents=True,exist_ok=True)
REPORTS=ROOT/'assets/stack16'

def previous_report(name):
    published=REPORTS/(name+'.json')
    return published if published.exists() else OUT/(name+'.json')

def receive_count(h,va,old,reg,stack,packet_offset=0x74,width=2,bounds=None):
    address='[eax+esi'+('+'+hex(packet_offset) if packet_offset else '')+']'
    h.hook(va,old,f'movzx {reg}, '+('word' if width==2 else 'byte')+f' ptr {address}; add eax,{width}; mov dword ptr [esp+{stack}],{reg}',f'receive quantity {hex(va)} width={width}')
    if bounds:
        start,old=bounds
        ins=list(h.cs.disasm(bytes.fromhex(old),start))
        asm='; '.join(('add '+i.op_str+',2') if i.mnemonic=='inc' else i.mnemonic+' '+i.op_str for i in ins)
        h.hook(start,old,asm,'quantity bounds '+hex(start))

def build_exe():
    h=Hooks(ROOT/'client-overlay/DeicideOnline.exe',previous_report('DeicideOnline.exe'))
    receive_count(h,0x4312d3,'8a5430744088542410','edx',0x10,bounds=(0x4312ac,'42668b083bd1'))
    receive_count(h,0x4319d1,'8a14304088542414','edx',0x14,0,bounds=(0x4319a4,'42668b083bd1'))
    receive_count(h,0x434486,'8a4c307440884c2408','ecx',8,bounds=(0x434461,'40668b113bc2'))
    receive_count(h,0x434566,'8a4c307440884c2408','ecx',8,bounds=(0x434541,'40668b113bc2'))
    receive_count(h,0x43496d,'8a4c307440884c2410','ecx',0x10,bounds=(0x434942,'41668b023bc8'))
    receive_count(h,0x4350ac,'8a4c307440884c240c','ecx',0xc,bounds=(0x435081,'40668b113bc2'))
    # Legacy cross-server/private-shop packets still have byte quantities. Zero
    # extend their arguments so widening the UI ABI cannot read stack garbage.
    receive_count(h,0x434bcd,'8a4c307440884c2410','ecx',0x10,width=1)
    receive_count(h,0x434f4d,'8a5430744088542414','edx',0x14,width=1)
    receive_count(h,0x43b2ca,'8a4c307440884c2414','ecx',0x14,width=1)
    original=h.pe.get_data(0x28270,0x63)
    asm=[]
    for i in h.cs.disasm(original,0x428270):
        line=i.mnemonic+' '+i.op_str
        if i.address==0x428281:line='push 12'
        elif i.address==0x42829a:line='movzx eax,word ptr [esp+0x1014]'
        elif i.address==0x4282ba:line='mov word ptr [esp+0x12],ax'
        elif i.address==0x4282be:line='mov word ptr [esp+8],12'
        asm.append(line)
    h.hook(0x428270,'b808100000e8d6740200','; '.join(asm),'buy WORD quantity')
    # Initialize outside the loader lock, after CRT startup and before WinMain.
    # This EXE has a fixed image base; DLL quantity patches remain relocatable.
    assert h.pe.OPTIONAL_HEADER.DllCharacteristics & 0x40 == 0
    library=h.base+h.rva+len(h.code);h.code.extend(b'LaqiaCrash.dll\0')
    export=h.base+h.rva+len(h.code);h.code.extend(b'InitializeCrashReporter\0')
    h.hook(0x44fd74,'e857b8ffff',
        f'pushfd; pushad; push {library}; call dword ptr [0x46a0d8]; test eax,eax; jz done; '
        f'push {export}; push eax; call dword ptr [0x46a158]; test eax,eax; jz done; call eax; '
        'done: popad; popfd; call 0x44b5d0','initialize local crash reporter')
    return h,h.finish()

def build_ui():
    h=Hooks(ROOT/'client-overlay/UInterface.dll',previous_report('UInterface.dll'))
    # SItemData has three padding bytes after the former BYTE at offset eight.
    h.hook(0x10009a25,'8a542454c6450800','movzx edx,word ptr [esp+0x54]; mov word ptr [ebp+8],0','item allocation quantity')
    h.hook(0x10009a41,'885508895d10','mov word ptr [ebp+8],dx; mov dword ptr [ebp+0x10],ebx','item store quantity')
    h.hook(0x10009d12,'8a5424588b5c2454','movzx edx,word ptr [esp+0x58]; mov ebx,dword ptr [esp+0x54]','temporary item quantity')
    h.hook(0x10009d37,'885508895d10','mov word ptr [ebp+8],dx; mov dword ptr [ebp+0x10],ebx','temporary item store quantity')
    h.hook(0x100231ac,'8a54240c5657885008','movzx edx,word ptr [esp+0xc]; push esi; push edi; mov word ptr [eax+8],dx','update UI quantity')
    for va,reg,offset in [(0x1002338f,'ebx',0x20),(0x100234ff,'edi',0x20),(0x10023745,'ebx',0x18),(0x100238a5,'ebx',0x18)]:
        old=h.pe.get_data(va-h.base,10).hex()
        h.hook(va,old,f'movzx ecx,word ptr [esp+{offset}]; cmp {reg},0x2771','UI argument '+hex(va))
    for va,offset in [(0x100233c7,0x20),(0x10023537,0x20),(0x1002377d,0x18),(0x100238dd,0x18)]:
        old=h.pe.get_data(va-h.base,9).hex()
        h.hook(va,old,f'test cx,cx; jnz {hex(va+9)}; mov dword ptr [esp+{offset}],1','zero potion normalization '+hex(va))
    h.hook(0x10007ae2,'8a400884c0','movzx eax,word ptr [eax+8]; test eax,eax','count matching inventory items')
    h.replace(0x10007ae9,'25ff000000',bytes.fromhex('25ffff0000'),'count sum mask')
    h.hook(0x10007cf8,'8a470884c0','movzx eax,word ptr [edi+8]; test eax,eax','draw count visibility')
    h.hook(0x10007d27,'8a4f08518d542418','movzx ecx,word ptr [edi+8]; push ecx; lea edx,[esp+0x18]','draw full quantity')
    h.replace(0x10025315,'b863000000',bytes.fromhex('b80f270000'),'shop purchase maximum 9999')
    return h,h.finish()

def machine(data,base=None):
    p=pefile.PE(data=data);base=base or p.OPTIONAL_HEADER.ImageBase
    p.relocate_image(base)
    u=Uc(UC_ARCH_X86,UC_MODE_32)
    u.mem_map(base,0x200000);u.mem_write(base,p.get_memory_mapped_image())
    u.mem_map(0x2000000,0x20000)
    u.reg_write(UC_X86_REG_ESP,0x201f000)
    return u,base

def test_receivers(data,report):
    cases=0
    for e in report['hooks']:
        if not e['label'].startswith('receive quantity'):continue
        width=int(e['label'][-1]);packet_offset=0 if e['va']==0x4319d1 else 0x74
        for count in [0,1,99,255,256,999,9999]:
            if width==1 and count>255:continue
            u,_=machine(data);u.reg_write(UC_X86_REG_ESI,0x2000000);u.reg_write(UC_X86_REG_EAX,0x100)
            u.mem_write(0x2000100+packet_offset,struct.pack('<H',count))
            u.mem_write(0x201f000,b'\xcc'*64)
            u.emu_start(e['va'],e['resume'],count=30)
            offset=int(e['assembly'].split('[esp+')[1].split(']')[0])
            assert struct.unpack('<I',u.mem_read(0x201f000+offset,4))[0]==count
            assert u.reg_read(UC_X86_REG_EAX)==0x100+width
            assert u.reg_read(UC_X86_REG_ESP)==0x201f000
            cases+=1
    # Execute the complete real purchase routine, including its stack probe.
    for count in [1,99,255,256,999,9999]:
        u,_=machine(data)
        u.mem_write(0x201f000,struct.pack('<III',0x400000,(10095<<16)|7,count))
        u.mem_write(0x4779d8,struct.pack('<I',0x2001000))
        u.emu_start(0x428270,0x449fe0,count=300)
        sp=u.reg_read(UC_X86_REG_ESP)
        pointer,size=struct.unpack('<II',u.mem_read(sp+4,8))
        assert size==12
        assert struct.unpack('<HHHiH',u.mem_read(pointer,12))==(12,69,7,10095,count)
        cases+=1
    return cases

def test_ui(data,report):
    cases=0
    hooks={e['label']:e for e in report['hooks']}
    for base in [0x10000000,0x3500000]:
        for count in [0,1,99,255,256,999,9999]:
            for e in report['hooks']:
                if not e['label'].startswith(('UI argument','zero potion normalization')):continue
                u,_=machine(data,base)
                offset=int(e['assembly'].split('[esp+')[1].split(']')[0])
                u.mem_write(0x201f000+offset,struct.pack('<I',count))
                u.reg_write(UC_X86_REG_ECX,count)
                u.emu_start(base+e['va']-0x10000000,base+e['resume']-0x10000000,count=30)
                if e['label'].startswith('UI argument'):assert u.reg_read(UC_X86_REG_ECX)==count
                else:assert struct.unpack('<I',u.mem_read(0x201f000+offset,4))[0]==max(1,count)
                cases+=1
            for label in ['item store quantity','temporary item store quantity','update UI quantity','count matching inventory items','draw count visibility','draw full quantity']:
                e=hooks[label];u,_=machine(data,base)
                obj=0x2001000
                for reg in [UC_X86_REG_EAX,UC_X86_REG_EBP,UC_X86_REG_EDI]:u.reg_write(reg,obj)
                u.reg_write(UC_X86_REG_EDX,count);u.mem_write(obj+8,struct.pack('<H',count))
                u.mem_write(0x201f00c,struct.pack('<I',count))
                u.emu_start(base+e['va']-0x10000000,base+e['resume']-0x10000000,count=30)
                if 'store quantity' in label or label=='update UI quantity':assert struct.unpack('<H',u.mem_read(obj+8,2))[0]==count
                elif label=='draw full quantity':assert struct.unpack('<I',u.mem_read(0x201effc,4))[0]==count
                else:assert u.reg_read(UC_X86_REG_EAX)==count
                cases+=1
    return cases

def test_packet_handlers(data):
    cases=0
    for qty in [1,255,256,999,9999]:
        tests=[
            (0x431080,43,struct.pack('<iiBBBH',7,10095,1,2,3,qty),0xbc,[7,10095,1,2,3,qty,1]),
            (0x431750,60,struct.pack('<HiHBBBH',1,7,10095,1,2,3,qty)+b'\0',0xbc,[7,10095,1,2,3,qty,0]),
            (0x4343d0,115,struct.pack('<iH',7,qty),0xc4,[7,qty]),
            (0x4344b0,256,struct.pack('<iH',7,qty),0xc4,[7,qty]),
            (0x4347d0,121,struct.pack('<HHiHi',1,10095,7,qty,1234),0x11c,[7,10095,qty]),
            (0x434f90,122,struct.pack('<HiH',10095,7,qty),0x11c,[7,10095,qty]),
        ]
        for va,tag,payload,slot,expected in tests:
            u,_=machine(data);game=0x2000000;gui=0x200a000;vtable=0x200b000;end=0x201d000
            raw=struct.pack('<HH',len(payload)+4,tag)+payload
            u.mem_write(game+0x74,raw)
            u.mem_write(game+0x1074,struct.pack('<II',game+0x74,4))
            u.mem_write(0x4779ec,struct.pack('<I',gui));u.mem_write(gui,struct.pack('<I',vtable))
            events=[];callbacks={}
            for off,nargs in [(0xbc,7),(0xc4,2),(0x11c,3),(0x130,0),(0x30,3),(0x124,1)]:
                address=0x200c000+off
                u.mem_write(vtable+off,struct.pack('<I',address))
                u.mem_write(address,b'\xc2'+struct.pack('<H',nargs*4))
                callbacks[address]=(off,nargs)
            def capture(uc,address,size,_):
                if address not in callbacks:return
                off,nargs=callbacks[address];sp=uc.reg_read(UC_X86_REG_ESP)
                args=list(struct.unpack('<'+'I'*nargs,uc.mem_read(sp+4,nargs*4))) if nargs else []
                events.append((off,args));uc.reg_write(UC_X86_REG_EAX,1)
            u.hook_add(UC_HOOK_CODE,capture)
            u.reg_write(UC_X86_REG_ECX,game);u.mem_write(0x201f000,struct.pack('<I',end))
            try:u.emu_start(va,end,count=10000)
            except Exception as error:
                raise AssertionError((tag,qty,hex(u.reg_read(UC_X86_REG_EIP)),hex(u.reg_read(UC_X86_REG_ESP)),events)) from error
            assert (slot,expected) in events,(tag,qty,events)
            assert struct.unpack('<I',u.mem_read(game+0x1078,4))[0]==len(raw),(tag,qty)
            assert u.reg_read(UC_X86_REG_ESP)==0x201f004
            cases+=1
    return cases

def test_crash_bootstrap(data,report):
    entry=next(e for e in report['hooks'] if e['label']=='initialize local crash reporter')
    for mode in ['missing DLL','missing export','initialized']:
        u,_=machine(data);calls=[]
        for iat,address,nargs in [(0x46a0d8,0x200b000,1),(0x46a158,0x200b100,2)]:
            u.mem_write(iat,struct.pack('<I',address));u.mem_write(address,b'\xc2'+struct.pack('<H',nargs*4))
        u.mem_write(0x200b200,b'\xc3')
        u.mem_write(0x44b5d0,b'\xc2\x10\x00')
        registers={UC_X86_REG_EAX:0x12345678,UC_X86_REG_EBX:0x23456789,UC_X86_REG_ECX:0x34567890,
            UC_X86_REG_EDX:0x45678901,UC_X86_REG_ESI:0x56789012,UC_X86_REG_EDI:0x67890123,UC_X86_REG_EBP:0x78901234}
        for reg,value in registers.items():u.reg_write(reg,value)
        u.reg_write(UC_X86_REG_EFLAGS,0x246)
        u.mem_write(0x201f000,struct.pack('<IIII',7,8,9,10))
        def capture(uc,address,size,_):
            if address==0x200b000:
                ptr=struct.unpack('<I',uc.mem_read(uc.reg_read(UC_X86_REG_ESP)+4,4))[0]
                assert bytes(uc.mem_read(ptr,len(b'LaqiaCrash.dll\0')))==b'LaqiaCrash.dll\0'
                calls.append('load');uc.reg_write(UC_X86_REG_EAX,0 if mode=='missing DLL' else 0x700000)
            elif address==0x200b100:
                module,ptr=struct.unpack('<II',uc.mem_read(uc.reg_read(UC_X86_REG_ESP)+4,8))
                assert module==0x700000 and bytes(uc.mem_read(ptr,24))==b'InitializeCrashReporter\0'
                calls.append('export');uc.reg_write(UC_X86_REG_EAX,0 if mode=='missing export' else 0x200b200)
            elif address==0x200b200:
                calls.append('initialize');uc.reg_write(UC_X86_REG_EAX,1)
                uc.reg_write(UC_X86_REG_ECX,0xdead);uc.reg_write(UC_X86_REG_EDX,0xbeef)
            elif address==0x44b5d0:
                calls.append('WinMain')
                assert all(uc.reg_read(reg)==value for reg,value in registers.items())
                assert uc.reg_read(UC_X86_REG_EFLAGS)==0x246
                assert bytes(uc.mem_read(uc.reg_read(UC_X86_REG_ESP)+4,16))==struct.pack('<IIII',7,8,9,10)
                uc.reg_write(UC_X86_REG_EAX,55)
        u.hook_add(UC_HOOK_CODE,capture)
        u.emu_start(entry['va'],entry['resume'],count=100)
        assert calls==({'missing DLL':['load','WinMain'],'missing export':['load','export','WinMain'],
            'initialized':['load','export','initialize','WinMain']}[mode])
        assert u.reg_read(UC_X86_REG_ESP)==0x201f010 and u.reg_read(UC_X86_REG_EAX)==55
    return 3


def main():
    exe,(exedata,exereport)=build_exe()
    ui,(uidata,uireport)=build_ui()
    cases=test_receivers(exedata,exereport)+test_ui(uidata,uireport)+test_packet_handlers(exedata)+test_crash_bootstrap(exedata,exereport)
    for name,data,report in [('DeicideOnline.exe',exedata,exereport),('UInterface.dll',uidata,uireport)]:
        (OUT/name).write_bytes(data);(OUT/(name+'.json')).write_text(json.dumps(report,indent=2)+'\n')
    result=dict(passed=True,emulated_cases=cases,complete_packet_handler_cases=30,dll_load_bases=[0x10000000,0x3500000],staged=str(OUT),installed=False)
    (OUT/'validation.json').write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps(result))

if __name__=='__main__':main()
