"""Execute native hoof-particle Update and SetWorld at every living millisecond.

Reads the current overlay and tests the generator's proposed row in memory.
Writes only its JSON validation report; never deploys a table, DLL or executable.
"""
from pathlib import Path
import hashlib
import json
import math
import struct
import sys

R = Path(__file__).resolve().parents[1]
sys.path[:0] = [str(R/'runtime/pylibs'),str(R/'tools'),str(R/'client-overlay/Tools/SkillColors')]
import pefile
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
from build_hell_horse_sparks import build_table,records,name,verify_table
from patch_hell_particle_fog import build

REPORT = R/'assets/hell-horse/hoof-particle-native-validation.json'


def native_case(current,base,row):
    fields = struct.unpack_from('<4i5f2i',row,125)
    keys = [struct.unpack_from('<7f',row,425+i*28) for i in range(12)]
    assert fields[2]==140041 and fields[3]==385 and fields[-1]==12
    u = Uc(UC_ARCH_X86,UC_MODE_32)
    render,common = pefile.PE(data=current),pefile.PE(str(R/'client-overlay/Common.dll'))
    common_base = 0x22000000
    for pe,address in ((render,base),(common,common_base)):
        pe.relocate_image(address)
        u.mem_map(address,(pe.OPTIONAL_HEADER.SizeOfImage+4095)&~4095)
        u.mem_write(address,pe.get_memory_mapped_image())
    exports = {entry.name:common_base+entry.address for entry in common.DIRECTORY_ENTRY_EXPORT.symbols}
    for descriptor in render.DIRECTORY_ENTRY_IMPORT:
        if descriptor.dll.lower()==b'common.dll':
            for entry in descriptor.imports:
                if entry.name in exports:
                    u.mem_write(entry.address,struct.pack('<I',exports[entry.name]))
    u.mem_map(0x30000000,0x10000)
    u.mem_map(0x40000000,0x10000)
    obj,head,node,particle,header = [0x30000100+i*0x100 for i in range(5)]
    stop,stack = 0x3000f000,0x40008000
    def put(address,value): u.mem_write(address,struct.pack('<I',value))
    def get(address): return struct.unpack('<I',u.mem_read(address,4))[0]
    def floats(address,values): u.mem_write(address,struct.pack('<'+'f'*len(values),*values))
    # Skip only static atexit registration. Update and native arithmetic execute.
    u.mem_write(base+0x89148,b'\x01')
    u.mem_write(base+0x8916c,b'\x01')
    put(base+0x73088,0x30008000)  # Substitute Frustum::CheckPoint, always visible.
    put(header+4,fields[2]); put(header+8,fields[1]); put(header+12,fields[3])
    put(header+44,fields[-1]); put(header+172,fields[9])
    floats(header+20,[fields[4]]); floats(header+28,fields[5:8]); floats(header+40,[fields[8]])
    # Reproduce LoadParticleTypeFile's serialized-float to BYTE assignments.
    for i,key in enumerate(keys):
        for channel in range(3):
            u.mem_write(header+48+channel*12+i,bytes([int(key[channel])]))
            u.mem_write(header+84+channel*12+i,bytes([int(key[3+channel])]))
        floats(header+120+i*4,[key[6]])
    device,vtable = 0x30000900,0x30000a00
    put(base+0x89328,device); put(device,vtable); put(vtable+0x94,0x30008010)
    floats(base+0x89214,[1 if i in (0,5,10,15) else 0 for i in range(16)])
    freed,matrices = [],[]
    def ret(argc,value=0):
        sp = u.reg_read(UC_X86_REG_ESP)
        target = get(sp)
        u.reg_write(UC_X86_REG_ESP,sp+4*(argc+1))
        u.reg_write(UC_X86_REG_EAX,value); u.reg_write(UC_X86_REG_EIP,target)
    def service(u,address,size,_):
        if address==0x30008000:
            ret(1,1)
        elif address==0x30008010:
            sp = u.reg_read(UC_X86_REG_ESP)
            assert get(sp+4)==device and get(sp+8)==256
            matrices.append(struct.unpack('<16f',u.mem_read(get(sp+12),64)))
            ret(3)
        elif address==base+0x62270:
            freed.append(get(u.reg_read(UC_X86_REG_ESP)+4)); ret(0)
    u.hook_add(UC_HOOK_CODE,service)
    colors,widths,world_checks = [],[],[]
    for age in range(386):
        put(obj+0x1c,head); put(obj+0x20,1)
        put(head,node); put(head+4,node); put(node,head); put(node+4,head); put(node+8,particle)
        put(obj+0x38,0); put(particle,header); put(particle+4,385)
        floats(particle+8,[1.2,3.4,.15]); floats(particle+0x20,[-3,2,1]); floats(particle+0x2c,[0])
        u.mem_write(stack,struct.pack('<II',stop,age))
        u.reg_write(UC_X86_REG_ESP,stack); u.reg_write(UC_X86_REG_ECX,obj)
        u.emu_start(base+0x1c050,stop,count=10000)
        assert u.reg_read(UC_X86_REG_EIP)==stop and u.reg_read(UC_X86_REG_ESP)==stack+8
        if age==385:
            assert get(obj+0x20)==0 and get(head)==head and freed==[node]
            continue
        assert get(obj+0x20)==1 and get(particle+4)==385-age
        assert bytes(u.mem_read(particle+8,12))==bytes(u.mem_read(particle+0x14,12))
        assert get(particle+0x34)==0xff000000
        color = get(particle+0x30)
        colors.append([(color>>16)&255,(color>>8)&255,color&255])
        key,ratio = age//35,(age%35)/35
        expected = keys[key][6]+(keys[key+1][6]-keys[key][6])*ratio
        size = struct.unpack('<f',u.mem_read(particle+0x38,4))[0]
        assert math.isclose(size,expected,abs_tol=1e-7)
        width = .06/size
        assert .24-1e-6<=width<=.264+1e-6
        widths.append(width)
        assert get(base+0x89150)==key and 0<=key<=10
        if age in (0,192,384):
            size_bits = struct.unpack('<I',struct.pack('<f',size))[0]
            u.mem_write(stack,struct.pack('<III',stop,particle+0x14,size_bits))
            u.reg_write(UC_X86_REG_ESP,stack); u.reg_write(UC_X86_REG_ECX,obj)
            u.emu_start(base+0x1c4c0,stop,count=10000)
            assert u.reg_read(UC_X86_REG_EIP)==stop and u.reg_read(UC_X86_REG_ESP)==stack+12
            matrix = matrices[-1]
            assert math.isclose(matrix[15],size,abs_tol=1e-7)
            assert all(math.isclose(matrix[12+i]/matrix[15],value,abs_tol=1e-6)
                       for i,value in enumerate((1.2,3.4,.15)))
            assert math.isclose(.06/matrix[15],width,abs_tol=1e-7)
            world_checks.append(age)
    assert all(all(a[i]>=b[i] for i in range(3)) for a,b in zip(colors,colors[1:]))
    return dict(base=hex(base),ages_checked=386,first_age_ms=0,last_age_ms=385,
                alive_ages_checked=385,erased_at_ms=385,freed_list_nodes=1,
                main_rgb_first=colors[0],main_rgb_last_alive=colors[-1],
                monotonic_rgb_fade=True,secondary_argb_always='0xff000000',
                width_min=min(widths),width_max=max(widths),position_unchanged_every_age=True,
                key_indices_min=0,key_indices_max=10,native_world_transform_ages_ms=world_checks,
                native_world_width_and_position_correct=True,stack_balanced=True)


