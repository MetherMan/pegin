"""Stage a scoped fog correction for explicitly marked hell-horse particles.

Native additive particles use ONE/ONE blending, so fog colors their nominally
black background. Only ParticleHead::nTick == 140041 disables fog during each
draw; the native TRUE state is restored immediately after it. Other particles,
texture/color/size choices and existing Render.dll patches remain untouched.
"""
from pathlib import Path
import argparse
import hashlib
import json
import struct
import sys

R = Path(__file__).resolve().parents[1]
sys.path[:0] = [str(R/'runtime/pylibs'), str(R/'client-overlay/Tools/SkillColors')]
import pefile
from pe_hooks import Hooks
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import *

DLL = R/'client-overlay/Render.dll'
REPORT = R/'assets/hell-horse/particle-fog-hook.json'
MARKER = 140041


def build(path=DLL, report=REPORT):
    h = Hooks(path, report, b'.hpfog')
    image = h.pe.get_memory_mapped_image()
    # Prove the scope assumptions from the shipped native code, not just source.
    assert image[0x1c974:0x1c981] == bytes.fromhex('8b086a016a1c50ff91c8000000')
    assert image[0x1bcc0:0x1bcc7] == bytes.fromhex('8b4b043bc1761b')
    # PIC helper loads the already-validated native device global. Its call/pop
    # anchor avoids new absolute relocations in the appended executable section.
    helper = h.base+h.rva+len(h.code)
    h.code.extend(h.asm(f'call {helper+5}; pop eax; sub eax, {helper+5-h.base}; '
                        'mov eax, dword ptr [eax+0x89328]; ret', helper))
    def set_fog(value, label):
        return f'''
            pushfd;
            pushad;
            mov eax, dword ptr [ebp+8];
            mov eax, dword ptr [eax];
            cmp dword ptr [eax+4], {MARKER};
            jne {label};
            call {helper};
            mov ecx, dword ptr [eax];
            push {value};
            push 0x1c;
            push eax;
            call dword ptr [ecx+0xc8];
        {label}:
            popad;
            popfd;
        '''
    for va, old, draw, label in (
        (0x1001c79c,'ff9118010000','call dword ptr [ecx+0x118]', 'dark'),
        (0x1001c862,'e8895bffff','call 0x100123f0', 'light')):
        h.hook(va,old,set_fog(0,'before_done')+draw+';'+set_fog(1,'after_done'),
               f'Disable fog only during marked hell-horse {label} particle draw')
    data,report = h.finish()
    report.update(marker=dict(field='ParticleHead::nTick',native_offset=4,value=MARKER),
                  behavior='FOGENABLE=FALSE only around each marked draw, then native TRUE restored',
                  pic_device_helper=helper)
    return data,report,h.original


