"""Execute hoof hook with real Render.GetBoneMatrix/Common.SetWorld binaries.

Only world particle/terrain, wall clock and the animation container accessors
are stand-ins. Frame selection, named-bone lookup, pose matrix, transforms,
landing branch and the original vehicle epilogue execute as native x86.
"""
from pathlib import Path
import hashlib,json,math,struct,sys
R=Path(__file__).resolve().parents[1]
sys.path[:0]=[str(R/'runtime/pylibs'),str(R/'tools'),str(R/'client-overlay/Tools/SkillColors')]
import pefile
from unicorn import UC_HOOK_CODE
from unicorn.x86_const import *
from native_actor import animation
from patch_stack_client import machine
from patch_hell_horse_hoof import FEET,ENTRY


def matrix(q):
    x,y,z,w=q
    return [[1-2*(y*y+z*z),2*(x*y-w*z),2*(z*x+w*y)],
            [2*(x*y+w*z),1-2*(x*x+z*z),2*(y*z-w*x)],
            [2*(z*x-w*y),2*(y*z+w*x),1-2*(x*x+y*y)]]


def mul(v,m,p):return [sum(v[k]*m[k][a] for k in range(3))+p[a] for a in range(3)]
def f32(x):return struct.unpack('<f',struct.pack('<f',x))[0]


