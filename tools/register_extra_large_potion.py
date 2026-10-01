"""Register the extra-large HP potion (19132 "초대형 물약", heal 600) everywhere a potion lives.

- Icon: assets/potions/hp-extra-large-v2-green.png (the user's ImageGen pick) reduced to the
  native 28x28 inventory sprite, packed as Item/ and Texture/Body/ hppotion_xl.wtm.
- Catalogues: each file's own HP 회복 포션(대) row (10097) is copied with the new id, name, heal
  amount (column 7, read by UseHPPotion) and that file's 10097 prices +20%. The server charges
  ITEM_DATA (600 → 720); the shop window shows Item/ITEM.dat, whose 10097 price was already lower
  (250 → 300), so both the charge and the displayed price are 20% above the large potion.
- Shops: every potion merchant that sells 10097 also sells 19132, listed right after it.
- Server: stacking, buying, selling and trading already go by item type 44. Only drinking and its
  sound are chosen by item id, so dHP_POSION_XL joins those switches.
Idempotent; every source edit is guarded by its original text.
"""
from pathlib import Path
from io import BytesIO
import hashlib, json, struct, sys, zlib

R = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(R/'runtime/pylibs'))
from PIL import Image, ImageFilter

A = R/'assets/potions'
D = R/'client-overlay'
S = R/'src/server'
ITEM, SOURCE = '19132', '10097'
NAME = '초대형 물약'
HEAL = 600
MARKUP = 1.2
ICON = 'hppotion_xl.bmp'


def icon():
    art = Image.open(A/'hp-extra-large-v2-green.png').convert('RGB')
    # Box-average to 4x first so the 1254px painting keeps its shading, then a light unsharp mask
    # to restore the dark outline that the original 28x28 potion sprites have.
    small = art.resize((112, 112), Image.BOX).resize((28, 28), Image.LANCZOS)
    small = small.filter(ImageFilter.UnsharpMask(1, 60, 2))
    small.save(A/'hp-extra-large-icon-28.png')
    return small


def pack(image, path):
    stream = BytesIO()
    image.save(stream, format='BMP')
    raw = stream.getvalue()
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_bytes(b'TEAMMAY\0\0'+struct.pack('<I', len(raw))+zlib.compress(raw, 9))
    decoded = Image.open(BytesIO(zlib.decompress(path.read_bytes()[13:])))
    assert decoded.size == image.size and decoded.convert('RGB').tobytes() == image.tobytes()


def add_row(text, ui=False):
    """Copy this file's 10097 row. The UI catalogue has one extra column after the item type."""
    shift = 1 if ui else 0
    lines = [l for l in text.splitlines(keepends=True) if not l.startswith(ITEM+'\t')]
    source = [l for l in lines if l.startswith(SOURCE+'\t')]
    assert len(source) == 1
    row = source[0].rstrip('\r\n').split('\t')
    assert row[4] == '44', row[:6]
    row[0], row[2], row[3] = ITEM, NAME, NAME
    row[6+shift] = str(HEAL)
    for k in range(3):
        row[24+shift+k] = str(round(int(row[24+shift+k])*MARKUP))
    for i in (58+shift, 64+shift):
        assert row[i] == 'hppotion_l.bmp', (i, row[i])
        row[i] = ICON
    at = lines.index(source[0])+1
    lines.insert(at, '\t'.join(row)+eol(source[0]))
    return ''.join(lines)


def eol(line):
    return line[len(line.rstrip('\r\n')):] or '\r\n'


def shops(text):
    out, added = [], []
    lines = [l for l in text.splitlines(keepends=True) if l.rstrip('\r\n').split('\t')[-1] != ITEM]
    for line in lines:
        out.append(line)
        cells = line.rstrip('\r\n').split('\t')
        if len(cells) == 2 and cells[1] == SOURCE:
            out.append(cells[0]+'\t'+ITEM+eol(line))
            added.append(int(cells[0]))
    return ''.join(out), added