def native_render(data, base, ticks, visible=None):
    p = pefile.PE(data=data); p.relocate_image(base)
    u = Uc(UC_ARCH_X86,UC_MODE_32)
    u.mem_map(base,(p.OPTIONAL_HEADER.SizeOfImage+4095)&~4095)
    u.mem_write(base,p.get_memory_mapped_image())
    u.mem_map(0x30000000,0x10000); u.mem_map(0x40000000,0x10000)
    obj,head,array,texture,vb,device,vtable = [0x30000100+i*0x100 for i in range(7)]
    stack,stop = 0x40008000,0x3000f000
    def put(at,value): u.mem_write(at,struct.pack('<I',value))
    def get(at): return struct.unpack('<I',u.mem_read(at,4))[0]
    put(base+0x89328,device); put(device,vtable)
    put(obj+0x1c,head); put(obj+0x24,vb)
    put(obj+0x2c,array); put(obj+0x30,array+4); put(array,texture)
    put(texture+0x14,0x7777)
    put(vb+4,0x102); put(vb+8,20); put(vb+12,0x8888)
    nodes = [0x30001000+i*0x100 for i in range(len(ticks))]
    put(head,nodes[0] if nodes else head)
    visible = visible if visible is not None else [True]*len(ticks)
    for i,(node,tick,shown) in enumerate(zip(nodes,ticks,visible)):
        particle,header = 0x30003000+i*0x100,0x30005000+i*0x100
        put(node,nodes[i+1] if i+1<len(nodes) else head); put(node+8,particle)
        put(particle,header); put(particle+0x3c,int(shown))
        put(particle+0x30,0xffa03804); put(particle+0x34,0xff000000)
        u.mem_write(particle+0x38,struct.pack('<f',.27))
        put(header+4,tick); put(header+0xa8,0)
    services = {0xc8:3,0xfc:4,0xf4:3,0x130:2,0x14c:4,0x118:4}
    for slot in services: put(vtable+slot,0x30008000+slot)
    calls,draws,worlds = [],[],[]
    states = {28:1}
    def ret(argc,value=0):
        sp = u.reg_read(UC_X86_REG_ESP)
        target = get(sp)
        u.reg_write(UC_X86_REG_EAX,value); u.reg_write(UC_X86_REG_ECX,0xc1c1c1c1)
        u.reg_write(UC_X86_REG_EDX,0xd2d2d2d2)
        u.reg_write(UC_X86_REG_ESP,sp+4*(argc+1)); u.reg_write(UC_X86_REG_EIP,target)
    def service(u,address,size,_):
        if address == base+0x1c050:  # Animation/update is separately verified.
            ret(1); return
        if address == base+0x1c4c0:  # Camera transform is not relevant to fog state.
            worlds.append(get(u.reg_read(UC_X86_REG_ESP)+4)); ret(2); return
        slot = address-0x30008000
        if slot not in services: return
        sp = u.reg_read(UC_X86_REG_ESP)
        args = [get(sp+4*(i+1)) for i in range(services[slot])]
        assert args[0] == device
        calls.append([slot,*args[1:]])
        if slot == 0xc8: states[args[1]] = args[2]
        if slot == 0x118:
            node = u.reg_read(UC_X86_REG_EBP)
            header = get(get(node+8))
            draws.append(dict(marker=get(header+4),fog=states[28],src=states[19],dst=states[20],args=args[1:]))
        ret(services[slot])
    u.hook_add(UC_HOOK_CODE,service)
    saved = {UC_X86_REG_EBX:0x1357,UC_X86_REG_ESI:0x2468,
             UC_X86_REG_EDI:0x3579,UC_X86_REG_EBP:0x468a}
    for reg,value in saved.items(): u.reg_write(reg,value)
    u.mem_write(stack,struct.pack('<II',stop,33))
    u.reg_write(UC_X86_REG_ESP,stack); u.reg_write(UC_X86_REG_ECX,obj)
    u.emu_start(base+0x1c570,stop,count=100000)
    assert u.reg_read(UC_X86_REG_EIP) == stop
    assert u.reg_read(UC_X86_REG_ESP) == stack+8
    assert all(u.reg_read(reg)==value for reg,value in saved.items())
    assert states[28]==1
    return dict(draws=draws,calls=calls,worlds=worlds,final_fog=states[28])


def native_birth_life(data,base,tick,elapsed,life=385):
    """Run native AddParticle's nTick/age gate without its unrelated map lookup."""
    p = pefile.PE(data=data); p.relocate_image(base)
    u = Uc(UC_ARCH_X86,UC_MODE_32)
    u.mem_map(base,(p.OPTIONAL_HEADER.SizeOfImage+4095)&~4095)
    u.mem_write(base,p.get_memory_mapped_image())
    u.mem_map(0x30000000,0x1000); u.mem_map(0x40000000,0x1000)
    obj,header,stack = 0x30000100,0x30000300,0x40000800
    u.mem_write(obj+0x3c,struct.pack('<I',elapsed))
    u.mem_write(header+4,struct.pack('<I',tick))
    u.reg_write(UC_X86_REG_EBP,obj); u.reg_write(UC_X86_REG_EBX,header)
    u.reg_write(UC_X86_REG_EDI,life); u.reg_write(UC_X86_REG_ESP,stack)
    calls = []
    def service(u,address,size,_):
        if address==base+0x63997:
            calls.append('rand')
            sp=u.reg_read(UC_X86_REG_ESP)
            ret=struct.unpack('<I',u.mem_read(sp,4))[0]
            u.reg_write(UC_X86_REG_EAX,42)
            u.reg_write(UC_X86_REG_ESP,sp+4); u.reg_write(UC_X86_REG_EIP,ret)
    u.hook_add(UC_HOOK_CODE,service)
    u.emu_start(base+0x1bcbd,base+0x1bce2,count=1000)
    assert u.reg_read(UC_X86_REG_EIP)==base+0x1bce2
    assert u.reg_read(UC_X86_REG_ESP)==stack
    remaining=u.reg_read(UC_X86_REG_EDI)
    assert remaining==(life if elapsed<=tick else life-42%(elapsed-tick))
    return dict(tick=tick,elapsed_ms=elapsed,remaining_ms=remaining,rand_calls=len(calls))


