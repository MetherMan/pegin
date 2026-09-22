"""Character MOD/ANI readers matching W3DModel/Animation and character.wad.

Native one-bone indexed vertex skinning; no invented motion or hierarchy.
Only the selected native frame ranges are returned to the browser.
"""
from pathlib import Path
import base64,struct

def model(raw):
    count=struct.unpack_from('<i',raw,32)[0];at=36;chunks=[]
    assert 0<count<100
    for _ in range(count):
        matrix=list(struct.unpack_from('<16f',raw,at));at+=64
        n=struct.unpack_from('<i',raw,at)[0];at+=4
        points=list(struct.iter_unpack('<3f',raw[at:at+n*12]));at+=n*12
        faces=struct.unpack_from('<i',raw,at)[0];at+=4
        corners=list(struct.iter_unpack('<i5f',raw[at:at+faces*72]));at+=faces*72+16
        texture=raw[at:at+32].split(b'\0')[0].decode('cp949');at+=32
        kind=struct.unpack_from('<i',raw,at)[0];at+=4
        # NONANIMATION=0, NONBIPED=1, BIPED=2.
        bones=list(raw[at:at+n]) if kind==2 else None
        if bones is not None:at+=n
        assert all(0<=c[0]<n for c in corners)
        chunks.append(dict(matrix=matrix,points=points,corners=corners,bones=bones,texture=Path(texture).with_suffix('.wtm').name))
    assert at==len(raw),(at,len(raw))
    return chunks

def animation(raw,frames):
    count,tm=struct.unpack_from('<2i',raw,32);at=40+tm*(4+count*28)
    n=struct.unpack_from('<i',raw,at)[0];at+=4
    assert 0<n<100 and all(0<=i<count for i in frames)
    bones=[]
    for i in range(n):
        name=raw[at:at+32].split(b'\0')[0].decode('cp949');at+=32
        rest=list(struct.unpack_from('<16f',raw,at));at+=68
        bones.append(dict(name=name,rest=rest))
    for bone in bones:
        bone['poses']=[list(struct.unpack_from('<3f',raw,at+i*12))+list(struct.unpack_from('<4f',raw,at+count*12+i*16)) for i in frames]
        at+=count*28
    assert at==len(raw)
    return dict(bones=bones,frames=len(frames),sourceFrames=frames)

def motion_table(raw):
    assert struct.unpack_from('<i',raw)[0]==100
    at=8;names={}
    for _ in range(struct.unpack_from('<i',raw,4)[0]):
        key,n=struct.unpack_from('<2i',raw,at);at+=8
        names[key]=raw[at:at+n].split(b'\0')[0].decode('cp949');at+=n
    sexes=[]
    for sex in range(2):
        count=struct.unpack_from('<i',raw,at)[0];at+=4
        def read_motion(weapon,state,level):
            begin,end,ani,weap,sound=struct.unpack_from('<2I3H',raw,at+weapon*28800+(state*3+level)*16)
            return dict(begin=begin,end=end,ani=names.get(ani),weapon=names.get(weap),sound=names.get(sound))
        sexes.append(dict(magic=[read_motion(21,14,i) for i in range(3)],idle=read_motion(21,0,0)))
        at+=count*28800
    return sexes

def load_actors(read,png_from_wtm):
    result=dict(people={},weapons={},textures={});table=motion_table(read('Body/character.wad'))
    def load_model(path,folder):
        chunks=model(read(path))
        for chunk in chunks:
            name=folder+'/'+chunk['texture'];chunk['texture']=name
            if name not in result['textures']:
                result['textures'][name]='data:image/png;base64,'+base64.b64encode(png_from_wtm(read('Texture/'+name))).decode()
        return chunks
    for sex,(armor,base,letter) in enumerate([('ma','mb','m')]):
        parts=[]
        for part in ('tor','leg','boo','gun'):
            parts+=load_model(f'Body/High/{armor}_{part}_b001_1.mod','Body')
        parts+=load_model(f'Body/High/{base}_hea_0001.mod','Body')
        parts+=load_model(f'Body/High/{base}_hir_0001.mod','Body')
        arrow_raw=read(f'Skill/Ani/arrow_{letter}.ani');n=struct.unpack_from('<i',arrow_raw,32)[0]
        arrow=animation(arrow_raw,list(range(n)));arrow['duration']=2500
        sections=table[sex]['magic'];ani=sections[0]['ani'];assert all(s['ani']==ani for s in sections)
        magic_frames=[i for s in sections for i in range(s['begin'],s['end']+1)]
        magic=animation(read('Body/Animation/'+ani),magic_frames);magic['duration']=1500
        idle=table[sex]['idle'];idle_ani=animation(read('Body/Animation/'+idle['ani']),list(range(idle['begin'],idle['end']+1)));idle_ani['duration']=len(idle_ani['sourceFrames'])*1000/30
        result['people'][str(sex)]=dict(parts=parts,arrow=arrow,magic=magic,idle=idle_ani)
    for kind,name,bone in [('arrow','mt_longbow_1.mod','Bip01 L Finger0'),('magic','mt_staff_1.mod','Bip01 R Finger0')]:
        result['weapons'][kind]=dict(parts=load_model('Equip/'+name,'Equip'),bone=bone)
    return result