class Harness:
    def __init__(self,data,base=0x400000):
        self.u,_=machine(data,base);self.base=base;self.delta=base-0x400000
        self.calls=[];self.now=0;self.current_foot=None;self.bone_calls=0
        self.obj=0x2001000;self.world=0x2002000;self.vt=0x2003000
        self.particle=0x2004000;self.clock=0x2004100;self.height=0x2004200
        self.getnum=0x2004300;self.getbone=0x2004400;self.action=0x2005000
        self.model=0x2006000;self.modelvt=0x2006400;self.ani=0x2006500
        self.anivt=0x2006600;self.saction=0x2006800
        self.sp=0x201e000;self.stop=0x201d000;self.bonebase=0x2100000
        self.boneindex={n:i for i,(n,p) in enumerate(FEET)}
        u=self.u;u.mem_map(0x2100000,0x100000)
        for name,baseaddr in [('Render.dll',0x10000000),('Common.dll',0x11000000)]:
            p=pefile.PE(str(R/'client-overlay'/name));p.relocate_image(baseaddr)
            u.mem_map(baseaddr,(p.OPTIONAL_HEADER.SizeOfImage+0xfff)&~0xfff)
            u.mem_write(baseaddr,p.get_memory_mapped_image())
        # Static Matrix4 constructors only initialize identity before being
        # completely overwritten; skip their CRT atexit registration here.
        u.mem_write(0x10089018,b'\x07');u.mem_write(0x110202c0,b'\x01')
        self.put(0x10073038,0x11007220);self.put(base+0x6a05c,0x11007220)
        self.put(self.world,self.vt);self.put(self.vt+0x90,self.particle)
        self.put(self.vt+0xa4,self.height);self.put(base+0x6a100,self.clock)
        u.mem_write(self.particle,bytes.fromhex('c20800'));u.mem_write(self.clock,b'\xc3')
        # fld dword ptr [terrain scratch]; ret4 (x87 float return ABI).
        u.mem_write(self.height,b'\xd9\x05'+struct.pack('<I',self.height+16)+b'\xc2\x04\x00')
        u.mem_write(self.getnum,b'\xb8'+struct.pack('<I',32)+b'\xc3')
        u.mem_write(self.getbone,bytes.fromhex('8b44240469c0a000000005')+struct.pack('<I',self.bonebase)+bytes.fromhex('c20400'))
        self.put(self.ani,self.anivt);self.put(self.anivt+0x10,self.getbone);self.put(self.anivt+0x14,self.getnum)
        self.put(self.model,self.modelvt);self.put(self.modelvt+0x3c,0x10013620)
        self.put(self.model+0x10,self.saction);self.put(self.model+0x90,35);self.put(self.model+0x1b8,self.ani)
        for address in (self.clock,self.particle,self.height,0x10013620):
            u.hook_add(UC_HOOK_CODE,self.observe,begin=address,end=address)

    def put(self,address,value):self.u.mem_write(address,struct.pack('<I',value))

    def clip(self,meta):
        self.meta=meta;raw=(R/meta['source']).read_bytes();count=struct.unpack_from('<i',raw,32)[0]
        self.animation=animation(raw,list(range(count)))
        assert len(self.animation['bones'])==32
        self.u.mem_write(self.saction,struct.pack('<4I',meta['first'],meta['last'],meta['last']-meta['first']+1,self.ani))
        at=self.bonebase+32*160
        for i,b in enumerate(self.animation['bones']):
            bone=self.bonebase+i*160
            self.u.mem_write(bone,b['name'].encode().ljust(20,b'\0'))
            pos=b''.join(struct.pack('<3f',*p[:3]) for p in b['poses'])
            rot=b''.join(struct.pack('<4f',*p[3:]) for p in b['poses'])
            self.put(bone+152,at);self.u.mem_write(at,pos);at+=len(pos)
            self.put(bone+156,at);self.u.mem_write(at,rot);at+=len(rot)
        self.model_points={}
        for index,(name,sole) in enumerate(FEET):
            bone=next(b for b in self.animation['bones'] if b['name']==name)
            self.model_points[index]=[mul(sole,matrix(p[3:]),p[:3]) for p in bone['poses']]

    def observe(self,u,address,size,data):
        if address==self.clock:u.reg_write(UC_X86_REG_EAX,self.now&0xffffffff)
        elif address==0x10013620:
            sp=u.reg_read(UC_X86_REG_ESP);dest,name,tick=struct.unpack('<3I',u.mem_read(sp+4,12))
            name=bytes(u.mem_read(name,20)).split(b'\0')[0].decode()
            assert name in self.boneindex and tick==self.animtick
            assert dest==self.sp-36-192,'Actual hidden return pointer points at Matrix4 scratch'
            self.current_foot=self.boneindex[name];self.bone_calls+=1
        elif address==self.height:
            sp=u.reg_read(UC_X86_REG_ESP);ptr=struct.unpack('<I',u.mem_read(sp+4,4))[0]
            x,y,z=struct.unpack('<3f',u.mem_read(ptr,12))
            u.mem_write(self.height+16,struct.pack('<f',self.terrain(x,y)))
        elif address==self.particle:
            sp=u.reg_read(UC_X86_REG_ESP);name,pos=struct.unpack('<2I',u.mem_read(sp+4,8))
            n=bytes(u.mem_read(name,32)).split(b'\0')[0]
            xyz=struct.unpack('<3f',u.mem_read(pos,12))
            self.calls.append(dict(now=self.now,name=n.decode('cp949'),foot=self.current_foot,frame=self.sourceframe,xyz=xyz))
            if n==b'mt_hell_hoof':
                expected=mul(self.model_points[self.current_foot][self.sourceframe],matrix(self.quaternion),self.position)
                expected[2]=self.terrain(*expected[:2])+.20
                assert max(abs(a-b) for a,b in zip(xyz,expected))<3e-5,(xyz,expected)

    @staticmethod
    def terrain(x,y):return .2*x-.1*y+.5

    def frame(self,time,x,y,z=0,kind=4,state=0,obj=None,identity=1,animtick=None,quaternion=None,model_enabled=True,action_enabled=True):
        u=self.u;obj=obj or self.obj;self.now=time;self.animtick=int(time if animtick is None else animtick)
        self.quaternion=tuple(f32(q) for q in (quaternion or (-math.sqrt(.5),0,0,math.sqrt(.5))))
        self.position=tuple(f32(v) for v in (x,y,z))
        self.sourceframe=self.meta['first']+((self.animtick*35)//1000)%(self.meta['last']-self.meta['first']+1)
        self.put(obj,self.model if model_enabled else 0);self.put(obj+4,self.saction if action_enabled else 0)
        self.put(obj+0xc,self.action);self.put(obj+0x10,self.world)
        u.mem_write(obj+0x20,struct.pack('<2I',identity,kind));u.mem_write(obj+0x44,struct.pack('<3f',*self.position))
        u.mem_write(obj+0x50,struct.pack('<4f',*self.quaternion));self.put(obj+0xb8,state)
        self.put(self.action+0x18,self.animtick)
        values={UC_X86_REG_EDI:0x11111111,UC_X86_REG_ESI:0x22222222,UC_X86_REG_EBP:0x33333333,UC_X86_REG_EBX:0x44444444}
        u.mem_write(self.sp,bytes(0x70));u.mem_write(self.sp,struct.pack('<4I',*values.values()))
        u.mem_write(self.sp+0x50,struct.pack('<2I',self.stop,16))
        u.reg_write(UC_X86_REG_EBX,obj);u.reg_write(UC_X86_REG_EBP,obj+0x44);u.reg_write(UC_X86_REG_ESP,self.sp)
        before=len(self.calls);old_top=u.reg_read(UC_X86_REG_FPSW)&0x3800
        u.emu_start(ENTRY+self.delta,self.stop,count=20000)
        assert u.reg_read(UC_X86_REG_EIP)==self.stop,hex(u.reg_read(UC_X86_REG_EIP))
        assert u.reg_read(UC_X86_REG_EAX)==1 and u.reg_read(UC_X86_REG_ESP)==self.sp+0x58
        assert all(u.reg_read(reg)==value for reg,value in values.items()),'callee-saved registers'
        assert u.reg_read(UC_X86_REG_FPSW)&0x3800==old_top,'x87 stack stays balanced'
        assert struct.unpack('<I',u.mem_read(obj+0xbc,4))[0]==self.animtick
        return len(self.calls)-before


def verify(data,report):
    p=pefile.PE(data=data);sections={s.Name.rstrip(b'\0'):s for s in p.sections}
    assert sections[b'.hhoof'].Characteristics==0x60000020
    assert sections[b'.hfdata'].Characteristics==0xc0000040
    assert not any(sections[b'.hfdata'].get_data())
    motion=json.loads((R/'assets/hell-horse/hoof-motion.json').read_text())
    clips={'run':dict(motion['run'],feet=motion['feet']),**motion['otherClips']}
    cases=[]
    for base in (0x400000,0x600000):
        h=Harness(data,base)
        for clipname in ('run','run-bow','walk'):
            meta=clips[clipname];h.clip(meta)
            for fps in (10,15,30,60,144):
                h.u.mem_write(base+report['state_section_rva'],bytes(4096));h.calls=[]
                expected=[];armed=[False]*4;previous=[0]*4;period=meta['last']-meta['first']+1
                for sample in range(fps*2+1):
                    time=round(sample*1000/fps);frame=meta['first']+((time*35)//1000)%period
                    if sample:
                        for foot in range(4):
                            height=h.model_points[foot][frame][1]
                            if height>.22:armed[foot]=True
                            elif armed[foot] and height<=.16 and height<=previous[foot]:
                                expected.append((time,foot,frame));armed[foot]=False
                            previous[foot]=height
                    h.frame(time,time/100,0,animtick=time)
                actual=[(c['now'],c['foot'],c['frame']) for c in h.calls]
                assert actual==expected,(clipname,fps,actual,expected)
                assert 4<=len(actual)<=20,(clipname,fps,len(actual))
                # A stopped horse does not query bones or add any particles.
                before=(len(h.calls),h.bone_calls)
                for sample in range(1,fps+1):h.frame(2000+round(sample*1000/fps),20,0)
                assert before==(len(h.calls),h.bone_calls)
                assert h.frame(3100,90,90)==0,'Teleport resets without a flame'
                assert h.frame(3200,91,90,identity=2)==0,'Object reuse resets'
                assert h.frame(3300,100,0,obj=h.obj+1024,identity=3)==0,'Colliding hash slot independent'
                cases.append(dict(base=base,clip=clipname,fps=fps,emissions_per_2_seconds=len(actual),idle_emissions=0))
        # Nontrivial heading uses native vehicle world SetWorld too.
        h.clip(clips['run']);h.u.mem_write(base+report['state_section_rva'],bytes(4096));h.calls=[]
        for frame in range(90):h.frame(frame*16,frame*.1,frame*.2,quaternion=(.5,-.5,.5,.5))
        assert h.calls
        # Force an armed state, then exercise stale and in-place action changes.
        slot=base+report['state_section_rva']+((h.obj>>4)&63)*64
        h.put(slot+20,15)
        assert h.frame(10000,9.0,17.8,animtick=1500)==0
        assert struct.unpack('<I',h.u.mem_read(slot+20,4))[0]==0,'Same identity must reset after inactivity'
        h.put(slot+20,15);h.put(h.saction,2)
        assert h.frame(10016,9.1,17.8,animtick=1516)==0
        assert struct.unpack('<I',h.u.mem_read(slot+20,4))[0]==0,'Same pointer, new action range resets'
        h.put(slot+20,15);h.put(h.saction+12,h.ani+4)
        assert h.frame(10032,9.2,17.8,animtick=1532)==0
        assert struct.unpack('<I',h.u.mem_read(slot+20,4))[0]==0,'Same pointer, new animation resets'
        h.put(slot+20,15)
        assert h.frame(10048,9.3,17.8,animtick=0)==0
        assert struct.unpack('<I',h.u.mem_read(slot+20,4))[0]==0,'Animation clock restart resets'
        h.put(slot+20,15);bones_before=h.bone_calls
        assert h.frame(10064,9.4,17.8,animtick=16,model_enabled=False)==0
        assert struct.unpack('<I',h.u.mem_read(slot+20,4))[0]==0
        assert h.frame(10080,9.5,17.8,animtick=32,action_enabled=False)==0
        assert h.bone_calls==bones_before,'Absent model/action must never call GetBoneMatrix'
        assert struct.unpack('<I',h.u.mem_read(slot+20,4))[0]==0
        for kind in (0,1,2,3,5):
            assert h.frame(4000,0,0,kind=kind,state=0)==0
            if base==0x400000:
                assert h.frame(4100,1,0,kind=kind,state=1)==1
                assert h.calls[-1]['name']!='mt_hell_hoof'
    return dict(passed=True,actual_native_GetBoneMatrix=True,actual_native_Common_SetWorld=True,
                hidden_return_buffer_and_ret12_checked=True,native_animation_fps=35,
                exact_raw_ANI_used=True,named_bones=[name for name,p in FEET],
                ground_height_call_abi_checked=True,ground_lift=.20,landing=.16,lift=.22,
                descending_landing=True,first_sample_emissions=0,stopped_bone_queries=0,
                teleports_and_identity_reset_checked=True,other_mount_dust_unchanged=True,
                same_pointer_stale_reset_checked=True,in_place_action_change_checked=True,
                animation_clock_restart_checked=True,
                null_model_and_action_suppressed=True,
                low_fps_limitation='10/15 fps are checked for bounded emissions and ABI safety; short contacts can be skipped, no catch-up bursts',
                original_epilogue_stack_registers_and_x87_preserved=True,code_rx_data_rw=True,
                existing_sections_retained=['.htrail','.htdata','.hfire','.mdeath'],
                frame_rate_cases=cases,in_game_visual_test=False,
                binaries={n:hashlib.sha256((R/'client-overlay'/n).read_bytes()).hexdigest() for n in ('Render.dll','Common.dll')})


if __name__=='__main__':
    from patch_hell_horse_hoof import build
    data,report=build();print(json.dumps(verify(data,report),indent=2))
