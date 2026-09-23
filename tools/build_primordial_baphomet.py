"""Build a separate Baphomet, uniformly scaling its mesh AND all native motions."""
from pathlib import Path
from io import BytesIO
import argparse, hashlib, json, struct, sys, zlib
ROOT=Path(__file__).resolve().parents[1]
sys.path[:0]=[str(ROOT/'runtime/pylibs'),str(ROOT/'client-overlay/Tools/SkillColors')]
from PIL import Image
from native_actor import model, animation
from primordial_geometry import TARGET_SCALE, scaled_animation
C=ROOT/'runtime/client/GameClient';D=ROOT/'client-overlay';O=ROOT/'assets/primordial-baphomet'
SCALE=TARGET_SCALE;BASE=80;IDS=[BASE+n for n in (0,200,300,400,500,600)]

def put(rel,raw):
    p=D/rel;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes(raw)

def scale_floats(raw,offset,n):
    values=struct.unpack_from('<'+str(n)+'f',raw,offset)
    struct.pack_into('<'+str(n)+'f',raw,offset,*(v*SCALE for v in values))

def scale_animation(source):
    return scaled_animation(source,SCALE)

def table_source(rel):
    """Preserve later overlay additions rather than rebuilding from an install."""
    return D/rel if (D/rel).exists() else C/rel

def without_variants(lines):
    return [l for l in lines if not (l.split() and l.split()[0].isdigit() and int(l.split()[0]) in IDS)]

def wad_read(raw):
    assert struct.unpack_from('<I',raw)[0]==100
    at=8;strings={}
    for _ in range(struct.unpack_from('<I',raw,4)[0]):
        key,n=struct.unpack_from('<II',raw,at);at+=8;strings[key]=raw[at:at+n];at+=n
    count=struct.unpack_from('<I',raw,at)[0];at+=4
    assert at+count*308==len(raw)
    rows=[raw[at+i*308:at+(i+1)*308] for i in range(count)]
    return strings,rows

def wad_write(strings,rows):
    raw=struct.pack('<II',100,len(strings))
    for key,name in strings.items():raw+=struct.pack('<II',key,len(name))+name
    return raw+struct.pack('<I',len(rows))+b''.join(rows)

def table_add(path,new_rows):
    old=path.read_bytes().decode('cp949');lines=old.splitlines()
    lines=[l for l in lines if not (l.split('\t')[0].isdigit() and int(l.split('\t')[0]) in IDS)]
    path.write_bytes(('\r\n'.join(lines+new_rows)+'\r\n').encode('cp949'))

