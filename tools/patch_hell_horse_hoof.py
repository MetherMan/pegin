"""Stage/verify actual animated hoof-contact emission; --apply installs it.

The previous center trail stays present for reversible undo, but is unreachable
for hellhorses. GetBoneMatrix uses the shipped native hidden-return-buffer ABI.
"""
from pathlib import Path
import argparse, hashlib, json, struct, sys
R=Path(__file__).resolve().parents[1]
sys.path[:0]=[str(R/'runtime/pylibs'),str(R/'tools')]
import pefile
from pe_hooks import Hooks, align

PATH=R/'client-overlay/DeicideOnline.exe'
REPORT=R/'assets/hell-horse/hoof-hook.json'
VALIDATION=R/'assets/hell-horse/hoof-hook-validation.json'
STAGED=R/'assets/hell-horse/hoof-hook-staged.json'
ENTRY=0x4223a3
SLOTS=64
FEET=[
    ('Bip02 L Finger01',(.128036912464,-.007704655553,.017997010834)),
    ('Bip02 R Finger01',(.128054758531,.007704499051,.018100216139)),
    ('Bip02 L Toe0',(.123759327335,.003451034783,.040027760792)),
    ('Bip02 R Toe0',(.123718520184,-.003541369337,.040048668851)),
]


def transform(source, matrix, dest):
    """Row-vector Matrix4 convention used by native Vector3::operator*."""
    rows=[]
    for axis in range(3):
        rows.append(f'''
          fld dword ptr [{source}]; fmul dword ptr [ebp+{matrix+axis*4}];
          fld dword ptr [{source}+4]; fmul dword ptr [ebp+{matrix+16+axis*4}];
          faddp st(1),st(0);
          fld dword ptr [{source}+8]; fmul dword ptr [ebp+{matrix+32+axis*4}];
          faddp st(1),st(0); fadd dword ptr [ebp+{matrix+48+axis*4}];
          fstp dword ptr [ebp+{dest+axis*4}];
        ''')
    return ''.join(rows)


