"""Fix hell-horse fire emission at the real position-update path.

Default: build and verify in memory only. --apply writes the verified executable.
The obsolete m_nState dust gate is bypassed only for ride kind 4. Other mounts
and every unrelated installed hook remain byte-for-byte intact.
"""
from pathlib import Path
import argparse, hashlib, json, math, struct, sys
R=Path(__file__).resolve().parents[1]
sys.path[:0]=[str(R/'runtime/pylibs'),str(R/'.cache/weapon-build-deps'),str(R/'tools')]
import pefile
from pe_hooks import Hooks,align
from patch_stack_client import machine
from unicorn import UC_HOOK_CODE
from unicorn.x86_const import *

PATH=R/'client-overlay/DeicideOnline.exe'
REPORT=R/'assets/hell-horse/fire-trail-hook.json'
ENTRY=0x4223a3
INTERVAL=80
SLOTS=64


def build(path=PATH):
    h=Hooks(path,REPORT,b'.htrail')
    name_rva=h.rva
    h.code.extend(b'mt_hell_hoof\0'.ljust(16,b'\0'))
    h.code.extend(struct.pack('<4f',.000001,64,.0625,.35))
    min_delta,max_delta,min_distance,height=(h.rva+i for i in (16,20,24,28))
    state_rva=h.rva+h.pe.OPTIONAL_HEADER.SectionAlignment
    # Each zero-initialized 32-byte slot holds the vehicle pointer/id, emission
    # time, last sampled XY and last emitted XY. No game object layout changes.
    assembly=f'''
      cmp dword ptr [ebx+0x24],4; jne ordinary;
      pushfd; pushad; sub esp,16;
      call pic; pic: pop esi; sub esi,pic; add esi,{h.base};
      call dword ptr [esi+0x6a100]; mov ebp,eax;
      mov edi,ebx; shr edi,4; and edi,{SLOTS-1}; shl edi,5;
      add edi,esi; add edi,{state_rva};
      lea edx,[esi+{state_rva+SLOTS*32}]; mov ecx,{SLOTS};
      probe: cmp dword ptr [edi],ebx; je found;
      cmp dword ptr [edi],0; je reset;
      mov eax,ebp; sub eax,[edi+8]; cmp eax,5000; jae reset;
      add edi,32; cmp edi,edx; jb next_slot; sub edi,{SLOTS*32};
      next_slot: loop probe; jmp reset;
      found:
      mov eax,[ebx+0x20]; cmp [edi+4],eax; jne reset;
      fld dword ptr [ebx+0x44]; fsub dword ptr [edi+12]; fmul st(0),st(0);
      fld dword ptr [ebx+0x48]; fsub dword ptr [edi+16]; fmul st(0),st(0);
      faddp st(1),st(0); fstp dword ptr [esp+12];
      mov eax,[ebx+0x44]; mov [edi+12],eax;
      mov eax,[ebx+0x48]; mov [edi+16],eax;
      fld dword ptr [esp+12]; fcomp dword ptr [esi+{min_delta}];
      fnstsw ax; test ah,0x45; jnz done;
      fld dword ptr [esp+12]; fcomp dword ptr [esi+{max_delta}];
      fnstsw ax; test ah,1; jz reset;
      mov eax,ebp; sub eax,[edi+8]; cmp eax,{INTERVAL}; jb done;
      fld dword ptr [ebx+0x44]; fsub dword ptr [edi+20]; fmul st(0),st(0);
      fld dword ptr [ebx+0x48]; fsub dword ptr [edi+24]; fmul st(0),st(0);
      faddp st(1),st(0); fcomp dword ptr [esi+{min_distance}];
      fnstsw ax; test ah,5; jnz done;
      mov [edi+8],ebp;
      mov eax,[ebx+0x44]; mov [edi+20],eax; mov [esp],eax;
      mov eax,[ebx+0x48]; mov [edi+24],eax; mov [esp+4],eax;
      fld dword ptr [ebx+0x4c]; fadd dword ptr [esi+{height}]; fstp dword ptr [esp+8];
      mov ecx,[ebx+0x10]; test ecx,ecx; jz done;
      lea eax,[esp]; push eax;
      lea eax,[esi+{name_rva}]; push eax;
      mov edx,[ecx]; call dword ptr [edx+0x90]; jmp done;
      reset:
      mov [edi],ebx; mov eax,[ebx+0x20]; mov [edi+4],eax;
      mov [edi+8],ebp;
      mov eax,[ebx+0x44]; mov [edi+12],eax; mov [edi+20],eax;
      mov eax,[ebx+0x48]; mov [edi+16],eax; mov [edi+24],eax;
      done: add esp,16; popad; popfd; pop edi; pop esi; jmp 0x4223bf;
      ordinary: mov eax,[ebx+0xb8]
    '''
    h.hook(ENTRY,'8b83b8000000',assembly,'hell-horse actual movement with distance/time budget')
    output,report=h.finish()
    assert len(h.code)<=h.pe.OPTIONAL_HEADER.SectionAlignment
    p=pefile.PE(data=output)
    header=p.sections[-1].get_file_offset()+40
    assert header+40<=p.OPTIONAL_HEADER.SizeOfHeaders and h.data[header:header+40]==bytes(40)
    state_size=SLOTS*32;raw_size=align(state_size,p.OPTIONAL_HEADER.FileAlignment)
    raw_at=align(len(h.data),p.OPTIONAL_HEADER.FileAlignment)
    h.write(header,struct.pack('<8sIIIIIIHHI',b'.htdata',state_size,state_rva,raw_size,raw_at,0,0,0,0,0xc0000040))
    h.write(p.FILE_HEADER.get_field_absolute_offset('NumberOfSections'),struct.pack('<H',p.FILE_HEADER.NumberOfSections+1))
    h.write(p.OPTIONAL_HEADER.get_field_absolute_offset('SizeOfImage'),struct.pack('<I',align(state_rva+state_size,p.OPTIONAL_HEADER.SectionAlignment)))
    h.write(p.OPTIONAL_HEADER.get_field_absolute_offset('SizeOfInitializedData'),struct.pack('<I',p.OPTIONAL_HEADER.SizeOfInitializedData+raw_size))
    h.data.extend(bytes(raw_at-len(h.data)+raw_size))
    output=bytes(h.data)
    report.update(sha256=hashlib.sha256(output).hexdigest(),state_section_rva=state_rva,version=2)
    # Prove this patch touched no original bytes outside its guarded branch and
    # section-directory fields. In particular .mdeath/.hell/.hfire remain intact.
    restored=bytearray(output[:len(h.original)])
    for offset,old in reversed(h.undo):restored[offset:offset+len(bytes.fromhex(old))]=bytes.fromhex(old)
    assert bytes(restored)==h.original
    return output,report


