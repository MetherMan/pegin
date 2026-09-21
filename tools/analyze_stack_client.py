"""Map the shipped packet dispatch table to source names; write local disassembly."""
from pathlib import Path
import json,re,sys
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'runtime/pylibs'))
import pefile,capstone

def packet_functions():
    p=pefile.PE(str(ROOT/'client-overlay/DeicideOnline.exe'))
    md=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32);md.detail=True
    defs=dict((name,int(number)) for name,number in re.findall(r'#define\s+(dPACKET_\w+)\s+(\d+)',(ROOT/'src/server/LAQIA_GameServer/message.h').read_text(encoding='cp949')))
    names={defs[tag]:name for tag,name in re.findall(r'SET_FUNC\(\s*(\w+)\s*,\s*(\w+)\s*\)',(ROOT/'src/client/gameclient/Src/Game.cpp').read_text(encoding='latin1')) if tag in defs}
    result={};regs={}
    for i in md.disasm(p.get_data(0x24130,0x6b0),0x424130):
        if i.mnemonic!='mov' or len(i.operands)!=2:continue
        dst,src=i.operands
        value=src.imm if src.type==capstone.CS_OP_IMM else regs.get(src.reg) if src.type==capstone.CS_OP_REG else None
        if dst.type==capstone.CS_OP_REG:regs[dst.reg]=value
        if dst.type==capstone.CS_OP_MEM and i.reg_name(dst.mem.base)=='edx' and value:
            tag=(dst.mem.disp-0x1094)//4
            if tag in names:result[names[tag]]=dict(tag=tag,va=value)
    return result

if __name__=='__main__':
    mapping=packet_functions();out=ROOT/'runtime/stack-analysis';out.mkdir(exist_ok=True)
    (out/'packet-functions.json').write_text(json.dumps(mapping,indent=2)+'\n')
    p=pefile.PE(str(ROOT/'client-overlay/DeicideOnline.exe'))
    md=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32)
    starts=sorted(set(x['va'] for x in mapping.values()))
    for name,entry in mapping.items():
        if not any(s in name for s in ('Inventory','ItemPlayerItemInfo','Cnt','Garbage','SerInven','MyShopItemList')):continue
        start=entry['va'];end=next((n for n in starts if n>start),start+0x300)
        lines=[hex(i.address)+' '+i.bytes.hex()+' '+i.mnemonic+' '+i.op_str for i in md.disasm(p.get_data(start-0x400000,end-start),start)]
        (out/(name+'.txt')).write_text('\n'.join(lines))
        print(name,entry)