def build(path=PATH):
    h=Hooks(path,REPORT,b'.hhoof')
    old=json.loads((R/'assets/hell-horse/fire-trail-hook.json').read_text())
    prior=next(x for x in old['hooks'] if x['va']==ENTRY)
    # Supersede only the known center-emission branch; unrelated hooks survive.
    for x in old['hooks']:
        off=h.pe.get_offset_from_rva(x['target']-h.base)
        assert h.original[off:off+len(bytes.fromhex(x['code']))]==bytes.fromhex(x['code'])
    name=h.rva;h.code.extend(b'mt_hell_hoof\0'.ljust(16,b'\0'))
    min_delta,max_delta,landing,lift,ground=(h.rva+i for i in (16,20,24,28,32))
    h.code.extend(struct.pack('<5f',.000001,64,.16,.22,.20))
    names=[]
    for n,_ in FEET:
        names.append(h.rva+len(h.code));h.code.extend(n.encode()+b'\0')
    h.code.extend(bytes(-len(h.code)%16))
    feet=h.rva+len(h.code)
    for (_,point),n in zip(FEET,names):h.code.extend(struct.pack('<3fI',*point,n))
    state_rva=h.rva+h.pe.OPTIONAL_HEADER.SectionAlignment
    # 64-byte slot: ptr,id,lastSeen,xy,armed,action,spare,previousY[4],
    # actionStart,actionEnd,animationPointer,animationTick.
    # Scratch: bone[0:64],world[64:128],modelPoint[128:140],worldPoint[144:156],
    # actionTick160,footIndex164,wallTick168,step172,footRecord176.
    assembly=f'''
      cmp dword ptr [ebx+0x24],4; jne ordinary;
      pushfd; pushad; sub esp,192; mov ebp,esp;
      call pic; pic: pop esi; sub esi,pic; add esi,{h.base};
      call dword ptr [esi+0x6a100]; mov [ebp+168],eax;
      mov edi,ebx; shr edi,4; and edi,{SLOTS-1}; shl edi,6;
      add edi,esi; add edi,{state_rva};
      lea edx,[esi+{state_rva+SLOTS*64}]; mov ecx,{SLOTS};
      probe: cmp [edi],ebx; je found;
      cmp dword ptr [edi],0; je reset;
      mov eax,[ebp+168]; sub eax,[edi+8]; cmp eax,5000; jae reset;
      add edi,64; cmp edi,edx; jb next_slot; sub edi,{SLOTS*64};
      next_slot: loop probe; jmp reset;
      found:
      mov eax,[ebp+168]; sub eax,[edi+8]; cmp eax,5000; jae reset;
      mov eax,[ebx+0x20]; cmp [edi+4],eax; jne reset;
      mov eax,[ebx+4]; cmp [edi+24],eax; jne reset;
      test eax,eax; jz reset;
      mov edx,[eax]; cmp [edi+48],edx; jne reset;
      mov edx,[eax+4]; cmp [edi+52],edx; jne reset;
      mov edx,[eax+12]; cmp [edi+56],edx; jne reset;
      mov eax,[ebx+0xc]; test eax,eax; jz reset;
      mov eax,[eax+0x18]; cmp eax,[edi+60]; jb reset;
      mov [edi+60],eax;
      fld dword ptr [ebx+0x44]; fsub dword ptr [edi+12]; fmul st(0),st(0);
      fld dword ptr [ebx+0x48]; fsub dword ptr [edi+16]; fmul st(0),st(0);
      faddp st(1),st(0); fstp dword ptr [ebp+172];
      mov eax,[ebp+168]; mov [edi+8],eax;
      mov eax,[ebx+0x44]; mov [edi+12],eax;
      mov eax,[ebx+0x48]; mov [edi+16],eax;
      fld dword ptr [ebp+172]; fcomp dword ptr [esi+{min_delta}];
      fnstsw ax; test ah,0x45; jnz reset;
      fld dword ptr [ebp+172]; fcomp dword ptr [esi+{max_delta}];
      fnstsw ax; test ah,1; jz reset;
      cmp dword ptr [ebx],0; je reset;
      cmp dword ptr [ebx+0x10],0; je reset;
      mov eax,[ebx+0xc]; test eax,eax; jz reset;
      mov eax,[eax+0x18]; mov [ebp+160],eax;
      lea eax,[ebx+0x50]; push eax;
      lea eax,[ebx+0x44]; push eax;
      lea ecx,[ebp+64]; call dword ptr [esi+0x6a05c];
      mov dword ptr [ebp+164],0;
      foot_loop:
      mov eax,[ebp+164]; shl eax,4; add eax,esi; add eax,{feet};
      mov [ebp+176],eax;
      push dword ptr [ebp+160];
      mov eax,[eax+12]; add eax,esi; push eax;
      push ebp;
      mov ecx,[ebx]; mov edx,[ecx]; call dword ptr [edx+0x3c];
      mov edx,[ebp+176];
      {transform('edx',0,128)}
      mov ecx,[ebp+164]; mov eax,[edi+32+ecx*4]; mov [ebp+180],eax;
      mov eax,[ebp+132]; mov [edi+32+ecx*4],eax;
      fld dword ptr [ebp+132]; fcomp dword ptr [esi+{lift}];
      fnstsw ax; test ah,0x45; jz arm;
      bt dword ptr [edi+20],ecx; jnc next_foot;
      fld dword ptr [ebp+132]; fcomp dword ptr [esi+{landing}];
      fnstsw ax; test ah,0x41; jz next_foot;
      test ah,4; jnz next_foot;
      fld dword ptr [ebp+132]; fcomp dword ptr [ebp+180];
      fnstsw ax; test ah,0x41; jz next_foot;
      test ah,4; jnz next_foot;
      btr dword ptr [edi+20],ecx;
      {transform('ebp+128',64,144)}
      lea eax,[ebp+144]; push eax;
      mov ecx,[ebx+0x10]; mov edx,[ecx]; call dword ptr [edx+0xa4];
      fadd dword ptr [esi+{ground}]; fstp dword ptr [ebp+152];
      lea eax,[ebp+144]; push eax;
      lea eax,[esi+{name}]; push eax;
      mov ecx,[ebx+0x10]; mov edx,[ecx]; call dword ptr [edx+0x90];
      jmp next_foot;
      arm: bts dword ptr [edi+20],ecx;
      next_foot: inc dword ptr [ebp+164]; cmp dword ptr [ebp+164],4; jb foot_loop;
      jmp done;
      reset:
      mov [edi],ebx; mov eax,[ebx+0x20]; mov [edi+4],eax;
      mov eax,[ebp+168]; mov [edi+8],eax;
      mov eax,[ebx+0x44]; mov [edi+12],eax;
      mov eax,[ebx+0x48]; mov [edi+16],eax;
      mov dword ptr [edi+20],0;
      mov eax,[ebx+4]; mov [edi+24],eax;
      xor edx,edx; test eax,eax; jz empty_action;
      mov edx,[eax]; mov [edi+48],edx;
      mov edx,[eax+4]; mov [edi+52],edx;
      mov edx,[eax+12]; mov [edi+56],edx; jmp reset_tick;
      empty_action: mov [edi+48],edx; mov [edi+52],edx; mov [edi+56],edx;
      reset_tick: xor edx,edx; mov eax,[ebx+0xc]; test eax,eax; jz empty_tick;
      mov edx,[eax+0x18];
      empty_tick: mov [edi+60],edx;
      mov dword ptr [edi+32],0; mov dword ptr [edi+36],0;
      mov dword ptr [edi+40],0; mov dword ptr [edi+44],0;
      done: add esp,192; popad; popfd; pop edi; pop esi; jmp 0x4223bf;
      ordinary: mov eax,[ebx+0xb8]
    '''
    h.hook(ENTRY,prior['replacement'],assembly,'four actual animated hoof contacts replace center trail')
    output,report=h.finish()
    assert len(h.code)<=h.pe.OPTIONAL_HEADER.SectionAlignment
    p=pefile.PE(data=output);header=p.sections[-1].get_file_offset()+40
    assert header+40<=p.OPTIONAL_HEADER.SizeOfHeaders and h.data[header:header+40]==bytes(40)
    size=SLOTS*64;raw_size=align(size,p.OPTIONAL_HEADER.FileAlignment)
    raw_at=align(len(h.data),p.OPTIONAL_HEADER.FileAlignment)
    h.write(header,struct.pack('<8sIIIIIIHHI',b'.hfdata',size,state_rva,raw_size,raw_at,0,0,0,0,0xc0000040))
    h.write(p.FILE_HEADER.get_field_absolute_offset('NumberOfSections'),struct.pack('<H',p.FILE_HEADER.NumberOfSections+1))
    h.write(p.OPTIONAL_HEADER.get_field_absolute_offset('SizeOfImage'),struct.pack('<I',align(state_rva+size,p.OPTIONAL_HEADER.SectionAlignment)))
    h.write(p.OPTIONAL_HEADER.get_field_absolute_offset('SizeOfInitializedData'),struct.pack('<I',p.OPTIONAL_HEADER.SizeOfInitializedData+raw_size))
    h.data.extend(bytes(raw_at-len(h.data)+raw_size));output=bytes(h.data)
    report.update(sha256=hashlib.sha256(output).hexdigest(),state_section_rva=state_rva,version=3,
                  supersedes='.htrail center emitter; old sections retained for undo',feet=FEET)
    restored=bytearray(output[:len(h.original)])
    for offset,oldbytes in reversed(h.undo):restored[offset:offset+len(bytes.fromhex(oldbytes))]=bytes.fromhex(oldbytes)
    assert bytes(restored)==h.original
    return output,report