class Harness:
    def __init__(self,data,base=0x400000):
        self.u,_=machine(data,base);self.base=base;self.delta=base-0x400000
        self.calls=[];self.now=0
        self.obj=0x2001000;self.world=0x2002000;self.vt=0x2003000
        self.particle=0x2004000;self.clock=0x2004100;self.action=0x2005000
        self.sp=0x201e000;self.stop=0x201d000
        u=self.u
        u.mem_write(self.world,struct.pack('<I',self.vt))
        u.mem_write(self.vt+0x90,struct.pack('<I',self.particle))
        u.mem_write(self.particle,bytes.fromhex('c20800'))
        u.mem_write(self.clock,b'\xc3')
        u.mem_write(base+0x6a100,struct.pack('<I',self.clock))
        u.mem_write(self.action+0x18,struct.pack('<I',1234))
        u.hook_add(UC_HOOK_CODE,self.observe)
    def observe(self,u,address,size,data):
        if address==self.clock:u.reg_write(UC_X86_REG_EAX,self.now&0xffffffff)
        if address==self.particle:
            sp=u.reg_read(UC_X86_REG_ESP)
            name,pos=struct.unpack('<2I',u.mem_read(sp+4,8))
            n=bytes(u.mem_read(name,32)).split(b'\0')[0]
            self.calls.append((self.now,n,struct.unpack('<3f',u.mem_read(pos,12))))
    def frame(self,time,x,y,z=0,kind=4,state=0,obj=None,identity=1):
        u=self.u;obj=obj or self.obj;self.now=time
        u.mem_write(obj+0xc,struct.pack('<I',self.action))
        u.mem_write(obj+0x10,struct.pack('<I',self.world))
        u.mem_write(obj+0x20,struct.pack('<2I',identity,kind))
        u.mem_write(obj+0x44,struct.pack('<3f',x,y,z))
        u.mem_write(obj+0xb8,struct.pack('<I',state))
        # At the real pre-gate instruction, EDI/ESI/EBP/EBX still have saved
        # caller values on the stack. Exercise the original epilogue and ret 4.
        values={UC_X86_REG_EDI:0x11111111,UC_X86_REG_ESI:0x22222222,
                UC_X86_REG_EBP:0x33333333,UC_X86_REG_EBX:0x44444444}
        u.mem_write(self.sp,b'\x00'*0x70)
        u.mem_write(self.sp,struct.pack('<4I',*values.values()))
        u.mem_write(self.sp+0x50,struct.pack('<2I',self.stop,16))
        u.reg_write(UC_X86_REG_EBX,obj);u.reg_write(UC_X86_REG_EBP,obj+0x44)
        u.reg_write(UC_X86_REG_ESP,self.sp)
        before=len(self.calls)
        old_top=u.reg_read(UC_X86_REG_FPSW)&0x3800
        u.emu_start(ENTRY+self.delta,self.stop,count=1200)
        assert u.reg_read(UC_X86_REG_EIP)==self.stop
        assert u.reg_read(UC_X86_REG_EAX)==1
        assert u.reg_read(UC_X86_REG_ESP)==self.sp+0x58
        assert all(u.reg_read(reg)==value for reg,value in values.items())
        assert u.reg_read(UC_X86_REG_FPSW)&0x3800==old_top,'x87 stack must stay balanced'
        assert struct.unpack('<I',u.mem_read(obj+0xbc,4))[0]==1234
        return len(self.calls)-before


