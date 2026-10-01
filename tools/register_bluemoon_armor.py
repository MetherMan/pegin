"""Register the Blue Moon (Twilight) armour: Rank 9, +0..+20, four warrior parts.

Stats: at every enchant level and for every numeric column, Blue Moon = Black Knight x step^2, i.e.
two rank steps above Black Knight (Rank 6), where step is the Black Knight / Red Knight (Rank 5)
ratio of the same part and column. The raw per-level ratio is noisy (+1 would come out below +0),
so step is the least-squares line through the per-level ratios. The rank-9 Twilight two-handed sword sits at
the same distance from the Black Knight sword (x1.68 at +0, x1.75 at +20 vs (BK/RK)^2 1.73/1.68).
Columns equal for both sets (class, weight, skin flags) are copied from Black Knight. Enchanting
needs no code: PACKET_EnchantItem moves to id+1 with the armour success table by target level.

Writes the models/textures/icons from bluemoon_armor_native / build_bluemoon_armor_icons, the
three item catalogues (same rows everywhere, like the Twilight weapons), the GM enhance table and
the warehouse 세트 group "푸른달 트와일라잇" (sword, longbow, staff and the four armour parts).
Idempotent: old rows/entries for these ids are replaced.
"""
from pathlib import Path
import hashlib, json, re, struct, sys, zlib

R = Path(__file__).resolve().parents[1]
sys.path[:0] = [str(R/'runtime/pylibs'), str(R/'tools'), str(R/'client-overlay/Tools/SkillColors')]
import bluemoon_armor_native as nat
import build_bluemoon_armor_icons as icons_tool
from native_actor import model

O = R/'assets/blue-moon-armor'
D = R/'client-overlay'
NAME = '푸른달의 트와일라잇 {}'
# part: (Korean name, first id, Black Knight +0, Red Knight +0)
FAMILIES = {'tor': ('아머', 19140, 10521, 10458), 'leg': ('팬츠', 19170, 10710, 10647),
            'gun': ('건틀렛', 19200, 10899, 10836), 'boo': ('부츠', 19230, 11088, 11025)}
STATS = [6, 7, 8, 9, 24, 25, 26]+list(range(27, 55))+[67, 68]
RANK = '9'
GROUP = '푸른달 트와일라잇'
# Warehouse 세트 slots: 0-9 weapons (2 = 양손검, 7 = 롱보우, 9 = 스태프), 10-13 warrior clothes.
SET_ENTRIES = [(19030, '푸른달의 트와일라잇 양손검', 2), (19060, '푸른달의 트와일라잇 롱보우', 7),
               (19090, '푸른달의 트와일라잇 스태프', 9)]+[
               (first, NAME.format(name), 10+k) for k, (name, first, _, _) in enumerate(FAMILIES.values())]


def rows(raw):
    return {int(r[0]): r for l in raw.decode('cp949').splitlines() if (r := l.split('\t'))[0].isdigit()}


def step_line(bk_rows, rk_rows, i):
    """Least-squares line through BK/RK of column i over the levels where both are positive."""
    pts = [(L, int(b[i])/int(r[i])) for L, (b, r) in enumerate(zip(bk_rows, rk_rows))
           if b[i].isdigit() and r[i].isdigit() and int(b[i]) > 0 and int(r[i]) > 0]
    if not pts:
        return None
    if len(pts) == 1:
        return lambda L: pts[0][1]
    n = len(pts)
    mx, my = sum(p[0] for p in pts)/n, sum(p[1] for p in pts)/n
    slope = sum((x-mx)*(y-my) for x, y in pts)/sum((x-mx)**2 for x, _ in pts)
    return lambda L: my+slope*(L-mx)