def main():
    raw = (R/'client-overlay/Effect/particle.ptc').read_bytes()
    proposed = build_table(raw)
    table = verify_table(raw,proposed)
    row = next(record for record in records(proposed) if name(record)=='mt_hell_hoof')
    patched,patch_report,original = build()
    current = (R/'client-overlay/Render.dll').read_bytes()
    assert patched==current, 'Install the validated Render fog patch before this test'
    old_pe,new_pe = pefile.PE(data=original),pefile.PE(data=current)
    old_ice = next(section for section in old_pe.sections if section.Name.rstrip(b'\0')==b'.icealp')
    new_ice = next(section for section in new_pe.sections if section.Name.rstrip(b'\0')==b'.icealp')
    assert old_ice.get_data()==new_ice.get_data() and old_ice.VirtualAddress==new_ice.VirtualAddress
    tests = [native_case(current,base,row) for base in (0x10000000,0x16000000)]
    report = dict(passed=True,recorded_from='tools/verify_hell_hoof_particle.py',
                  render_sha256=hashlib.sha256(current).hexdigest(),
                  tested_row_sha256=hashlib.sha256(row).hexdigest(),
                  deployed_table_matches_generator=raw==proposed,
                  particle_table_validation=table,tests=tests,
                  icealp_section_bytes_preserved=old_ice.SizeOfRawData,icealp_rva_preserved=True,
                  actual_native_functions=['CW3DParticles::Update','CW3DParticles::SetWorld','Common Matrix4::GetBillboard','native float-to-integer conversion'],
                  substituted_services=['Frustum::CheckPoint always visible','D3D SetTransform records matrix','list-node allocator free'],
                  limits='Generator row is tested in memory. Resource loader BYTE conversion is reproduced, not executed. No in-game visual or FPS test.',
                  overlay_written=False,in_game_visual_test=False)
    REPORT.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
    print('PASS: native Update ages 0..385 at two bases, fade/size/position/cleanup, native SetWorld and preserved .icealp')


if __name__ == '__main__': main()