def verify(data,report):
    p=pefile.PE(data=data)
    code=next(s for s in p.sections if s.Name.rstrip(b'\0')==b'.htrail')
    state=next(s for s in p.sections if s.Name.rstrip(b'\0')==b'.htdata')
    assert code.Characteristics==0x60000020 and state.Characteristics==0xc0000040
    assert not any(state.get_data())
    # Reproduce only the old movement gate, retaining unrelated patches that
    # may have been installed after this one. Full undo fidelity is checked
    # during build(), when the original executable bytes are available.
    original=bytearray(data)
    gate=p.get_offset_from_rva(ENTRY-p.OPTIONAL_HEADER.ImageBase)
    original[gate:gate+6]=bytes.fromhex('8b83b8000000')
    legacy=Harness(bytes(original))
    assert legacy.frame(1000,0,0,state=0)==0
    assert legacy.frame(1080,1,0,state=0)==0
    assert legacy.frame(1160,2,0,state=0)==0
    cases=[]
    for base in (0x400000,0x600000):
        # The old hook emits nothing while moving with the unmaintained state
        # field at zero. The new actual-position path must emit despite that.
        h=Harness(data,base)
        assert h.frame(1000,0,0)==0
        assert h.frame(1080,1,0)==1
        assert h.calls[-1][1]==b'mt_hell_hoof' and math.isclose(h.calls[-1][2][2],.35,abs_tol=1e-6)
        assert h.frame(1160,1,0)==0
        assert h.frame(1240,1,0,z=1)==0,'Vertical movement alone must not emit'
        assert h.frame(1241,40,0)==0,'Teleport must reset without a trail'
        assert h.frame(1321,41,0)==1
        assert h.frame(1361,42,0)==0,'80 ms emission cap'
        assert h.frame(1401,42.01,0)==1
        # Tiny XY jitter and slow movement below the 0.25 emission distance.
        assert h.frame(1481,42.0101,0)==0
        assert h.frame(1561,42.02,0)==0
        assert h.frame(1641,42.1,0)==0
        assert h.frame(1721,42.3,0)==1
        # Object reuse and hash collisions reset safely, never copy another
        # horse's old coordinates or emit a joining/teleport streak.
        assert h.frame(1801,43,0,identity=2)==0
        assert h.frame(1881,44,0,identity=2)==1
        assert h.frame(1961,90,0,obj=h.obj+1024,identity=3)==0
        assert h.frame(2041,45,0,identity=2)==1
        assert h.frame(2121,91,0,obj=h.obj+1024,identity=3)==1
        for kind in (0,1,2,3,5,0xffffffff):
            assert h.frame(3000,0,0,kind=kind,state=0)==0
            # The original executable has ASLR disabled and its dust-name
            # immediate is not relocated. Check that untouched original path
            # at its real base; the new branch is independently PIC-tested.
            if base==0x400000:
                assert h.frame(3080,1,0,kind=kind,state=1)==1
                assert h.calls[-1][1]!=b'mt_hell_hoof','Other mounts retain dust'
        wrap=Harness(data,base)
        assert wrap.frame(0xfffffff0,0,0)==0
        assert wrap.frame(0x100000050,1,0)==1
        for fps in (15,30,60,144,240):
            load=Harness(data,base)
            for frame in range(fps*10+1):
                t=round(frame*1000/fps);load.frame(t,t/100,0,state=0)
            assert 50<len(load.calls)<=125,(fps,len(load.calls))
            assert all(b[0]-a[0]>=80 for a,b in zip(load.calls,load.calls[1:]))
            prior=len(load.calls)
            for frame in range(1,fps*3+1):load.frame(10000+round(frame*1000/fps),100,0)
            assert len(load.calls)==prior,'Stopped horse must emit no new particles'
            cases.append(dict(base=base,fps=fps,emissions_per_10_seconds=prior,stopped_emissions=0))
    return dict(passed=True,legacy_missing_trail_reproduced=True,actual_movement_with_legacy_state_zero=True,
                initialized_rw_slots=SLOTS,bounded_collision_probing=True,code_rx_data_rw=True,
                frame_rate_cases=cases,minimum_interval_ms=INTERVAL,
                minimum_emit_distance=.25,maximum_step_before_reset=8,
                ground_lift=.35,teleport_and_object_reuse_checked=True,
                original_epilogue_stack_and_nonvolatile_registers_preserved=True,
                timestamp_wrap_checked=True,other_mounts_unchanged=True,
                maximum_particles_per_second=1000/INTERVAL,
                # Allow one transient newly emitted particle before the next
                # render Update removes an expired entry at the boundary.
                maximum_live_trail_particles=math.ceil(1760/INTERVAL)+1,
                in_game_visual_test=False)


