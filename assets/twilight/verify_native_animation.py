"""Check rigid MOD chunks against the original game's idle weapon tracks."""
from pathlib import Path
import argparse,json,struct,zlib
from io import BytesIO
import numpy as np
from PIL import Image

O=Path(__file__).resolve().parent
def read_mod(path):
    data=path.read_bytes();count=struct.unpack_from('<i',data,32)[0];offset=36;parts=[]
    dtype=np.dtype([('index','<i4'),('normal','<f4',3),('uv','<f4',2)])
    for index in range(count):
        matrix=np.frombuffer(data,'<f4',16,offset).reshape(4,4);offset+=64
        vertices=struct.unpack_from('<i',data,offset)[0];offset+=4
        points=np.frombuffer(data,'<f4',vertices*3,offset).reshape(-1,3);offset+=vertices*12
        faces=struct.unpack_from('<i',data,offset)[0];offset+=4
        corners=np.frombuffer(data,dtype,faces*3,offset);offset+=faces*72+16
        texture=data[offset:offset+32].split(b'\0')[0].decode();offset+=32
        assert struct.unpack_from('<i',data,offset)[0]==0;offset+=4
        assert corners['index'].min()>=0 and corners['index'].max()<vertices<65536
        assert np.isfinite(points).all() and np.isfinite(corners['normal']).all() and np.isfinite(corners['uv']).all()
        parts.append(dict(index=index,matrix=matrix,points=points,corners=corners,texture=texture))
    assert offset==len(data)
    return parts

def main():
    parser=argparse.ArgumentParser();parser.add_argument('--client',type=Path,required=True);args=parser.parse_args()
    animations={}
    for name in ['m_all_pe_std_81_weA.ani','w_all_pe_std_61_weA.ani','female_twohand_pe_we.ani']:
        data=(args.client/'Body/Weapon'/name).read_bytes();frames,count=struct.unpack_from('<2i',data,32)
        tracks=[struct.unpack_from('<i',data,40+i*(4+frames*28))[0] for i in range(count)]
        assert tracks==[0],(name,tracks)
        animations[name]=dict(frames=frames,mesh_tracks=tracks)
    original_count=len(read_mod(args.client/'Equip/Wp_0026_1.mod'))
    assert original_count==1
    lods=[]
    reference=json.loads((O/'native_model_validation.json').read_text())
    for lod in [1,2,3]:
        parts=read_mod(O/f'payload/mt_twilight_{lod}.mod');assert len(parts)==1
        part=parts[0];assert part['texture']=='mt_twilight_atlas.bmp'
        assert np.allclose(part['matrix'],np.eye(4))
        assert np.allclose([part['points'].min(axis=0),part['points'].max(axis=0)],reference['bounds'],atol=1e-6)
        assert part['corners']['uv'].min()>0 and part['corners']['uv'].max()<1
        for animation in animations.values():assert all(p['index'] in animation['mesh_tracks'] for p in parts)
        lods.append(dict(lod=lod,meshes=len(parts),unanimated_parts=0,texture=part['texture']))
    texture=(O/'payload/mt_twilight_atlas.wtm').read_bytes();bmp=zlib.decompress(texture[13:]);assert len(bmp)==struct.unpack_from('<I',texture,9)[0]
    assert Image.open(BytesIO(bmp)).size==(2048,2048)
    result=dict(passed=True,reference_meshes=original_count,animations=animations,lods=lods,visual_in_game_checked=False)
    (O/'animation_validation.json').write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps(result))
if __name__=='__main__':main()
