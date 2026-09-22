"""Small, reversible x86 PE hooks with exact-byte guards and relocation checks."""
import hashlib,json,struct
from pathlib import Path
import pefile
from keystone import Ks,KS_ARCH_X86,KS_MODE_32
from capstone import Cs,CS_ARCH_X86,CS_MODE_32

def align(n,a):return (n+a-1)//a*a

class Hooks:
    def __init__(self,path,report,section=b'.stk16'):
        self.section=section
        self.path=Path(path);self.report=Path(report)
        data=self.path.read_bytes()
        if self.report.exists():
            old=json.loads(self.report.read_text())
            if hashlib.sha256(data).hexdigest()==old['sha256']:
                restored=bytearray(data[:old['original_size']])
                for offset,hexdata in reversed(old['undo']):
                    restored[offset:offset+len(bytes.fromhex(hexdata))]=bytes.fromhex(hexdata)
                data=bytes(restored)
                assert hashlib.sha256(data).hexdigest()==old['original_sha256']
        self.original=data;self.data=bytearray(data);self.pe=pefile.PE(data=data)
        assert not any(s.Name.rstrip(b'\0')==section for s in self.pe.sections), 'Unrecognized previous patch'
        self.base=self.pe.OPTIONAL_HEADER.ImageBase
        last=self.pe.sections[-1]
        self.rva=align(last.VirtualAddress+max(last.Misc_VirtualSize,last.SizeOfRawData),self.pe.OPTIONAL_HEADER.SectionAlignment)
        self.raw=align(len(data),self.pe.OPTIONAL_HEADER.FileAlignment)
        self.code=bytearray();self.undo=[];self.entries=[]
        self.ks=Ks(KS_ARCH_X86,KS_MODE_32);self.cs=Cs(CS_ARCH_X86,CS_MODE_32)

    def asm(self,s,va):return bytes(self.ks.asm(s,va)[0])
    def write(self,offset,data):
        self.undo.append((offset,bytes(self.data[offset:offset+len(data)]).hex()))
        self.data[offset:offset+len(data)]=data
    def replace(self,va,old,new,label):
        old=bytes.fromhex(old) if isinstance(old,str) else old
        off=self.pe.get_offset_from_rva(va-self.base)
        assert self.data[off:off+len(old)]==old,(label,hex(va),self.data[off:off+len(old)].hex(),old.hex())
        assert len(old)==len(new)
        self.write(off,new);self.entries.append(dict(label=label,va=va,original=old.hex(),replacement=new.hex()))
    def hook(self,va,old,assembly,label):
        old=bytes.fromhex(old)
        assert len(old)>=5
        ins=list(self.cs.disasm(old,va));assert sum(i.size for i in ins)==len(old)
        assert not any(va<=self.base+e.rva<va+len(old) and e.type for block in getattr(self.pe,'DIRECTORY_ENTRY_BASERELOC',[]) for e in block.entries), 'Hook would move an absolute relocation'
        self.code.extend(b'\x90'*(-len(self.code)%16))
        target=self.base+self.rva+len(self.code)
        code=self.asm(assembly+'; jmp '+hex(va+len(old)),target)
        jump=b'\xe9'+struct.pack('<i',target-va-5)+b'\x90'*(len(old)-5)
        self.replace(va,old,jump,label)
        self.entries[-1].update(target=target,resume=va+len(old),assembly=assembly,code=code.hex())
        self.code.extend(code)

    def finish(self):
        p=self.pe;h=p.sections[-1].get_file_offset()+40
        assert h+40<=p.OPTIONAL_HEADER.SizeOfHeaders
        assert self.data[h:h+40]==bytes(40),'No spare section header'
        raw_size=align(len(self.code),p.OPTIONAL_HEADER.FileAlignment)
        self.write(h,struct.pack('<8sIIIIIIHHI',self.section,len(self.code),self.rva,raw_size,self.raw,0,0,0,0,0x60000020))
        self.write(p.FILE_HEADER.get_field_absolute_offset('NumberOfSections'),struct.pack('<H',p.FILE_HEADER.NumberOfSections+1))
        self.write(p.OPTIONAL_HEADER.get_field_absolute_offset('SizeOfImage'),struct.pack('<I',align(self.rva+len(self.code),p.OPTIONAL_HEADER.SectionAlignment)))
        self.write(p.OPTIONAL_HEADER.get_field_absolute_offset('SizeOfCode'),struct.pack('<I',p.OPTIONAL_HEADER.SizeOfCode+raw_size))
        self.write(p.OPTIONAL_HEADER.get_field_absolute_offset('CheckSum'),bytes(4))
        self.data.extend(bytes(self.raw-len(self.data)))
        self.data.extend(self.code);self.data.extend(bytes(raw_size-len(self.code)))
        result=dict(original_sha256=hashlib.sha256(self.original).hexdigest(),original_size=len(self.original),sha256=hashlib.sha256(self.data).hexdigest(),section_rva=self.rva,undo=self.undo,hooks=self.entries)
        # Keep builds staged until server and client validation both pass.
        return bytes(self.data),result