def main():
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--apply',action='store_true');args=parser.parse_args()
    existing=PATH.read_bytes();pe=pefile.PE(data=existing)
    if any(s.Name.rstrip(b'\0')==b'.htrail' for s in pe.sections):
        metadata=json.loads(REPORT.read_text(encoding='utf-8'))
        for hook in metadata['hooks']:
            for va,key in ((hook['va'],'replacement'),(hook['target'],'code')):
                offset=pe.get_offset_from_rva(va-pe.OPTIONAL_HEADER.ImageBase);expected=bytes.fromhex(hook[key])
                assert existing[offset:offset+len(expected)]==expected
        validation=verify(existing,metadata)
        (R/'assets/hell-horse/fire-trail-hook-validation.json').write_text(json.dumps(validation,indent=2)+'\n',encoding='utf-8')
        print(json.dumps(dict(already_installed=True,validation=validation)));return
    output,metadata=build();validation=verify(output,metadata);metadata['validation']=validation
    (R/'assets/hell-horse/fire-trail-hook-validation.json').write_text(json.dumps(validation,indent=2)+'\n',encoding='utf-8')
    if args.apply:
        assert PATH.read_bytes()==existing,'Executable changed during verification; rerun against latest bytes'
        PATH.write_bytes(output);REPORT.write_text(json.dumps(metadata,indent=2)+'\n',encoding='utf-8')
    print(json.dumps(dict(applied=args.apply,validation=validation)))


if __name__=='__main__':main()