def item_rows():
    source = rows((R/'game-data/DATA/ITEM_DATA.txt').read_bytes())
    new, definitions = [], []
    for part, (label, first, black, red) in FAMILIES.items():
        bks, rks = [source[black+L] for L in range(21)], [source[red+L] for L in range(21)]
        lines = {i: step_line(bks, rks, i) for i in STATS}
        for level in range(21):
            bk, rk = source[black+level], source[red+level]
            assert bk[1] == rk[1] == str(level) and bk[4] == rk[4], (black+level, red+level)
            r = bk.copy()
            r[0] = str(first+level)
            r[2] = r[3] = (f'+{level} ' if level else '')+NAME.format(label)
            assert len(r[2].encode('cp949')) < 32
            r[5] = RANK
            for i in STATS:
                if lines[i] and bk[i].isdigit() and int(bk[i]) > 0:
                    r[i] = str(round(int(bk[i])*lines[i](level)**2))
            r[55:58] = [f'ma_{part}_{nat.NUM}_{lod}.mod' for lod in (1, 2, 3)]
            r[58:60] = [f'bm_ma_{part}_icon.tga']*2
            r[61:64] = [f'fe_{part}_{nat.NUM}_{lod}.mod' for lod in (1, 2, 3)]
            r[64:66] = [f'bm_fe_{part}_icon.tga']*2
            new.append(r)
            definitions.append(dict(id=first+level, level=level, name=r[2], type=int(r[4]), rank=int(RANK),
                                    defense=int(r[6]), bonus_col34=int(r[33]), magic_defense=int(r[36]),
                                    price=int(r[26]), black_knight=dict(id=black+level, defense=int(bk[6])),
                                    red_knight=dict(id=red+level, defense=int(rk[6]))))
        defense = [int(r[6]) for r in new[-21:]]
        assert defense == sorted(defense) and len(set(defense)) == 21, (part, defense)
        for i in STATS:
            column = [int(r[i]) for r in new[-21:]]
            if [int(b[i]) for b in bks] == sorted(int(b[i]) for b in bks):
                assert column == sorted(column), (part, i, column)
        steps = {i: [round(lines[i](L), 4) for L in (0, 20)] for i in (6, 33, 36) if lines[i]}
        definitions[-21]['step_ratio_at_0_and_20'] = steps
    return new, definitions


def merge(raw, values):
    ids = {int(r[0]) for r in values}
    before = rows(raw)
    lines = [l for l in raw.decode('cp949').splitlines() if not (l.split('\t')[0].isdigit() and int(l.split('\t')[0]) in ids)]
    last = max(i for i, l in enumerate(lines) if l.split('\t')[0].isdigit())
    lines[last+1:last+1] = ['\t'.join(r) for r in values]
    out = ('\r\n'.join(lines)+'\r\n').encode('cp949')
    after = rows(out)
    assert all(after[i] == r for i, r in before.items() if i not in ids)
    assert all(after[int(r[0])] == r for r in values)
    return out


def catalogues(new):
    for rel in ['game-data/DATA/ITEM_DATA.txt', 'client-overlay/Item/ITEM.dat']:
        p = R/rel
        p.write_bytes(merge(p.read_bytes(), new))
    p = D/'Interface/item.dat'
    raw = p.read_bytes()
    ui = []
    for r in new:
        u = r[:5]+['0']+r[5:]
        u[2] = u[3] = r[2].split(' ', 1)[1] if r[1] != '0' else r[2]  # base name, as the Twilight weapons
        assert len(('(+20)'+u[2]).encode('cp949')) < 32
        ui.append(u)
    data = merge(zlib.decompress(raw[20:]), ui)
    p.write_bytes(raw[:16]+struct.pack('<I', len(data))+zlib.compress(data, 9))


def enhance(new):
    p = R/'src/server/LAQIA_GameServer/gm_enhance_data.h'
    raw = p.read_bytes()
    text = raw.decode('cp949')
    head = text[:text.index('static const GMEnhanceEntry')]
    entries = [tuple(map(int, g)) for g in re.findall(r'\{(\d+),(\d+),(\d+)\}', text)]
    ids = {int(r[0]) for r in new}
    entries = [e for e in entries if e[0] not in ids]+[(int(r[0]), int(r[0])-int(r[1]), int(r[1])) for r in new]
    nl = '\r\n' if '\r\n' in text else '\n'
    body = 'static const GMEnhanceEntry gmEnhanceEntries[]={'+nl+''.join('{%d,%d,%d},' % e+nl for e in sorted(entries))+'};'+nl
    p.write_bytes((head+body).encode('cp949'))


