"""Import skill-specific compositions in the game's native 28px art style."""
from pathlib import Path
import json
import struct
import zlib
from build_twilight_icon_frames import unpack, pack
from build_skill140 import SPECS, read
from PIL import Image, ImageDraw, ImageFont

R = Path(__file__).resolve().parents[1]
D = R/'client-overlay'
O = R/'assets/skills140/icons'
ART_REVISION = 'effect-matched-v5'
ART = O/ART_REVISION
ART_OVERRIDES = {'six': 'painted-v4'}
ORIGINALS = {
    'heaven': ('ls_0014', '데쓰패너트레이션'),
    'six': ('ls_0013', '더블더블샷'),
    'meteor': ('mb_0004', '파이어볼'),
    'frost': ('mb_0009', '스파이크아이스'),
}


def art_dir(key):
    return O/ART_OVERRIDES.get(key, ART_REVISION)


def main():
    frame = unpack(R/'runtime/client/GameClient/Item/ls_0014.wtm')
    icons = {}
    for spec in SPECS:
        key = spec['key']; short = key.removeprefix('mt_')
        # Asset import only: artwork was generated with ImageGen. Keep the
        # game's exact native border so it blends into its existing quickbar.
        im = Image.open(art_dir(short)/f'{short}-source.png').convert('RGB').resize((28,28), Image.Resampling.BOX)
        for y in range(28):
            for x in range(28):
                if x < 2 or x >= 26 or y < 2 or y >= 26:
                    im.putpixel((x,y), frame.getpixel((x,y)))
        for folder in ['Item','Texture/Body']:
            pack(im, D/folder/f'{key}_icon.wtm')
        im.save(O/f'{short}-28.png'); im.save(art_dir(short)/f'{short}-28.png'); icons[spec['id']] = im

    def update(text, offset=0):
        lines=[]; found=set()
        for line in text.splitlines():
            row=line.split('\t')
            if row[0].isdigit() and int(row[0]) in icons:
                spec=next(s for s in SPECS if s['id']==int(row[0]))
                for col in [59,65]: row[col+offset]=spec['key']+'_icon.bmp'
                # Both male/female inventory fields use the approved scroll.
                for col in [58,64]: row[col+offset]='mt_skill140_scroll.bmp'
                found.add(spec['id']);line='\t'.join(row)
            lines.append(line)
        assert found==set(icons)
        return ('\r\n'.join(lines)+'\r\n').encode('cp949')

    for p in [D/'Item/ITEM.dat']:
        p.write_bytes(update(read(p)))
    p=D/'Interface/item.dat';b=p.read_bytes()
    raw=update(zlib.decompress(b[20:]).decode('cp949'),1)
    p.write_bytes(b[:16]+struct.pack('<I',len(raw))+zlib.compress(raw,9))

    sheet=Image.new('RGB',(840,242),(24,26,30)); draw=ImageDraw.Draw(sheet)
    font=ImageFont.truetype('C:/Windows/Fonts/malgun.ttf',13)
    for i,spec in enumerate(SPECS):
        x=i*210+7;im=icons[spec['id']]
        draw.text((x,5),spec['name'],font=font,fill='white')
        sheet.paste(im.resize((168,168),Image.Resampling.NEAREST),(x,31))
        sheet.paste(im,(x,207));draw.text((x+36,211),'실제 크기 28 × 28',font=font,fill='#b9bec9')
    sheet.save(O/'skill-icons-preview.png')
    art_sheet=Image.new('RGB',(1008,320),(24,26,30));draw=ImageDraw.Draw(art_sheet)
    for i,spec in enumerate(SPECS):
        x=i*252+6;short=spec['key'].removeprefix('mt_')
        draw.text((x,5),spec['name'],font=font,fill='white')
        source=Image.open(art_dir(short)/f'{short}-source.png').convert('RGB')
        art_sheet.paste(source.resize((240,240),Image.Resampling.LANCZOS),(x,29))
        art_sheet.paste(icons[spec['id']],(x,282))
        draw.text((x+37,288),'게임 아이콘 28 × 28',font=font,fill='#b9bec9')
    art_sheet.save(ART/'art-and-native-preview.png')
    comparison=Image.new('RGB',(840,448),(24,26,30));draw=ImageDraw.Draw(comparison)
    for i,spec in enumerate(SPECS):
        x=i*210+7;short=spec['key'].removeprefix('mt_');peer,title=ORIGINALS[short]
        original=unpack(R/f'runtime/client/GameClient/Item/{peer}.wtm')
        draw.text((x,5),'기존 계열 · '+title,font=font,fill='#c2c6ce')
        comparison.paste(original.resize((168,168),Image.Resampling.NEAREST),(x,30))
        draw.text((x,215),spec['name'],font=font,fill='white')
        im=icons[spec['id']]
        comparison.paste(im.resize((168,168),Image.Resampling.NEAREST),(x,240))
        comparison.paste(im,(x,415));draw.text((x+36,418),'실제 크기 28 × 28',font=font,fill='#b9bec9')
    comparison.save(ART/'original-to-upgraded.png')
    report=dict(items={s['id']:s['key']+'_icon.bmp' for s in SPECS},
                size=[28,28],frame='ls_0014.wtm',border_width=2,
                male_and_female=True,scroll='mt_skill140_scroll.bmp',
                art_revision=ART_REVISION,style_reference_skills=ORIGINALS,
                art_overrides=ART_OVERRIDES,
                effect_color_reference={key:json.loads((art_dir(key)/'prompts.json').read_text(encoding='utf-8')).get('colors',{}).get(key) for key in ORIGINALS})
    (O/'import.json').write_text(json.dumps(report,indent=2)+'\n')
    print('Imported four 28px native skill icons; linked both genders and compressed UI tables')


if __name__=='__main__':main()
