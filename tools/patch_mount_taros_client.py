"""Guarded mount-sound and map-41-only time/weather fixes; preserve prior client patches."""
from pathlib import Path
import json,struct,sys
ROOT=Path(__file__).resolve().parents[1]
sys.path[:0]=[str(ROOT/'runtime/pylibs'),str(ROOT/'.cache/weapon-build-deps'),str(ROOT/'tools')]
from pe_hooks import Hooks
from patch_stack_client import machine
from unicorn import UC_HOOK_CODE
from unicorn.x86_const import *

def test_packets(data):
    tests=[]
    for map_id in [14,41,3]:
        for hour in range(24):
            u,_=machine(data);game=0x2000000;world=0x2003000;vt=0x2004000;stub=0x201c000;stop=0x201d000;sp=0x201f000
            packet=struct.pack('<HHBBI',10,64,hour,(hour+1)%24,180000)
            u.mem_write(game+0x74,packet);u.mem_write(game+0x1074,struct.pack('<II',game+0x74,4))
            u.mem_write(game+0x24,struct.pack('<IH',world,map_id));u.mem_write(world,struct.pack('<I',vt))
            u.mem_write(vt+0x30,struct.pack('<I',stub));u.mem_write(stub,b'\xc2\x0c\x00')
            u.mem_write(sp,struct.pack('<I',stop));u.reg_write(UC_X86_REG_ECX,game);got=[]
            def capture(mu,pc,size,unused):
                if pc==stub:got.append(struct.unpack('<III',bytes(mu.mem_read(mu.reg_read(UC_X86_REG_ESP)+4,12))))
            u.hook_add(UC_HOOK_CODE,capture);u.emu_start(0x431ca0,stop,count=2000)
            want=(23,23,180000) if map_id==41 else (hour,(hour+1)%24,180000)
            assert got==[want],(map_id,hour,got)
            assert struct.unpack('<I',u.mem_read(game+0x1078,4))[0]==10
            tests.append((map_id,hour))
    for map_id in [14,41,3]:
        for weather in range(3):
            u,_=machine(data);game=0x2000000;world=0x2003000;vt=0x2004000;stub=0x201c000;stop=0x201d000;sp=0x201f000
            packet=struct.pack('<HHB',5,142,weather)
            u.mem_write(game+0x74,packet);u.mem_write(game+0x1074,struct.pack('<II',game+0x74,4))
            u.mem_write(game+0x24,struct.pack('<IH',world,map_id));u.mem_write(world,struct.pack('<I',vt))
            u.mem_write(vt+0x10c,struct.pack('<I',stub));u.mem_write(stub,b'\xc2\x04\x00')
            u.mem_write(sp,struct.pack('<I',stop));u.reg_write(UC_X86_REG_ECX,game);got=[]
            def capture(mu,pc,size,unused):
                if pc==stub:got.append(struct.unpack('<I',bytes(mu.mem_read(mu.reg_read(UC_X86_REG_ESP)+4,4)))[0])
            u.hook_add(UC_HOOK_CODE,capture);u.emu_start(0x437590,stop,count=1000)
            assert got==[0 if map_id==41 else weather],(map_id,weather,got)
            assert struct.unpack('<I',u.mem_read(game+0x1078,4))[0]==5
    return dict(time_packet_cases=len(tests),weather_packet_cases=9,other_maps_unchanged=True)

def main():
    out=ROOT/'assets/hell-horse';exe=ROOT/'client-overlay/DeicideOnline.exe'
    h=Hooks(exe,out/'sound-weather-exe-hooks.json',b'.mtnight')
    # CGame::m_wMap follows m_pWorld at +0x24; LoadMap writes this WORD at 0x427024.
    assert h.pe.get_data(0x27024,4)==bytes.fromhex('66897e28')
    h.hook(0x431dd2,'8b4e24528b542408',
        'cmp word ptr [esi+0x28],41; jne unchanged; mov byte ptr [esp+8],23; mov byte ptr [esp+4],23; '
        'unchanged: mov ecx,[esi+0x24]; push edx; mov edx,[esp+8]',
        'map 41 always receives current and next night time')
    h.hook(0x437619,'81e2ff000000',
        'and edx,0xff; cmp word ptr [esi+0x28],41; jne unchanged; xor edx,edx; unchanged: nop',
        'map 41 ignores snow and rain changes')
    h.hook(0x4085bc,'66837d2e01',
        'cmp word ptr [ebp+0x2e],1; je done; cmp word ptr [ebp+0x2e],0; jne done; '
        'cmp word ptr [ebp+0x2c],1; done: nop',
        'repeat walking hoof sound as well as existing middle-phase sounds')
    data,report=h.finish();report['validation']=test_packets(data)
    for state in range(50):
        for level in range(3):
            u,_=machine(data);obj=0x2000000;u.reg_write(UC_X86_REG_EBP,obj)
            u.mem_write(obj+0x2c,struct.pack('<HH',state,level));reached=[]
            def halt(mu,pc,size,unused):
                if pc in [0x4085c7,0x4086a0]:reached.append(pc);mu.emu_stop()
            u.hook_add(UC_HOOK_CODE,halt);u.emu_start(0x4085bc,0,count=100)
            assert reached==[0x4085c7 if level==1 or (state==1 and level==0) else 0x4086a0]
    report['validation']['sound_repeat_cases']=150

    ui=ROOT/'client-overlay/UInterface.dll';hu=Hooks(ui,out/'sound-ui-hooks.json',b'.mounts')
    hu.hook(0x100257b8,'3dcc270000',
        'cmp eax,19130; je 0x100257cd; cmp eax,10188',
        'hell-horse certificate uses native horse whinny on use and item handling')
    ui_data,ui_report=hu.finish()
    for base in [0x10000000,0x13000000]:
        for item in [10188,10189,10190,19130,10191,19129,19131,0]:
            u,_=machine(ui_data,base);u.reg_write(UC_X86_REG_EAX,item);reached=[]
            yes=base+0x257cd;no=base+0x257e3
            def halt(mu,pc,size,unused):
                if pc in [yes,no]:reached.append(pc);mu.emu_stop()
            u.hook_add(UC_HOOK_CODE,halt);u.emu_start(base+0x257b8,0,count=100)
            assert reached==[yes if item in [10188,10189,10190,19130] else no]
    ui_report['validation']=dict(item_sound_cases=16,relocated_bases=[0x10000000,0x13000000])
    # Publish only after every native instruction-path test passed.
    exe.write_bytes(data);ui.write_bytes(ui_data)
    for name,value in [('sound-weather-exe-hooks.json',report),('sound-ui-hooks.json',ui_report)]:
        (out/name).write_text(json.dumps(value,indent=2)+'\n',encoding='utf-8')
    print(json.dumps(dict(exe=report['validation'],ui=ui_report['validation'])))

if __name__=='__main__':main()
