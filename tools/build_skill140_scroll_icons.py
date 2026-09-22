"""Reuse the original red-ribbon scroll sprite and the established gold item frame."""
from pathlib import Path
import json
import struct
import zlib
from build_twilight_icon_frames import unpack, pack

R=Path(__file__).resolve().parents[1]
D=R/'client-overlay'
IDS={19120,19121,19122,19123}
NAME='mt_skill140_scroll.bmp'

def main():
    original=unpack(R/'runtime/client/GameClient/Item/magicscroll_b.wtm')
    frame=unpack(R/'runtime/client/GameClient/Item/ws_0081.wtm')
    assert original.size==frame.size==(28,28)
    icon=original.copy();removed=0
    for y in range(28):
        for x in range(28):
            r,g,b=original.getpixel((x,y))
            if x<2 or x>=26 or y<2 or y>=26:
                icon.putpixel((x,y),frame.getpixel((x,y)))
            elif g>r+7 and g>b+4:
                # Remove only the existing green background; paper/ribbon pixels stay intact.
                icon.putpixel((x,y),(0,0,0));removed+=1
    for folder in ['Item','Texture/Body']:
        path=D/folder/Path(NAME).with_suffix('.wtm');path.parent.mkdir(parents=True,exist_ok=True);pack(icon,path)
    def update(text,column):
        out=[];seen=set()
        for line in text.splitlines():
            row=line.split('\t')
            if row[0].isdigit() and int(row[0]) in IDS:
                row[column]=NAME;line='\t'.join(row);seen.add(int(row[0]))
            out.append(line)
        assert seen==IDS
        return ('\r\n'.join(out)+'\r\n').encode('cp949')
    path=D/'Item/ITEM.dat';path.write_bytes(update(path.read_bytes().decode('cp949'),58))
    path=D/'Interface/item.dat';raw=path.read_bytes();data=update(zlib.decompress(raw[20:]).decode('cp949'),59)
    path.write_bytes(raw[:16]+struct.pack('<I',len(data))+zlib.compress(data,9))
    icon.save(R/'assets/skills140/skill140-scroll.png')
    report=dict(items=sorted(IDS),source='magicscroll_b.wtm',frame='ws_0081.wtm',background='black',ribbon='original red',green_background_pixels_removed=removed)
    (R/'assets/skills140/scroll-icon-validation.json').write_text(json.dumps(report,indent=2)+'\n')
    print('Four new skill scrolls use the original red ribbon, black background and native gold frame')

if __name__=='__main__':main()
