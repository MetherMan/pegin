"""Register the Blue Moon horse resources and certificate 19131 in the client and server catalogues.

Model/texture come from tools/build_blue_moon_horse.py (coat and saddle chosen below), the icon from
tools/build_blue_moon_certificate.py. Catalogue rows copy the hell-horse certificate row (same item
kind 41, prices and ground model) with a new name and icon. Idempotent.
"""
from pathlib import Path
from io import BytesIO
import hashlib, json, shutil, struct, sys, zlib

R = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(R/'runtime/pylibs'))
from PIL import Image

A = R/'assets/blue-moon-horse'
D = R/'client-overlay'
COAT, SADDLE = 'midnight', 'original'  # the look shown by default in the approved preview
NAME = '푸른달의 말 소유증서'
ITEM, SOURCE_ITEM = '19131', '19130'
ICON = 'mt_bluemoonhorse_icon.bmp'


def pack(image, path):
    stream = BytesIO()
    image.save(stream, format='BMP')
    raw = stream.getvalue()
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_bytes(b'TEAMMAY\0\0'+struct.pack('<I', len(raw))+zlib.compress(raw, 9))
    decoded = Image.open(BytesIO(zlib.decompress(path.read_bytes()[13:])))
    assert decoded.size == image.size and decoded.convert('RGB').tobytes() == image.tobytes()


def add_row(text, compressed=False):
    lines = text.splitlines(keepends=True)
    lines = [l for l in lines if not l.startswith(ITEM+'\t')]
    source = [l for l in lines if l.startswith(SOURCE_ITEM+'\t')]
    assert len(source) == 1
    body = source[0].rstrip('\r\n')
    row = body.split('\t')
    row[0], row[2], row[3] = ITEM, NAME, NAME
    icons = [59, 65] if compressed else [58, 64]
    for i in icons:
        assert row[i] == 'mt_hellhorse_icon.bmp', (i, row[i])
        row[i] = ICON
    at = lines.index(source[0])+1
    lines.insert(at, '\t'.join(row)+'\r\n')
    if not lines[at-1].endswith('\n'):
        lines[at-1] += '\r\n'
    return ''.join(lines)


def main():
    files = {}
    model = A/'payload/Vehicle/mt_bluemoonhorse.mod'
    for target in [D/'Vehicle/mt_bluemoonhorse.mod']:
        shutil.copy2(model, target)
    act = (D/'Vehicle/mt_hellhorse.act').read_bytes()
    (D/'Vehicle/mt_bluemoonhorse.act').write_bytes(act)
    atlas = Image.open(A/f'atlas-{COAT}-{SADDLE}.png').convert('RGB')
    pack(atlas, D/'Texture/Vehicle/mt_bluemoonhorse.wtm')
    icon = Image.open(A/'certificate-icon.png').convert('RGB')
    assert icon.size == (28, 28)
    for folder in ['Item', 'Texture/Body']:
        pack(icon, D/folder/'mt_bluemoonhorse_icon.wtm')
    for path in [D/'Item/ITEM.dat', R/'game-data/DATA/ITEM_DATA.txt']:
        path.write_bytes(add_row(path.read_bytes().decode('cp949')).encode('cp949'))
    ui = D/'Interface/item.dat'
    before = ui.read_bytes()
    raw = add_row(zlib.decompress(before[20:]).decode('cp949'), True).encode('cp949')
    ui.write_bytes(before[:16]+struct.pack('<I', len(raw))+zlib.compress(raw, 9))
    for rel in ['Vehicle/mt_bluemoonhorse.mod', 'Vehicle/mt_bluemoonhorse.act', 'Texture/Vehicle/mt_bluemoonhorse.wtm',
                'Item/mt_bluemoonhorse_icon.wtm', 'Texture/Body/mt_bluemoonhorse_icon.wtm', 'Item/ITEM.dat',
                'Interface/item.dat']:
        files['client-overlay/'+rel] = hashlib.sha256((D/rel).read_bytes()).hexdigest()
    files['game-data/DATA/ITEM_DATA.txt'] = hashlib.sha256((R/'game-data/DATA/ITEM_DATA.txt').read_bytes()).hexdigest()
    report = dict(item=int(ITEM), name=NAME, ride_type=5, speed=120, coat=COAT, saddle=SADDLE, icon=ICON,
                  copied_from=int(SOURCE_ITEM), files=files)
    (A/'registration.json').write_text(json.dumps(report, ensure_ascii=False, indent=2)+'\n', encoding='utf-8')
    print(json.dumps(report, ensure_ascii=False)[:400])


if __name__ == '__main__':
    main()