def verify(original,patched):
    tests,births = [],[]
    cases = [([],[]),([33],[True]),([MARKER],[True]),
             ([33,MARKER,47,MARKER,125],[True]*5),
             ([MARKER,33,MARKER],[False,True,False]),
             ([MARKER-1,MARKER+1],[True,True])]
    for base in (0x10000000,0x16000000):
        for elapsed in (1,16,33,80,200,1000):
            births.append(dict(base=hex(base),legacy=native_birth_life(patched,base,33,elapsed),
                               marked=native_birth_life(patched,base,MARKER,elapsed)))
        for ticks,visible in cases:
            old = native_render(original,base,ticks,visible)
            new = native_render(patched,base,ticks,visible)
            assert len(new['draws'])==sum(visible)*2
            for before,after in zip(old['draws'],new['draws']):
                assert before['fog']==1
                assert after['fog']==int(after['marker']!=MARKER)
                assert {k:v for k,v in before.items() if k!='fog'}=={k:v for k,v in after.items() if k!='fog'}
            marked = sum(t==MARKER and v for t,v in zip(ticks,visible))
            assert len(new['calls'])-len(old['calls'])==4*marked
            assert old['worlds']==new['worlds']
            if marked==0: assert old==new
            # Remove the exact fog-off/on pairs around marked draws; every
            # other device call must remain in the same order with same args.
            filtered = []
            calls = new['calls']
            i = 0
            while i<len(calls):
                if calls[i]==[0xc8,28,0]:
                    j=i+1
                    while calls[j]!=[0xc8,28,1]: j+=1
                    filtered.extend(calls[i+1:j]); i=j+1
                else:
                    filtered.append(calls[i]); i+=1
            assert filtered==old['calls']
            tests.append(dict(base=hex(base),ticks=ticks,visible=visible,draws=new['draws'],
                              extra_device_calls=4*marked,fog_restored=True))
    return dict(passed=True,native_render_cases=tests,
                native_birth_life_cases=births,
                actual_native_functions=['CW3DParticles::Render','CW3DParticles::SetRenderState','CVertexBuffer::Draw'],
                substituted_services=['D3D device methods','particle Update','SetWorld camera transform'],
                unchanged_unmarked_call_sequences=True,exact_mark_only=True,
                nonvolatile_registers_and_stack=True,in_game_visual_test=False)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--input',type=Path,default=DLL)
    parser.add_argument('--output',type=Path,default=R/'runtime/performance/Render-hell-particle-fog.dll')
    parser.add_argument('--report',type=Path,default=R/'runtime/performance/particle-fog-hook.json')
    args = parser.parse_args()
    input_before = args.input.read_bytes()
    data,report,original = build(args.input)
    report['validation'] = verify(original,data)
    assert args.input.read_bytes() == input_before, 'Input changed during validation'
    args.output.parent.mkdir(parents=True,exist_ok=True)
    args.report.parent.mkdir(parents=True,exist_ok=True)
    args.output.write_bytes(data)
    args.report.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
    print('PASS: staged hell-particle fog patch, native Render at two bases, ordinary particles unchanged')
    print(str(args.output))


if __name__ == '__main__': main()
