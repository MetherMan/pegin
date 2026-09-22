"""Position-independent per-item tooltip colors; emulate normal and relocated loads."""
from pathlib import Path
import sys,struct,json,hashlib
O=Path(__file__).resolve().parent;R=O.parents[1]/'runtime';sys.path.insert(0,str(R/'pylibs'))
import pefile
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32
from unicorn.x86_const import *
target=R/'client/GameClient/Engine.dll';backup=R/'backups/before-twilight/client/GameClient/Engine.dll';backup.parent.mkdir(parents=True,exist_ok=True)
if not backup.exists():backup.write_bytes(target.read_bytes())
b=bytearray(backup.read_bytes());p=pefile.PE(data=bytes(b));cave=0x3cc00;cur=cave
assert b[p.get_offset_from_rva(cave):p.get_offset_from_rva(cave)+512]==bytes(512)
patches=[];removed=[]
def put(rva,data):off=p.get_offset_from_rva(rva);b[off:off+len(data)]=data
def jump(src,dst):return b'\xe9'+struct.pack('<i',dst-src-5)
def custom_test():return b'\x66\x81\x3f'+struct.pack('<H',19030)+b'\x72\x07\x66\x81\x3f'+struct.pack('<H',19110)+b'\x76'
# First hook: choose Rank color for this family, leave original switch for every other item.
start=cur;code=bytearray(custom_test()+b'\x0e')
code+=bytes.fromhex('33c0668b8784000000');code+=jump(start+len(code),0x23cd5)
code+=b'\xe8\0\0\0\0\x58';pc=start+len(code)-1
code+=b'\xc7\x80'+struct.pack('<i',0x45e60-pc)+struct.pack('<I',0xff72cfff)
code+=b'\xb8\x08\0\0\0';code+=jump(start+len(code),0x23d28)
assert bytes(b[p.get_offset_from_rva(0x23ccc):p.get_offset_from_rva(0x23ccc)+9])==bytes.fromhex('33c0668b8784000000')
put(start,code);put(0x23ccc,jump(0x23ccc,start)+b'\x90'*4);patches.append((0x23ccc,0x23d28));cur+=len(code)+16
# Name draw loads (base item uses EAX, enhanced item uses ECX).
for hook,reg,original in [(0x23e5e,'eax',bytes.fromhex('a1605e0410')),(0x23f54,'ecx',bytes.fromhex('8b0d605e0410'))]:
 assert p.get_data(hook,len(original))==original
 start=cur;code=bytearray(b'\x9c') # preserve flags; original MOV does not alter them
 code+=custom_test()+b'\x11' # jump over original load and its jump
 code+=b'\xe8\0\0\0\0'+(b'\x58' if reg=='eax' else b'\x59');pc=start+len(code)-1
 code+=(b'\x8b\x80' if reg=='eax' else b'\x8b\x89')+struct.pack('<i',0x45e60-pc)
 code+=b'\xe9\x05\0\0\0'
 code+=(b'\xb8' if reg=='eax' else b'\xb9')+struct.pack('<I',0xff72cfff)
 code+=b'\x9d';code+=jump(start+len(code),hook+len(original))
 put(start,code);put(hook,jump(hook,start)+b'\x90'*(len(original)-5));patches.append((hook,hook+len(original)));cur+=len(code)+16
 removed.append(hook+(1 if reg=='eax' else 2))
# These two absolute-address MOVs became relative jumps; discard their old relocations.
for block in p.DIRECTORY_ENTRY_BASERELOC:
 for e in block.entries:
  if e.rva in removed:struct.pack_into('<H',b,e.struct.get_file_offset(),e.struct.Data&0xfff)
struct.pack_into('<I',b,p.sections[0].get_file_offset()+8,p.sections[0].SizeOfRawData)
assert cur<cave+512
cases=0
for base in [0x10000000,0x03500000]:
 image=pefile.PE(data=bytes(b));image.relocate_image(base);mapped=image.get_memory_mapped_image()
 for rank in range(1,21):
  for item in [11445,19000,19029,19030,19040,19110,19111]:
   default=[0xffc2c2c2,0xfffcff00,0xff30ff00,0xff00ffea,0xfffe7e7e][rank-1] if rank<=5 else 0xffff0000
   color=0xff72cfff if 19030<=item<=19110 else default
   u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(base,0x100000);u.mem_write(base,mapped);u.mem_map(0x02000000,0x2000);u.reg_write(UC_X86_REG_EDI,0x02000000);u.reg_write(UC_X86_REG_ESP,0x02001f00)
   u.mem_write(0x02000000,struct.pack('<H',item));u.mem_write(0x02000084,struct.pack('<H',rank))
   u.emu_start(base+0x23ccc,base+0x23d28,count=100);assert struct.unpack('<I',u.mem_read(base+0x45e60,4))[0]==color,(rank,item,hex(base))
   for (hook,end),reg in zip(patches[1:],[UC_X86_REG_EAX,UC_X86_REG_ECX]):
    flags=u.reg_read(UC_X86_REG_EFLAGS);stack=u.reg_read(UC_X86_REG_ESP)
    u.emu_start(base+hook,base+end,count=100);assert u.reg_read(reg)==(0xff72cfff if 19030<=item<=19110 else color)
    assert u.reg_read(UC_X86_REG_ESP)==stack and u.reg_read(UC_X86_REG_EFLAGS)==flags
   cases+=1
(O/'payload/Engine.dll').write_bytes(b)
(O/'tooltip_validation.json').write_text(json.dumps(dict(passed=True,cases=cases,rank_color='#72CFFF',name_color='#72CFFF',item_range=[19030,19110],two_image_bases=True,sha256=hashlib.sha256(b).hexdigest()),indent=2))
print('PASS',cases,'item/rank/relocation combinations, both name paths, stack and flags preserved.')