def verify(data,report):
    from verify_hell_horse_hoof import verify as run
    return run(data,report)


def main():
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--apply',action='store_true');args=parser.parse_args()
    existing=PATH.read_bytes();pe=pefile.PE(data=existing)
    installed=any(s.Name.rstrip(b'\0')==b'.hhoof' for s in pe.sections)
    if installed:
        output=existing;metadata=json.loads(REPORT.read_text(encoding='utf-8'))
        for hook in metadata['hooks']:
            for va,key in ((hook['va'],'replacement'),(hook['target'],'code')):
                offset=pe.get_offset_from_rva(va-pe.OPTIONAL_HEADER.ImageBase);expected=bytes.fromhex(hook[key])
                assert output[offset:offset+len(expected)]==expected
        if hashlib.sha256(existing).hexdigest()==metadata['sha256']:
            # Hooks restores exactly its recorded input first, preserving all
            # older patches, so a source change can be applied reproducibly.
            output,metadata=build()
        elif metadata.get('version')!=3:
            raise RuntimeError('A later executable patch prevents safe hoof rebuild; retain that patch and rebase explicitly')
    else:output,metadata=build()
    validation=verify(output,metadata);metadata['validation']=validation
    VALIDATION.write_text(json.dumps(validation,indent=2)+'\n',encoding='utf-8')
    changed=output!=existing
    if args.apply and changed:
        assert PATH.read_bytes()==existing,'Executable changed during verification; rerun against latest bytes'
        PATH.write_bytes(output);REPORT.write_text(json.dumps(metadata,indent=2)+'\n',encoding='utf-8')
    elif changed:STAGED.write_text(json.dumps(metadata,indent=2)+'\n',encoding='utf-8')
    print(json.dumps(dict(applied=args.apply and changed,already_installed=installed,validation=validation)))


if __name__=='__main__':main()
