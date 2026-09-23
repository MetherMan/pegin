"""Integrate the ImageGen moon into its existing UV island; never rebuild MODs."""
from pathlib import Path
import io,json,hashlib,struct,sys,zlib
R=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(R/'runtime/pylibs'))
from PIL import Image
import numpy as np
O=R/'assets/twilight-set/eclipse-hook-staff'

def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()

def replace_glb_image(path,image):
    raw=path.read_bytes();size,kind=struct.unpack_from('<II',raw,12)
    doc=json.loads(raw[20:20+size]);at=20+size
    length,kind=struct.unpack_from('<II',raw,at);body=raw[at+8:at+8+length]
    # Replace only the matching embedded image. Vertex/index/UV buffers stay exact.
    for entry in doc['images']:
        view=doc['bufferViews'][entry['bufferView']];start=view.get('byteOffset',0)
        old=body[start:start+view['byteLength']]
        if Image.open(io.BytesIO(old)).size!=image.size:continue
        output=io.BytesIO();image.save(output,format='PNG');data=output.getvalue()
        old_end=(start+view['byteLength']+3)//4*4
        data_padded=data+b'\0'*((-len(data))%4)
        delta=len(data_padded)-(old_end-start)
        body=body[:start]+data_padded+body[old_end:]
        view['byteLength']=len(data)
        for other in doc['bufferViews']:
            if other is not view and other.get('byteOffset',0)>=old_end:other['byteOffset']+=delta
        doc['buffers'][0]['byteLength']=len(body)
        break
    else:raise ValueError('Matching embedded texture not found: '+str(path))
    encoded=json.dumps(doc,separators=(',',':')).encode();encoded+=b' '*((-len(encoded))%4)
    path.write_bytes(struct.pack('<III',0x46546C67,2,28+len(encoded)+len(body))+struct.pack('<II',len(encoded),0x4E4F534A)+encoded+struct.pack('<II',len(body),0x004E4942)+body)

def main():
    protected=[R/f'client-overlay/Equip/mt_staff_{i}.mod' for i in (1,2,3)]
    before={str(p.relative_to(R)):sha(p) for p in protected}
    original=Image.open(O/'source-staff-crop.png').convert('RGB')
    generated=Image.open(O/'moon-bright-imagegen.png').convert('RGB')
    # Remove ImageGen's white side padding and map its existing full-height
    # crop back onto the 298x1008 UV rectangle. This is asset placement only.
    placed=generated.crop((42,0,683,2170)).resize(original.size,Image.Resampling.LANCZOS)
    a=np.asarray(original);b=np.asarray(placed);y,x=np.indices(a.shape[:2])
    radius=np.sqrt(((x-122)/66.5)**2+((y-142)/66.5)**2)
    # Keep the original gold filigree crossing the moon and all other islands.
    mask=(radius<1)&(a[:,:,2].astype(float)>a[:,:,0]*1.22)&(a[:,:,2]>45)
    alpha=np.clip((1-radius)*20,0,1)*mask
    merged=np.rint(a*(1-alpha[:,:,None])+b*alpha[:,:,None]).astype('uint8')
    assert np.array_equal(merged[~mask],a[~mask])
    final=Image.fromarray(merged);final.save(O/'moon-bright-staff-crop.png')
    source=R/'client-overlay/Texture/Equip/mt_staff_atlas.wtm'
    raw=source.read_bytes();atlas=Image.open(io.BytesIO(zlib.decompress(raw[13:]))).convert('RGB')
    prior=np.asarray(atlas).copy();atlas.paste(final,(8,8));pixels=np.asarray(atlas)
    allowed=np.zeros(prior.shape[:2],dtype=bool);allowed[8:1016,8:306]=mask
    assert np.array_equal(prior[~allowed],pixels[~allowed])
    bmp=io.BytesIO();atlas.save(bmp,format='BMP');data=bmp.getvalue()
    source.write_bytes(raw[:9]+struct.pack('<I',len(data))+zlib.compress(data,9))
    atlas.save(O/'moon-bright-atlas.png')
    replace_glb_image(R/'assets/twilight-set/staff-native.glb',atlas)
    replace_glb_image(O/'eclipse-hook-staff.glb',final)
    after={str(p.relative_to(R)):sha(p) for p in protected};assert before==after
    report=dict(passed=True,mode='built-in ImageGen edit; UV integration',changed_pixels=int(np.count_nonzero(np.any(prior!=pixels,axis=2))),outside_moon_byte_identical=True,mod_mesh_uv_sha256=after,atlas_sha256=sha(source),in_game_visual_test=False)
    (O/'moon-bright-validation.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps(report))

if __name__=='__main__':main()
