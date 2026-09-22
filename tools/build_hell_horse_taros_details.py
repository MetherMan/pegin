"""Package ImageGen art, native horse sounds, and the map-41 night table."""
from pathlib import Path
from io import BytesIO
import hashlib,json,re,shutil,struct,sys,wave,zlib
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'runtime/pylibs'))
from PIL import Image
D=ROOT/'client-overlay';C=ROOT/'runtime/client/GameClient'
HORSE=ROOT/'assets/hell-horse';TAROS=ROOT/'assets/forgotten-taros'

def pack(image,path):
    stream=BytesIO();image.save(stream,format='TGA' if image.mode=='RGBA' else 'BMP')
    raw=stream.getvalue();path.parent.mkdir(parents=True,exist_ok=True)
    path.write_bytes(b'TEAMMAY\0\0'+struct.pack('<I',len(raw))+zlib.compress(raw,9))
    decoded=Image.open(BytesIO(zlib.decompress(path.read_bytes()[13:])))
    assert decoded.mode==image.mode and decoded.size==image.size and decoded.tobytes()==image.tobytes()

def icon_rows(raw,compressed=False):
    lines=raw.decode('cp949').splitlines(keepends=True);found=0;out=[]
    for line in lines:
        body=line.rstrip('\r\n');ending=line[len(body):];row=body.split('\t')
        if row[0]=='19130':
            found+=1;columns=[59,65] if compressed else [58,64]
            for index in columns:
                assert row[index] in ['horse01.bmp','mt_hellhorse_icon.bmp']
                row[index]='mt_hellhorse_icon.bmp'
            line='\t'.join(row)+ending
        out.append(line)
    if not found and compressed:
        # The earlier new mount was absent from the compressed UI catalogue.
        row=next(l.split('\t') for l in (D/'Item/ITEM.dat').read_bytes().decode('cp949').splitlines() if l.startswith('19130\t'))
        row=row[:5]+['0']+row[5:]
        assert row[59]==row[65]=='mt_hellhorse_icon.bmp'
        return (''.join(out).rstrip('\r\n')+'\r\n'+'\t'.join(row)+'\r\n').encode('cp949')
    assert found==1
    return ''.join(out).encode('cp949')

def build_icon():
    # ImageGen edits an enlarged 28x28 pixel grid; keep its original pixel edges.
    im=Image.open(HORSE/'certificate-imagegen.png').convert('RGB').resize((28,28),Image.Resampling.NEAREST)
    im.save(HORSE/'certificate-icon.png')
    for folder in ['Item','Texture/Body']:pack(im,D/folder/'mt_hellhorse_icon.wtm')
    for p in [D/'Item/ITEM.dat',ROOT/'game-data/DATA/ITEM_DATA.txt']:p.write_bytes(icon_rows(p.read_bytes()))
    p=D/'Interface/item.dat';before=p.read_bytes();raw=icon_rows(zlib.decompress(before[20:]),True)
    p.write_bytes(before[:16]+struct.pack('<I',len(raw))+zlib.compress(raw,9))

