"""Convert the approved ImageGen floor decal to the native power-of-two WTM."""
from pathlib import Path
import sys,io,zlib,struct
R=Path(__file__).resolve().parents[1];sys.path.insert(0,str(R/'runtime/pylibs'))
from PIL import Image
def main():
    convert('meteor-magma-imagegen.png','mm_magma_surface.wtm')
    convert('cocytus-frozen-ground-imagegen.png','mf_frozen_ground.wtm')

def convert(source_name,target_name):
    source=R/'assets/visual-refresh-20260923'/source_name
    im=Image.open(source).convert('RGB').resize((1024,1024),Image.Resampling.LANCZOS)
    buf=io.BytesIO();im.save(buf,format='BMP');raw=buf.getvalue()
    target=R/'client-overlay/Texture/Effect'/target_name
    target.write_bytes(b'TEAMMAY\0\0'+struct.pack('<I',len(raw))+zlib.compress(raw,9))
    assert Image.open(io.BytesIO(zlib.decompress(target.read_bytes()[13:]))).tobytes()==im.tobytes()
    print('Native texture converted and read back successfully:',target_name)
if __name__=='__main__':main()
