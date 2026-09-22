from pathlib import Path
import sys,struct
R=Path(__file__).resolve().parents[1];sys.path.insert(0,str(R/'runtime/pylibs'))
import pefile
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
cs=Cs(CS_ARCH_X86,CS_MODE_32)
def dump(p,start,size):
 for i in cs.disasm(p.get_data(start-p.OPTIONAL_HEADER.ImageBase,size),start):print(hex(i.address),i.bytes.hex(),i.mnemonic,i.op_str)
if len(sys.argv)>2:
 p=pefile.PE(str(R/'client-overlay'/sys.argv[1]));dump(p,int(sys.argv[2],16),int(sys.argv[3],16));sys.exit()
p=pefile.PE(str(R/'client-overlay/UInterface.dll'));base=p.OPTIONAL_HEADER.ImageBase
b=p.get_memory_mapped_image();va=base+b.index(b'CHK_INVEN_TAB\0')
print('string',hex(va))
text=p.sections[0].get_data();start=base+p.sections[0].VirtualAddress
for off in range(len(text)-4):
 if text[off:off+4]==struct.pack('<I',va):print('ref',hex(start+off));dump(p,start+off-33,170)