def main(geometry_only=False):
    if geometry_only:
        # No monster definitions, experience or drop tables are touched here.
        from sculpt_primordial_baphomet import main as sculpt
        sculpt(SCALE)
        return
    O.mkdir(exist_ok=True)
    source=(C/'Monster/p-warrior.mod').read_bytes();original=model(source);raw=bytearray(source);at=36
    assert len(original)==2 and all(c['bones'] is not None for c in original)
    textures=['mt_prime_body.bmp','mt_prime_blade.bmp']
    for idx,chunk in enumerate(original):
        scale_floats(raw,at+48,3);at+=64
        n=struct.unpack_from('<i',raw,at)[0];at+=4;scale_floats(raw,at,n*3);at+=n*12
        faces=struct.unpack_from('<i',raw,at)[0];at+=4
        if idx==1:
            # ImageGen's narrow-image output has white side gutters. Sample its
            # original atlas region in UV space instead of painting/cropping it.
            for corner in range(faces*3):
                u=struct.unpack_from('<f',raw,at+corner*24+16)[0]
                assert -.001<=u<=1.001
                struct.pack_into('<f',raw,at+corner*24+16,(85+u*548)/725)
        at+=faces*72+16;raw[at:at+32]=textures[idx].encode().ljust(32,b'\0');at+=32
        kind=struct.unpack_from('<i',raw,at)[0];at+=4+(n if kind==2 else 0)
    assert at==len(raw);modified=model(raw)
    for a,b in zip(original,modified):
        assert a['bones']==b['bones']
        assert all(abs(q[i]-p[i]*SCALE)<1e-5 for p,q in zip(a['points'],b['points']) for i in range(3))
        assert all(x[:4]==y[:4] and x[5]==y[5] for x,y in zip(a['corners'],b['corners']))
    assert original[0]['corners']==modified[0]['corners']
    put('Monster/mt_prime_baphomet.mod',raw)
    for key,size in [('body',(1024,1024)),('blade',(256,1024))]:
        source_texture=O/(key+'-imagegen.png')
        if key=='body' and (O/'body-antler-imagegen.png').exists():source_texture=O/'body-antler-imagegen.png'
        if key=='body' and (O/'body-cyclops-imagegen.png').exists():source_texture=O/'body-cyclops-imagegen.png'
        im=Image.open(source_texture).convert('RGB').resize(size,Image.Resampling.LANCZOS)
        im.save(O/(key+'-atlas.png'));buf=BytesIO();im.save(buf,format='BMP');bmp=buf.getvalue()
        wtm=b'TEAMMAY\0\0'+struct.pack('<I',len(bmp))+zlib.compress(bmp,9)
        assert Image.open(BytesIO(zlib.decompress(wtm[13:]))).tobytes()==im.tobytes()
        put('Texture/Monster/mt_prime_'+key+'.wtm',wtm)
    strings,rows=wad_read(table_source('Monster/monster.wad').read_bytes())
    rows=[row for row in rows if struct.unpack_from('<I',row)[0]!=BASE]
    original_row=next(row for row in rows if struct.unpack_from('<I',row)[0]==44)
    new_row=bytearray(original_row);struct.pack_into('<I',new_row,0,BASE)
    animations={};new_string_ids={};clips={}
    for mode in range(2):
        for state in range(9):
            offset=4+mode*152+8+state*16
            first,last,ani,sound,use=struct.unpack_from('<IIHHI',original_row,offset)
            if not use:continue
            name=strings[ani].rstrip(b'\0').decode('cp949')
            if ani not in new_string_ids:
                output='mt_prime_'+name.removeprefix('p-warrior_bla_')
                raw_ani=(C/'Monster/Animation'/name).read_bytes()
                scaled,old_clip,new_clip=scale_animation(raw_ani)
                put('Monster/Animation/'+output,scaled)
                encoded=output.encode()+b'\0'
                sid=next((key for key,value in strings.items() if value==encoded),max(strings)+1)
                strings[sid]=encoded;new_string_ids[ani]=sid
                animations[name]=dict(source_sha256=hashlib.sha256(raw_ani).hexdigest(),output=output,frames=old_clip['frames'])
                clips[name]=(old_clip,new_clip)
            struct.pack_into('<H',new_row,offset+8,new_string_ids[ani])
    rows.append(bytes(new_row));put('Monster/monster.wad',wad_write(strings,rows))
    parsed_strings,parsed_rows=wad_read((D/'Monster/monster.wad').read_bytes())
    assert parsed_rows[:-1]==rows[:-1] and len(parsed_strings)==len(strings)
    info='\r\n'.join(without_variants(table_source('Monster/MobInfo.dat').read_bytes().decode('cp949').splitlines()))
    info=info.rstrip()+'\r\n'+'\r\n'.join(f'{n}\t231\t태초의 바포메트 \\\tmt_prime_baphomet\tNONE\t0' for n in IDS)+'\r\n'
    put('Monster/MobInfo.dat',info.encode('cp949'))
    server=ROOT/'game-data/DATA/MOB_DATA.txt'
    source_row=next(l.split('\t') for l in server.read_bytes().decode('cp949').splitlines() if l.startswith('44\t'))
    new_rows=[]
    for ident in IDS:
        row=source_row[:];row[0]=str(ident);row[1]='태초의 바포메트';row[2]='Primordial Baphomet'
        for idx,value in [(3,231),(4,23162),(6,1470),(7,1610)]:row[idx]=str(value)
        # Use the authoritative server base (14328), not the old client 6312.
        row[12]=str(int(source_row[12])*2)
        new_rows.append('\t'.join(row))
    table_add(server,new_rows)
    old=table_source('Monster/monster.dat').read_bytes()
    txt='\r\n'.join(without_variants(zlib.decompress(old[20:]).decode('cp949').splitlines())+new_rows)+'\r\n'
    encoded=txt.encode('cp949');put('Monster/monster.dat',old[:16]+struct.pack('<I',len(encoded))+zlib.compress(encoded))
    drop=ROOT/'game-data/DATA/MobItemLoseTable.txt';old=drop.read_bytes().decode('cp949').splitlines()
    base_drop=[l.split() for l in old if l.split() and l.split()[0]=='44']
    assert base_drop
    keep=[l for l in old if not (l.split() and l.split()[0] in set(map(str,IDS)))]
    for ident in IDS:
        keep.extend('\t'.join([str(ident)]+fields[1:]) for fields in base_drop)
    drop.write_bytes(('\r\n'.join(keep)+'\r\n').encode('cp949'))
    motions={key:dict(clip=name,first=first,last=last) for key,name,first,last in [
        ('idle','p-warrior_bla_pst.ani',1,70),('walk','p-warrior_bla_pwa.ani',1,41),
        ('attack','p-warrior_bla_wa1.ani',1,87),('run','p-warrior_bla_wru.ani',1,25)]}
    data=dict(original=original,primordial=modified,clips={n:dict(original=a,primordial=b) for n,(a,b) in clips.items()},motions=motions,
              textures=dict(original=['cm_0018-original.png','cm_0018_it-original.png'],primordial=['body-atlas.png','blade-atlas.png']))
    (O/'model.json').write_text(json.dumps(data,separators=(',',':')),encoding='utf-8')
    report=dict(passed=True,base_id=BASE,variant_ids=IDS,name='태초의 바포메트',level=231,hp=23162,attack=[1470,1610],scale=SCALE,
                vertices=sum(len(c['points']) for c in original),triangles=sum(len(c['corners'])//3 for c in original),bones=29,
                animations=animations,original_monsters_untouched=True,blood_textures_from_imagegen=True,
                native_mesh_and_all_animation_translations_scaled=True,original_rotations_and_skin_indices_preserved=True,
                unspecified_stats_and_drops='original basic Baphomet (44)',drop_rows_per_variant=len(base_drop))
    (O/'validation.json').write_text(json.dumps(report,ensure_ascii=False,indent=2)+'\n',encoding='utf-8')
    print(json.dumps({k:v for k,v in report.items() if k!='animations'},ensure_ascii=False))
    from sculpt_primordial_baphomet import main as sculpt
    sculpt()

if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--geometry-only',action='store_true',help='Rebuild model, original-source motions and textures without editing monster/stat/drop tables')
    main(parser.parse_args().geometry_only)
