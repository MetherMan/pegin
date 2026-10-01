"""Pack approved ImageGen weapon icons into the unchanged native item frame.

Only a declared crop of a generated frame, mechanical resizing, the existing UI
frame, and BMP/WTM encoding happen here. Weapon artwork comes from image_gen.
"""
from pathlib import Path
from io import BytesIO
import hashlib
import json
import struct
import sys
import zlib

R = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(R/'runtime/pylibs'))
from PIL import Image

A = R/'assets/twilight-set/icons-current'
MANIFEST = A/'manifest.json'
FAMILIES = {'sword':('twilight',19030), 'longbow':('longbow',19060), 'staff':('staff',19090)}


def sha(data):
    return hashlib.sha256(data).hexdigest()


def unpack(raw):
    assert raw[:7] == b'TEAMMAY'
    bmp = zlib.decompress(raw[13:])
    assert len(bmp) == struct.unpack_from('<I', raw, 9)[0]
    return Image.open(BytesIO(bmp)).convert('RGB')


def encode(icon):
    stream = BytesIO()
    icon.save(stream, format='BMP')
    bmp = stream.getvalue()
    assert len(bmp) == 2406 and struct.unpack_from('<H', bmp, 28)[0] == 24
    return b'TEAMMAY\0\0'+struct.pack('<I',len(bmp))+zlib.compress(bmp,9)


def main():
    config = json.loads(MANIFEST.read_text(encoding='utf-8'))
    frame = Image.open(A/config['frame']).convert('RGB')
    assert frame.size == (28,28)
    records = []
    for kind,(prefix,start) in FAMILIES.items():
        spec = config['icons'][kind]
        source = (A/spec['source']).resolve()
        assert source.is_relative_to(R.resolve())
        master = Image.open(source).convert('RGBA')
        assert master.width == master.height
        source_dimensions = list(master.size)
        # Approved masters include a drawn frame. Remove only that margin so
        # downsampling does not put a second gold rectangle inside the native UI.
        crop = spec.get('art_box', [0, 0, master.width, master.height])
        assert len(crop) == 4 and all(type(v) is int for v in crop)
        left, top, right, bottom = crop
        assert 0 <= left < right <= master.width and 0 <= top < bottom <= master.height
        assert right-left == bottom-top
        master = master.crop(crop)
        # Flatten supplied transparency only onto the native black UI backdrop.
        backdrop = Image.new('RGBA',master.size,(0,0,0,255))
        backdrop.alpha_composite(master)
        inner = backdrop.convert('RGB').resize((24,24),Image.Resampling.LANCZOS)
        icon = frame.copy()
        icon.paste(inner,(2,2))
        packed = encode(icon)
        decoded = unpack(packed)
        assert decoded.tobytes() == icon.tobytes()
        assert all(icon.getpixel((x,y)) == frame.getpixel((x,y))
                   for y in range(28) for x in range(28)
                   if x<2 or x>=26 or y<2 or y>=26)
        native_name = 'mt_'+prefix+'_icon.wtm'
        for folder in ('Item','Texture/Body'):
            (R/'client-overlay'/folder/native_name).write_bytes(packed)
        icon.save(A/(kind+'-28.png'))
        icon.save(R/'assets/skills140'/('mt_'+prefix+'_icon.png'))
        if kind == 'sword':
            icon.save(R/'assets/twilight/inventory_icon_28.png')
            (R/'assets/twilight/payload'/native_name).write_bytes(packed)
        else:
            icon.save(R/'assets/twilight-set'/(kind+'-icon-28.png'))
        records.append(dict(kind=kind,item_ids=list(range(start,start+21)),
                            source=source.relative_to(R).as_posix(),source_sha256=sha(source.read_bytes()),
                            source_dimensions=source_dimensions,art_box=crop,
                            native_name=native_name,wtm_sha256=sha(packed),
                            dimensions=[28,28],bits=24,existing_frame_identical=True))
    validation = dict(passed=True,artwork_tool='built-in image_gen',
                      native_frame_source=config['frame_origin'],frame_width=2,
                      item_and_equipped_slots_share_sprite=True,icons=records,
                      covered_items=63,in_game_visual_test=False)
    (A/'validation.json').write_text(json.dumps(validation,indent=2)+'\n',encoding='utf-8')
    print('Updated 3 weapon icons / 6 WTM files / 63 item IDs; native gold frames preserved')
    return validation


if __name__ == '__main__':
    main()
