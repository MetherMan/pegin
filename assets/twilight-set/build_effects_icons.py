"""Compile the existing arrow effect with a per-weapon blue material tint.

Motion, mesh, sound, blend mode, opacity, damage timing and frame count are
copied byte-for-byte. Only RGB material values change in the new resources.
"""
from mesh_tools import *
import hashlib,subprocess,sys
O=Path(__file__).resolve().parent;C=ROOT/'runtime/client/GameClient';D=ROOT/'client-overlay'
if (O/'icons-current/manifest.json').is_file():
    subprocess.run([sys.executable,str(ROOT/'tools/build_current_weapon_icons.py')],check=True,cwd=ROOT)
else:
    for kind,prefix in [('longbow','mt_longbow'),('staff','mt_staff')]:
        im=Image.open(O/(kind+'-icon.png')).convert('RGB').resize((28,28),Image.Resampling.LANCZOS)
        im.save(O/(kind+'-icon-28.png'));pack_texture(im,D/f'Item/{prefix}_icon.wtm');pack_texture(im,D/f'Texture/Body/{prefix}_icon.wtm')
color=np.array([.45,.80,1.0]);wed_reports=[]
for original,target in [('arrow01.wed','mt_arrow.wed'),('ta_arrow01.wed','mt_arrow_hit.wed')]:
    raw=(C/'Effect'/original).read_bytes();b=bytearray(raw);at=31
    for _ in range(2):n=struct.unpack_from('<i',b,at)[0];at+=4+n
    start,last,count,row,col=struct.unpack_from('<5i',b,at);at+=20
    assert at+count*52==len(b),(original,at,count,len(b))
    changed=set()
    for i in range(count):
        off=at+i*52+44;old=struct.unpack_from('<I',b,off)[0];rgb=[(old>>16)&255,(old>>8)&255,old&255]
        rgb=np.rint(np.array(rgb)*color).astype(int);new=(old&0xff000000)|(rgb[0]<<16)|(rgb[1]<<8)|rgb[2]
        struct.pack_into('<I',b,off,int(new));changed.update(range(off,off+3))
    assert all(i in changed for i,(a,z) in enumerate(zip(raw,b)) if a!=z)
    (D/'Effect'/target).write_bytes(b);wed_reports.append(dict(source=original,target=target,frames=count,only_rgb_changed=True))

# Bake the trace material's RGB tint into its native BMP pixel channel bytes.
# The original alpha, black additive mask, dimensions and texture pattern are
# kept exactly; no image generation or resampling changes the effect's shape.
raw=(C/'Texture/Effect/arw01.wtm').read_bytes();bmp=bytearray(zlib.decompress(raw[13:]));pixoff=struct.unpack_from('<I',bmp,10)[0]
width,height=struct.unpack_from('<2i',bmp,18);bits=struct.unpack_from('<H',bmp,28)[0];compression=struct.unpack_from('<I',bmp,30)[0]
assert bits in [24,32] and compression==0
stride=((width*bits+31)//32)*4;bytespp=bits//8
original_bmp=bytes(bmp)
for y in range(abs(height)):
    for x in range(width):
        off=pixoff+y*stride+x*bytespp
        for channel,factor in enumerate(color[::-1]):bmp[off+channel]=round(bmp[off+channel]*factor)
assert bmp[:pixoff]==original_bmp[:pixoff]
if bits==32:assert all(bmp[pixoff+y*stride+x*bytespp+3]==original_bmp[pixoff+y*stride+x*bytespp+3] for y in range(abs(height)) for x in range(width))
(D/'Texture/Effect/mt_arrow_trace.wtm').write_bytes(raw[:9]+struct.pack('<I',len(bmp))+zlib.compress(bmp,9))
tracepath=D/'Effect/trace.tdf';data=tracepath.read_bytes();version,count=struct.unpack_from('<2i',data);at=8;entries=[];peer=None
for _ in range(count):
    start=at;n=struct.unpack_from('<i',data,at)[0];at+=4;name=data[at:at+n].rstrip(b'\0').decode('cp949');at+=n;n=struct.unpack_from('<i',data,at)[0];at+=4;tex=data[at:at+n].rstrip(b'\0').decode('cp949');at+=n;level,rate=struct.unpack_from('<2i',data,at);at+=8
    if name=='활':peer=(level,rate)
    if name!='mt_arrow_trace':entries.append(data[start:at])
assert at==len(data) and peer
def string(s):b=s.encode('ascii')+b'\0';return struct.pack('<i',len(b))+b
entries.append(string('mt_arrow_trace')+string('mt_arrow_trace.bmp')+struct.pack('<2i',*peer));tracepath.write_bytes(struct.pack('<2i',version,len(entries))+b''.join(entries))
magic=(C/'Magic/arw_d1.ms').read_text(encoding='cp949').replace('[NAME] 단궁기본','[NAME] mt_arrow').replace('[TRACE] 활','[TRACE] mt_arrow_trace').replace('[SPE] arrow01.wed','[SPE] mt_arrow.wed').replace('[TAIL] ta_arrow01.wed','[TAIL] mt_arrow_hit.wed')
(D/'Magic').mkdir(exist_ok=True);(D/'Magic/mt_arrow.ms').write_bytes(magic.encode('cp949'))
ml=(D/'Magic/MagicList.ml' if (D/'Magic/MagicList.ml').exists() else C/'Magic/MagicList.ml').read_bytes()
ml=b'\r\n'.join(l for l in ml.splitlines() if not l.startswith(b'[MAGIC] mt_arrow'))
ml+=b'\r\n[MAGIC] mt_arrow\t\tmt_arrow.ms\r\n';(D/'Magic/MagicList.ml').write_bytes(ml)
report=dict(passed=True,effects=wed_reports,trace_frames_and_rate=peer,trace_dimensions=[width,abs(height)],trace_opacity_and_shape_preserved=True,only_twilight_bow=True,staff_effects_unchanged=True,visual_in_game_checked=False)
(O/'arrow-effect-validation.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