def build_sounds():
    p=D/'Vehicle/vehicle.wad';before=(p if p.exists() else C/'Vehicle/vehicle.wad').read_bytes()
    version,count=struct.unpack_from('<II',before);assert version==100
    at=8;strings={}
    for _ in range(count):
        ident,n=struct.unpack_from('<II',before,at);at+=8;strings[ident]=before[at:at+n];at+=n
    names={v.rstrip(b'\0').decode('cp949'):k for k,v in strings.items()}
    sounds=['HorseGallop.wav','HorseBlow.wav','HorseWalk_A01.wav','HorseWhinney.wav']
    for name in sounds[:3]:
        if name not in names:
            ident=max(strings)+1;strings[ident]=name.encode()+b'\0';names[name]=ident
    records=bytearray(before[at:]);cursor=0;linked=[];sound_bytes=set()
    for sex in range(2):
        weapons=struct.unpack_from('<I',records,cursor)[0];cursor+=4
        for weapon in range(weapons):
            for mode in range(10):
                for state in range(50):
                    for level in range(3):
                        first,last,ani,unused,sound=struct.unpack_from('<IIHHH',records,cursor)
                        name={(1,0):'HorseWalk_A01.wav',(2,1):'HorseGallop.wav',(2,2):'HorseBlow.wav'}.get((state,level))
                        if mode==1 and name and ani and last>first:
                            struct.pack_into('<H',records,cursor+12,names[name])
                            sound_bytes.update([cursor+12,cursor+13])
                            linked.append(dict(sex=sex,weapon=weapon,state=state,phase=level,sound=name))
                        cursor+=16
    assert cursor==len(records) and len(linked)==12
    # Sound indices are the only modified fields in existing motion records.
    old_records=before[at:];allowed=set()
    for i,(a,b) in enumerate(zip(old_records,records)):
        if a!=b:allowed.add(i)
    assert allowed<=sound_bytes
    raw=bytearray(struct.pack('<II',100,len(strings)))
    for ident,value in strings.items():raw+=struct.pack('<II',ident,len(value))+value
    raw+=records;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes(raw)
    audio=[]
    for name in sounds:
        source=C/'Sound'/name;target=D/'Sound'/name;target.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(source,target)
        with wave.open(str(target)) as wav:
            assert wav.getnchannels()==1 and wav.getsampwidth()==2 and wav.getnframes()>0
            audio.append(dict(file=name,seconds=round(wav.getnframes()/wav.getframerate(),3),sha256=hashlib.sha256(target.read_bytes()).hexdigest()))
    return dict(linked_motion_sounds=linked,native_audio=audio,animation_frames_and_models_unchanged=True)

def build_map_presentation(client=C):
    image=Image.open(TAROS/'loading-imagegen.png').convert('RGB')
    for width,height in [(800,600),(1024,768)]:
        native=image.resize((width,height),Image.Resampling.LANCZOS)
        pack(native,D/'Map'/f'loading0041_{width:04d}.wtm');native.save(TAROS/f'loading-{width}.png')
    title=Image.open(TAROS/'title-imagegen.png').convert('RGBA').resize((324,131),Image.Resampling.LANCZOS)
    assert title.getchannel('A').getextrema()[0]==0 and title.getchannel('A').getextrema()[1]>0
    pack(title,D/'Map/title0041.wtm');title.save(TAROS/'title.png')
    table=D/'Light/table.scr';source=table if table.exists() else client/'Light/table.scr'
    text=source.read_bytes().decode('cp949')
    text=re.sub(r'(?:; Forgotten Taros: always night[\r\n]+)?\[MAP\]\s+41\b.*?\[/MAP\][\r\n]*','',text,flags=re.S)
    text=text.rstrip()+'\r\n\r\n; Forgotten Taros: always night\r\n[MAP]\t41\r\n\t[ALL]\t23\r\n[/MAP]\r\n'
    table.parent.mkdir(parents=True,exist_ok=True);table.write_bytes(text.encode('cp949'))
    (D/'Map/land0041.mds').write_bytes(b'; Forgotten Taros: fixed clear night; packet filter prevents rain/snow.\r\nWeather: 3\r\nEnd\r\n')
    return dict(map_id=41,title='Forgotten Taros',loading_sizes=[[800,600],[1024,768]],light_hour=23,weather='fixed clear; map-scoped client packet filter')

def main():
    build_icon();report=dict(sounds=build_sounds(),map=build_map_presentation(),actual_game_audio_visual_tested=False)
    (HORSE/'details-validation.json').write_text(json.dumps(report,ensure_ascii=False,indent=2)+'\n',encoding='utf-8')
    print(json.dumps(report,ensure_ascii=False))

if __name__=='__main__':main()