def warehouse():
    """Add the 세트 group after the existing three Power sets in both catalogue copies."""
    for path, encoding in [(R/'src/server/utf8/gm_catalog_data.utf8.h', 'utf-8'),
                           (R/'src/server/LAQIA_GameServer/gm_catalog_data.h', 'cp949')]:
        text = path.read_bytes().decode(encoding)
        nl = '\r\n' if '\r\n' in text else '\n'
        lines = text.split(nl)
        groups = re.search(r'gmCatalogGroups\[\]=\{(.*)\};', text).group(1)
        names = re.findall(r'"([^"]*)"', groups)
        if GROUP not in names:
            names.append(GROUP)
            text = text.replace('gmCatalogGroups[]={'+groups+'};', 'gmCatalogGroups[]={'+','.join(f'"{n}"' for n in names)+'};')
            lines = text.split(nl)
        group = names.index(GROUP)
        lines = [l for l in lines if not re.match(r'\{\d+,"[^"]*",0,%d,\d+\},$' % group, l)]
        last = max(i for i, l in enumerate(lines) if re.match(r'\{\d+,"[^"]*",0,\d+,\d+\},$', l))
        lines[last+1:last+1] = ['{%d,"%s",0,%d,%d},' % (i, n, group, slot) for i, n, slot in SET_ENTRIES]
        path.write_bytes(nl.join(lines).encode(encoding))
    a = (R/'src/server/utf8/gm_catalog_data.utf8.h').read_bytes().decode('utf-8')
    b = (R/'src/server/LAQIA_GameServer/gm_catalog_data.h').read_bytes().decode('cp949')
    assert a == b
    return group


def main():
    base = json.loads((O/'armor-sets.json').read_text(encoding='utf-8'))
    bones = {s: base['people'][s]['idle']['bones'] for s in ('0', '1')}
    files, summary = nat.build(bones)
    # The native models must be exactly the approved preview geometry.
    preview = json.loads((O/'elf-bluemoon.json').read_text(encoding='utf-8'))
    card = next(s for s in preview['sets'] if s['key'] == 'elf-bk-prev')
    for sex, prefix in (('0', 'ma'), ('1', 'fe')):
        def shape(chunks):
            return sorted((len(c['points']), len(c['corners']), sum(sum(p) for p in c['points'])) for c in chunks)
        want = shape(card['parts'][sex])
        got = shape(c for part in nat.PARTS for c in model(files[f'Body/High/{prefix}_{part}_{nat.NUM}_1.mod']))
        assert len(got) == len(want) and all(a[:2] == b[:2] and abs(a[2]-b[2]) < 1e-2 for a, b in zip(got, want)), prefix
    images = icons_tool.build(files, bones)
    for key, image in images.items():
        for folder in ('Item', 'Texture/Body'):
            files[f'{folder}/bm_{key}_icon.wtm'] = nat.wtm(image)
    for rel, data in files.items():
        (D/rel).parent.mkdir(parents=True, exist_ok=True)
        (D/rel).write_bytes(data)
    new, definitions = item_rows()
    catalogues(new)
    enhance(new)
    group = warehouse()
    report = dict(rank=int(RANK), formula='BK*(BK/RK)^2 per level and column', group=dict(index=group, name=GROUP,
                  entries=SET_ENTRIES), models=summary,
                  files={f'client-overlay/{rel}': hashlib.sha256(data).hexdigest() for rel, data in sorted(files.items())})
    (O/'registration.json').write_text(json.dumps(report, ensure_ascii=False, indent=2)+'\n', encoding='utf-8')
    (O/'item_definitions.json').write_text(json.dumps(definitions, ensure_ascii=False, indent=2)+'\n', encoding='utf-8')
    print(json.dumps([(d['id'], d['name'], d['defense'], d['black_knight']['defense']) for d in definitions
                      if d['level'] in (0, 10, 20)], ensure_ascii=False))


if __name__ == '__main__':
    main()
