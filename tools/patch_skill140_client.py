"""Guarded, additive card-drag and weapon-aware Sixfold Shot hooks."""
from pathlib import Path
import sys,json,struct
R=Path(__file__).resolve().parents[1];O=R/'assets/skills140';O.mkdir(exist_ok=True)
sys.path[:0]=[str(R/'.cache/weapon-build-deps'),str(R/'runtime/pylibs'),str(R/'tools')]
from pe_hooks import Hooks
from patch_stack_client import machine
from unicorn import UC_HOOK_CODE
from unicorn.x86_const import *

def patch_ui():
 p=R/'client-overlay/UInterface.dll';report=O/'card-drag-hook.json';h=Hooks(p,report,b'.card')
 h.hook(0x1002140c,'85ff741e6a00',
  'test edi,edi; jz 0x1002142e; mov eax,[ebp+4]; cmp eax,10193; je card; cmp eax,10194; jne enchant; '
  'card: cmp eax,[edi+4]; je 0x1002142e; enchant: push 0','merge matching enchant cards before enchant selection')
 data,result=h.finish();cases=[]
 for base in [0x10000000,0x3500000]:
  delta=base-0x10000000
  for source,target,want in [(10193,10193,'move'),(10194,10194,'move'),(10193,10194,'enchant'),(10193,1,'enchant'),(10194,3970,'enchant'),(10095,10095,'move'),(10193,0,'move')]:
   u,_=machine(data,base);ui,inv,src,dst,sp,stop=0x2001000,0x2002000,0x2003000,0x2004000,0x201f000,0x201d000
   u.mem_write(src,struct.pack('<II',27,source));u.mem_write(dst,struct.pack('<II',28,target))
   u.mem_write(ui+0x1f0,struct.pack('<II',2,3));u.mem_write(sp,struct.pack('<III',stop,src,inv))
   u.reg_write(UC_X86_REG_ECX,ui);events=[]
   stubs={0x10007970:8,0x10025af0:8,0x100260d0:4,0x10007870:8,0x1001ec60:4,0x100330c0:0}
   for va,args in stubs.items():u.mem_write(va+delta,b'\xc2'+struct.pack('<H',args))
   def intercept(uc,address,size,_):
    va=address-delta
    if va not in stubs:return
    if va==0x10007970:uc.reg_write(UC_X86_REG_EAX,dst if target else 0)
    elif va==0x10025af0:uc.reg_write(UC_X86_REG_EAX,int(source in [10193,10194]))
    elif va==0x100260d0:events.append('enchant')
    elif va in [0x10007870,0x1001ec60]:uc.reg_write(UC_X86_REG_EAX,1)
    elif va==0x100330c0:
     pos=uc.reg_read(UC_X86_REG_ESP);args=struct.unpack('<4I',uc.mem_read(pos+4,16));assert args[1:3]==(303,27);events.append('move')
   u.hook_add(UC_HOOK_CODE,intercept);u.emu_start(0x100213b0+delta,stop,count=1000)
   assert events==[want],(source,target,events);assert u.reg_read(UC_X86_REG_ESP)==sp+12
   cases.append([hex(base),source,target,want])
 result.update(passed=True,full_drag_function_cases=cases);p.write_bytes(data);report.write_text(json.dumps(result,indent=2))
 # Source equivalent, restricted to the active implementation (not its old comment).
 p=R/'src/client/uinterface/Src/W3DGUIInput.cpp';s=p.read_bytes().decode('cp949')
 start=s.index('Assert( pItemDat );',s.index('void CW3DUInterface::MoveItemInInventory'))
 a=s[:start];b=s[start:];old='if( SelectedEnchantItem( pItemDat, FALSE ) )'
 new='if( !((pItemDat->nType == 10193 || pItemDat->nType == 10194) && pItemDat->nType == pExistItemDat->nType) && SelectedEnchantItem( pItemDat, FALSE ) )'
 if old in b:b=b.replace(old,new,1)
 assert new in b;p.write_bytes((a+b).encode('cp949'));print('Card drag: 14 complete function cases passed')

def patch_sixfold():
 p=R/'client-overlay/DeicideOnline.exe';report=O/'sixfold-hook.json';h=Hooks(p,report,b'.s140')
 name=h.base+h.rva;h.code.extend(b'mt_six_blue\0')
 h.hook(0x41028c,'8b8e7001000083c043',
  f'pushfd; cmp word ptr [eax],19121; jne normal; mov ecx,[esi+0x300]; test ecx,ecx; jz normal; '
  f'cmp dword ptr [ecx],0x615f746d; jne normal; cmp dword ptr [ecx+4],0x776f7272; jne normal; '
  f'mov eax,{name}; jmp done; normal: add eax,0x43; done: popfd; mov ecx,[esi+0x170]','Sixfold Shot selects equipped Twilight arrow color')
 data,result=h.finish();cases=0
 for item in [12044,19120,19121,19122]:
  for arrow in [b'mt_arrow\0',b'other\0\0\0\0']:
   u,_=machine(data);obj,info,buf,sp=0x2001000,0x2002000,0x2003000,0x201f000
   u.mem_write(info,struct.pack('<H',item));u.mem_write(obj+0x300,struct.pack('<I',buf));u.mem_write(buf,arrow);u.mem_write(obj+0x170,struct.pack('<I',1))
   u.reg_write(UC_X86_REG_ESI,obj);u.reg_write(UC_X86_REG_EAX,info);u.emu_start(0x41028c,0x410295,count=50)
   assert u.reg_read(UC_X86_REG_EAX)==(name if item==19121 and arrow.startswith(b'mt_arrow') else info+0x43)
   assert u.reg_read(UC_X86_REG_ECX)==1 and u.reg_read(UC_X86_REG_ESP)==sp;cases+=1
 result.update(passed=True,selector_cases=cases);p.write_bytes(data);report.write_text(json.dumps(result,indent=2))
 p=R/'src/client/gameclient/Src/Character.cpp';s=p.read_bytes().decode('cp949');old='char*\t\tszSkillName = pItemInfoDat->szEName;'
 new='const char* szSkillName = (wSkillId == 19121 && m_sArrow == "mt_arrow") ? "mt_six_blue" : pItemInfoDat->szEName;'
 if old in s:s=s.replace(old,new)
 assert new in s;p.write_bytes(s.encode('cp949'));print('Sixfold selector: 8 cases passed')

if __name__=='__main__':patch_ui();patch_sixfold()
