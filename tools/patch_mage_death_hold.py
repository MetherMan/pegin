"""Recognize server state 2: stop a lethal spell's victim before its death animation."""
from pathlib import Path
import json,struct,sys
R=Path(__file__).resolve().parents[1]
sys.path[:0]=[str(R/'runtime/pylibs'),str(R/'tools')]
from pe_hooks import Hooks
from patch_stack_client import machine
from unicorn import UC_HOOK_CODE
from unicorn.x86_const import *
import pefile

def verify_native_monster(data, initial_action=4, resources='ready'):
    """Run shipped monster methods: mocks cannot establish the virtual ABI."""
    p=pefile.PE(data=data)
    vtable=0x46a9b0
    expected={0x24:0x4114a0,0x2c:0x4115a0,0x68:0x411840,
              0xcc:0x411e10,0xd0:0x40f540,0xd4:0x40f550,0x12c:0x41ce50}
    for slot,fn in expected.items():
        assert struct.unpack('<I',p.get_data(vtable-0x400000+slot,4))[0]==fn
    # Stop calls SetState(STOP); slot CC is RemoveAttacker and consumes an
    # argument. Calling CC as Stop corrupts the caller's stack by four bytes.
    assert p.get_data(expected[0xd0]-0x400000,8).hex()=='8b016a00ff5068c3'
    cases=[]
    for state in (0,1,2,3):
        for present in (False,True):
            for animated in (False,True):
                if resources!='ready' and (state!=2 or not present or not animated):continue
                u,_=machine(data)
                game,world,wvt,mob,action,info,saction,anim=0x2000000,0x2003000,0x2004000,0x2005000,0x2007000,0x2008000,0x2009000,0x200a000
                sp,end,lookup=0x201f000,0x201d000,0x201c000
                packet=struct.pack('<HHHB',7,130,321,state)
                u.mem_write(game+0x74,packet);u.mem_write(game+0x1074,struct.pack('<II',game+0x74,4))
                u.mem_write(game+0x24,struct.pack('<I',world));u.mem_write(world,struct.pack('<I',wvt))
                u.mem_write(wvt+0xc8,struct.pack('<I',lookup))
                u.mem_write(lookup,b'\xb8'+struct.pack('<I',mob if present else 0)+b'\xc2\x04\x00')
                u.mem_write(mob,struct.pack('<I',vtable))
                pos=struct.pack('<3f',12.5,45.5,3.0);goal=struct.pack('<3f',22.,55.,4.)
                u.mem_write(mob+0x54,pos);u.mem_write(mob+0x48,goal)
                u.mem_write(mob+0xd0,struct.pack('<I',4))
                u.mem_write(mob+0x1ac,struct.pack('<I',action if animated else 0))
                u.mem_write(action+8,struct.pack('<I',0 if resources=='no_info' else info))
                u.mem_write(action+0xc,struct.pack('<I',0 if resources=='no_action' else saction))
                u.mem_write(info+12,struct.pack('<IIIHHI',1,30,0 if resources=='no_animation' else anim,0,0,0))
                u.mem_write(action+0x1c,struct.pack('<I',99))
                u.mem_write(action+0x24,struct.pack('<HHIIIIf',initial_action,initial_action,350,16,55,1000,1.0))
                u.mem_write(sp,struct.pack('<I',end));u.reg_write(UC_X86_REG_ECX,game)
                preserved={UC_X86_REG_EBX:0x11111111,UC_X86_REG_ESI:0x22222222,UC_X86_REG_EDI:0x33333333,UC_X86_REG_EBP:0x44444444}
                for reg,value in preserved.items():u.reg_write(reg,value)
                executed=set()
                def trace(mu,pc,size,unused):
                    assert pc!=0x411e10, 'RemoveAttacker called instead of Stop'
                    executed.add(pc)
                u.hook_add(UC_HOOK_CODE,trace)
                u.emu_start(0x435990,end,count=4000)
                assert u.reg_read(UC_X86_REG_EIP)==end
                assert u.reg_read(UC_X86_REG_ESP)==sp+4
                assert all(u.reg_read(reg)==value for reg,value in preserved.items())
                frozen=present and state==2
                assert u.mem_read(mob+0x48,12)==(pos if frozen else goal)
                assert struct.unpack('<I',u.mem_read(mob+0xd0,4))[0]==(0 if frozen else 4)
                assert 0x411e10 not in executed, 'Never call RemoveAttacker as Stop'
                if frozen:assert all(fn in executed for fn in (0x4115a0,0x4114a0,0x40f540,0x411840))
                if animated and present and (state!=2 or resources=='ready'):
                    mode=1 if state==2 else int(state==0)
                    assert struct.unpack('<I',u.mem_read(action+0x1c,4))[0]==mode
                if frozen and animated and resources=='ready':
                    assert struct.unpack('<HH',u.mem_read(action+0x24,4))==(0,0)
                    assert bytes(u.mem_read(saction,16))==struct.pack('<4I',1,30,30,anim)
                    # The following native animation frame must follow StateStop,
                    # never the old attack StateAction or an end-of-attack wait.
                    executed.clear();u.mem_write(sp,struct.pack('<II',end,16))
                    u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,action)
                    u.emu_start(0x4079f0,end,count=4000)
                    assert u.reg_read(UC_X86_REG_ESP)==sp+8
                    assert all(u.reg_read(reg)==value for reg,value in preserved.items())
                    assert 0x407c40 in executed and 0x407c60 not in executed
                    assert struct.unpack('<HH',u.mem_read(action+0x24,4))==(0,0)
                if frozen and animated and resources!='ready':assert 0x407af0 not in executed
                cases.append(dict(state=state,present=present,animated=animated,initial_action=initial_action,resources=resources,stack_preserved=True,stopped=frozen))
    return cases

