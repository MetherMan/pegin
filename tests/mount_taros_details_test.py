"""Read back shipped resources and execute the mounted footstep update path."""
from pathlib import Path
from io import BytesIO
import hashlib,json,re,struct,sys,zlib
ROOT=Path(__file__).resolve().parents[1]
sys.path[:0]=[str(ROOT/'runtime/pylibs'),str(ROOT/'tools')]
from PIL import Image
from patch_stack_client import machine
from unicorn import UC_HOOK_CODE
from unicorn.x86_const import *
D=ROOT/'client-overlay';data=(D/'DeicideOnline.exe').read_bytes()
wad=(D/'Vehicle/vehicle.wad').read_bytes();at=8;names={}
for _ in range(struct.unpack_from('<I',wad,4)[0]):
    ident,n=struct.unpack_from('<II',wad,at);at+=8;names[ident]=wad[at:at+n].split(b'\0')[0].decode();at+=n
cases=[]
for sex in range(2):
    count=struct.unpack_from('<I',wad,at)[0];at+=4
    for weapon in range(count):
        for mode in range(10):
            for state in range(50):
                for phase in range(3):
                    first,last,ani,wp,sound=struct.unpack_from('<IIHHH',wad,at);at+=16
                    if mode==1 and ani and last>first and (state,phase) in [(1,0),(2,1),(2,2)]:
                        assert sound in names and (D/'Sound'/names[sound]).exists()
                        if phase!=2:cases.append((sex,weapon,state,phase,sound))
assert at==len(wad) and len(cases)==8
checks=[]
for sex,weapon,state,phase,sound in cases:
    for elapsed in [100,650]:
        u,_=machine(data);obj=0x2000000;model=0x2004000;motion=0x2005000;audio=0x2006000;vt=0x2007000
        get_ticks=0x201a000;play=0x201a100;filename=0x201b000;stop=0x201d000;sp=0x201f000
        u.mem_write(obj+0xc,struct.pack('<I',motion));u.mem_write(obj+0x20,struct.pack('<I',model))
        u.mem_write(obj+0x2c,struct.pack('<HH',state,phase));u.mem_write(obj+0x4c,struct.pack('<I',audio))
        u.mem_write(motion+0x14,struct.pack('<H',sound));u.mem_write(model,struct.pack('<I',vt))
        u.mem_write(audio,struct.pack('<I',vt));u.mem_write(vt+0x34,struct.pack('<I',get_ticks));u.mem_write(vt+8,struct.pack('<I',play))
        u.mem_write(get_ticks,b'\xb8'+struct.pack('<I',600)+b'\xc3');u.mem_write(play,b'\xc2\x0c\x00')
        u.mem_write(filename,names[sound].encode()+b'\0')
        u.mem_write(0x4030d0,b'\xb8'+struct.pack('<I',filename)+b'\xc2\x04\x00')
        u.mem_write(sp,struct.pack('<II',stop,elapsed));u.reg_write(UC_X86_REG_ECX,obj);heard=[]
        def capture(mu,pc,size,unused):
            if pc==0x4030d0:
                ident=struct.unpack('<I',bytes(mu.mem_read(mu.reg_read(UC_X86_REG_ESP)+4,4)))[0]
                assert ident==sound
            if pc==play:
                ptr,loop,distance=struct.unpack('<III',bytes(mu.mem_read(mu.reg_read(UC_X86_REG_ESP)+4,12)))
                heard.append(bytes(mu.mem_read(ptr,40)).split(b'\0')[0].decode())
                assert loop==1 and distance==30
        u.hook_add(UC_HOOK_CODE,capture);u.emu_start(0x408540,stop,count=4000)
        assert heard==([names[sound]] if elapsed>=600 else []),(sex,weapon,state,phase,elapsed,heard)
        assert u.reg_read(UC_X86_REG_ESP)==sp+8
        checks.append(dict(sex=sex,weapon=weapon,state=state,phase=phase,elapsed=elapsed,play3d=heard))

def read_wtm(path):
    wrapped=path.read_bytes();raw=zlib.decompress(wrapped[13:]);assert len(raw)==struct.unpack_from('<I',wrapped,9)[0]
    return Image.open(BytesIO(raw))
for folder in ['Item','Texture/Body']:
    im=read_wtm(D/folder/'mt_hellhorse_icon.wtm');assert im.size==(28,28)
    assert im.tobytes()==Image.open(ROOT/'assets/hell-horse/certificate-icon.png').tobytes()
for width,height in [(800,600),(1024,768)]:assert read_wtm(D/'Map'/f'loading0041_{width:04d}.wtm').size==(width,height)
title=read_wtm(D/'Map/title0041.wtm');assert title.mode=='RGBA' and title.size==(324,131) and title.getchannel('A').getextrema()[0]==0
plain=(D/'Item/ITEM.dat').read_bytes().decode('cp949');ui=zlib.decompress((D/'Interface/item.dat').read_bytes()[20:]).decode('cp949')
for text,columns in [(plain,[58,64]),(ui,[59,65])]:
    rows=[line.split('\t') for line in text.splitlines() if line.startswith('19130\t')];assert len(rows)==1
    assert all(rows[0][c]=='mt_hellhorse_icon.bmp' for c in columns)
table=(D/'Light/table.scr').read_bytes().decode('cp949')
blocks=re.findall(r'\[MAP\]\s+(\d+)\s+(.*?)\[/MAP\]',table,re.S)
night=[body for ident,body in blocks if ident=='41'];assert len(night)==1 and re.search(r'\[ALL\]\s+23\b',night[0])
assert any(ident=='1004' and re.search(r'\[ALL\]\s+0\b',body) for ident,body in blocks)
result=dict(native_footstep_calls=checks,resource_readback_passed=True,night_table_passed=True,exe_sha256=hashlib.sha256(data).hexdigest(),actual_game_audio_visual_tested=False)
(ROOT/'assets/hell-horse/details-native-validation.json').write_text(json.dumps(result,indent=2)+'\n',encoding='utf-8')
print('PASS: 16 native footstep update cases, both icon tables, native images and permanent-night table')