# (anchor line, inserted line, 'before'/'after', expected anchor count). Lines are whole lines
# without their ending; the inserted line reuses the anchor's own ending (these files mix CRLF/LF).
SERVER = {
    'item.h': [('#define dHP_POSION_L\t\t\t\t10097\t\t\t// HP 포션(대)',
                '#define dHP_POSION_XL\t\t\t19132\t\t\t// HP 포션(초대형), 회복량은 ITEM_DATA 7열', 'after', 1)],
    'message.cpp': [('\t\t\t\t\tcase dHP_POSION_L: // HP 포션', '\t\t\t\t\tcase dHP_POSION_XL: // HP 포션(초대형)', 'before', 1),
                    ('\t\tcase dHP_POSION_L: // HP 포션', '\t\tcase dHP_POSION_XL: // HP 포션(초대형)', 'before', 1)],
    'item.cpp': [('\t\tcase dHP_POSION_L:', '\t\tcase dHP_POSION_XL:', 'after', 1)],
    'gm_catalog_data.h': [('{10097,"HP 회복 포션(대)",4,-1,-1},', '{19132,"초대형 물약",4,-1,-1},', 'after', 1)],
}
PAIRS = {'item.cpp': 'item.utf8.cpp', 'gm_catalog_data.h': 'gm_catalog_data.utf8.h'}


def edit(text, edits, name):
    lines = text.splitlines(keepends=True)
    for anchor, line, where, count in edits:
        bare = [l.rstrip('\r\n') for l in lines]
        hits = [i for i, l in enumerate(bare) if l == anchor]
        assert len(hits) == count, (name, anchor, len(hits))
        for i in reversed(hits):
            near = i-1 if where == 'before' else i+1
            if 0 <= near < len(bare) and bare[near] == line:
                continue  # already applied
            lines.insert(i if where == 'before' else i+1, line+eol(lines[i]))
    return ''.join(lines)


def main():
    image = icon()
    for folder in ['Item', 'Texture/Body']:
        pack(image, D/folder/'hppotion_xl.wtm')
    for path in [D/'Item/ITEM.dat', R/'game-data/DATA/ITEM_DATA.txt']:
        path.write_bytes(add_row(path.read_bytes().decode('cp949')).encode('cp949'))
    ui = D/'Interface/item.dat'
    before = ui.read_bytes()
    raw = add_row(zlib.decompress(before[20:]).decode('cp949'), True).encode('cp949')
    ui.write_bytes(before[:16]+struct.pack('<I', len(raw))+zlib.compress(raw, 9))
    shop = R/'game-data/DATA/SHOP_INFO.txt'
    text, added = shops(shop.read_bytes().decode('cp949'))
    shop.write_bytes(text.encode('cp949'))
    for name, edits in SERVER.items():
        cp = S/'LAQIA_GameServer'/name
        original = cp.read_bytes().decode('cp949')
        assert original.encode('cp949') == cp.read_bytes(), name
        updated = edit(original, edits, name)
        cp.write_bytes(updated.encode('cp949'))
        if name in PAIRS:
            u = S/'utf8'/PAIRS[name]
            text = edit(u.read_bytes().decode('utf-8'), edits, name)
            u.write_bytes(text.encode('utf-8'))
            assert text == updated, name
    files = {}
    for rel in ['client-overlay/Item/hppotion_xl.wtm', 'client-overlay/Texture/Body/hppotion_xl.wtm',
                'client-overlay/Item/ITEM.dat', 'client-overlay/Interface/item.dat',
                'game-data/DATA/ITEM_DATA.txt', 'game-data/DATA/SHOP_INFO.txt']:
        files[rel] = hashlib.sha256((R/rel).read_bytes()).hexdigest()
    prices = {}
    for label, text, shift in [('ITEM_DATA (server charge)', (R/'game-data/DATA/ITEM_DATA.txt').read_bytes(), 0),
                               ('Item/ITEM.dat (shop display)', (D/'Item/ITEM.dat').read_bytes(), 0),
                               ('Interface/item.dat', zlib.decompress(ui.read_bytes()[20:]), 1)]:
        rows = {l.split('	')[0]: l.split('	') for l in text.decode('cp949').splitlines()}
        prices[label] = {k: [int(rows[k][24+shift+i]) for i in range(3)] for k in (SOURCE, ITEM)}
    report = dict(item=int(ITEM), name=NAME, heal=HEAL, markup=MARKUP, prices_repair_sell_buy=prices,
                  purchase_rule='same as other potions: quantity box up to 9999, server charges price x count (MIN(cnt,1) means at least 1 here)',
                  copied_from=int(SOURCE), icon=ICON, shops=added, stack_limit='type 44 = 9999 (stack_limits.h)',
                  files=files)
    (A/'registration.json').write_text(json.dumps(report, ensure_ascii=False, indent=2)+'\n', encoding='utf-8')
    print(json.dumps(dict(item=report['item'], shops=added), ensure_ascii=False))


if __name__ == '__main__':
    main()