def main():
    p=R/'client-overlay/DeicideOnline.exe';report=R/'assets/skills140/death-hold-client.json'
    h=Hooks(p,report,b'.mdeath')
    h.hook(0x435a68,'8b1084db0f94c1518bc8ff922c010000',
        'cmp bl,2; jne ordinary; mov edi,eax; sub esp,12; mov edx,esp; push edx; '
        'mov ecx,edi; mov eax,[edi]; call dword ptr [eax+0x2c]; '
        'push eax; mov ecx,edi; mov edx,[edi]; call dword ptr [edx+0x24]; '
        'mov ecx,edi; mov edx,[edi]; call dword ptr [edx+0xd0]; '
        'mov ecx,[edi+0x1ac]; test ecx,ecx; jz held; '
        'mov edx,[ecx+8]; test edx,edx; jz held; '
        'cmp dword ptr [ecx+0xc],0; je held; cmp dword ptr [edx+0x14],0; je held; '
        'push 1; mov ecx,edi; mov edx,[edi]; call dword ptr [edx+0x12c]; '
        'mov ecx,[edi+0x1ac]; mov word ptr [ecx+0x26],0; call 0x407af0; '
        'held: add esp,12; jmp 0x435a78; ordinary: mov edx,[eax]; test bl,bl; sete cl; '
        'push ecx; mov ecx,eax; call dword ptr [edx+0x12c]',
        'Immediately stop movement/attack for an already-dead mage victim; keep normal modes')
    data,result=h.finish();result.update(passed=True,
        native_monster_cases=verify_native_monster(data),in_game_visual_test=False)
    for state in range(8):verify_native_monster(data,initial_action=state)
    # Missing resources matter only to the added state-2 path. Ordinary modes
    # retain the native initialization preconditions.
    result['immediate_idle_action_states_checked']=list(range(8))
    result['incomplete_animation_resource_cases']=[case for resource in ('no_info','no_action','no_animation') for case in verify_native_monster(data,resources=resource)]
    # Ensure this test rejects the exact previous bad virtual call, not merely
    # a mock whose offsets were copied from the implementation under test.
    broken=bytearray(data)
    begin=h.raw
    offset=data.index(bytes.fromhex('ff92d0000000'),begin)
    broken[offset+2]=0xcc
    try:verify_native_monster(bytes(broken))
    except AssertionError as error:
        assert str(error)=='RemoveAttacker called instead of Stop'
    else:raise AssertionError('The known crashing virtual call was not detected')
    result['prior_crash_regression_rejected']=True
    p.write_bytes(data);report.write_text(json.dumps(result,indent=2)+'\n')
    p=R/'src/client/gameclient/Src/GameProcessPacket.cpp';s=p.read_bytes().decode('cp949')
    old='\t\tpMonster->SetMode( !btState );'
    new='''        if (btState == 2)
        {
            // Server already settled this kill. Wait still for the climax/death packet.
            pMonster->SetGoalPosition(pMonster->GetPosition());
            pMonster->Stop();
            pMonster->SetMode(2);
        }
        else
            pMonster->SetMode(!btState);'''
    if old in s:s=s.replace(old,new,1)
    s=s.replace(new.replace('SetMode(2)', 'SetMode(TRUE)'),new,1)
    assert new in s;s=s.replace('pMonster->SetMode(!btState);\t\t','pMonster->SetMode(!btState);');p.write_bytes(s.encode('cp949'))
    print('PASS native packet/monster ABI, eight immediate idle action states, next-frame animation, known-crash rejection')

if __name__=='__main__':main()
