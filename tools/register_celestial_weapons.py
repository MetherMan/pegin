"""Register the Golden Knight "천상의 날개" (Celestial Wings) weapons: Rank 10, +0..+20.

Models: the approved single-weapon Codex art traced to 3D by tools/build_golden_knight_models.py
(same method and lengths as the Twilight weapons), exported to native MOD + full-resolution 2048
atlas with the matching game weapon as reference (orientation, winding, header).
Stats: every enchant level copies the Twilight weapon of the same type (Rank 9) and multiplies each
numeric column by one more rank step: the least-squares line of Black Knight / Red Knight (Rank
6/5) ratios of that weapon type, as for the Blue Moon armour. Enchanting needs no code.
Icons: the art itself turned diagonal, on the black tile with the 1px gold frame of the Twilight /
Black Knight icons. Also the GM enhance table, the warehouse 세트 group "천상의 날개" and the sword
trail (Equip/Trace.txt, 검흔C like the paladin greatsword). Idempotent.
"""
from pathlib import Path
import hashlib, json, re, shutil, struct, sys, zlib

R = Path(__file__).resolve().parents[1]
sys.path[:0] = [str(R/'.cache/weapon-build-deps'), str(R/'runtime/pylibs'), str(R/'assets/twilight-set'),
                str(R/'assets/twilight'), str(R/'tools')]
import build_golden_knight_models as models
from mesh_tools import export_native, np, Image
from noble_moon_atlas import full_resolution_atlas
from verify_closed_surfaces import audit
from PIL import ImageFilter
import build_bluemoon_armor_icons as armor_icons
from register_bluemoon_armor import rows, merge, step_line
import bluemoon_armor_native as nat

O = R/'assets/golden-knight'
D = R/'client-overlay'
GAME = R/'runtime/client/GameClient'
RANK = '10'
GROUP = '천상의 날개'
# kind: (item name, first id, Twilight first id, (Black Knight peer, Red Knight peer), model prefix, reference MOD, shelf slot)
FAMILIES = {'sword': ('천상의 날개 양손검', 19260, 19030, (11424, 11403), 'cw_sword', 'ws_0081_1.mod', 2),
            'longbow': ('천상의 날개 롱보우', 19290, 19060, (11739, 11718), 'cw_longbow', 'wb_0040_1.mod', 7),
            'staff': ('천상의 날개 스태프', 19320, 19090, (11865, 11844), 'cw_staff', 'ww_0015_1.mod', 9)}
STATS = [6, 7, 8, 9, 24, 25, 26, 67, 68]
TRAIL = 'celestial_gold'  # gold copy of 검흔C, tools/build_celestial_effects.py


def native():
    files, report = {}, {}
    specs = {s['kind']: s for s in json.loads(models.SPEC.read_text(encoding='utf-8'))['weapons']}
    out = O/'payload'
    for kind, (_, _, _, _, prefix, ref, _) in FAMILIES.items():
        spec = specs[kind]
        parts, images, scale, polys = models.parts_for(spec)
        result = export_native(out, prefix, parts, images, GAME/'Equip'/ref, atlas_packer=full_resolution_atlas)
        surface = audit(out/f'{prefix}_1.mod')
        for lod in (1, 2, 3):
            files[f'Equip/{prefix}_{lod}.mod'] = (out/f'{prefix}_{lod}.mod').read_bytes()
        files[f'Texture/Equip/{prefix}_atlas.wtm'] = (out/f'{prefix}_atlas.wtm').read_bytes()
        report[kind] = dict(prefix=prefix, reference=ref, scale=scale, triangles=result['triangles'],
                            vertices=result['vertices'], bounds=result['bounds'], polygons=len(polys),
                            closed_surface=surface.get('passed'), surface=surface)
        if kind == 'sword':
            grip = spec['grip'][1]
            report[kind]['trail'] = [round((grip-spec['blade_root_y'])*scale, 4), round((grip-spec['tip_y'])*scale*.98, 4)]
    return files, report


# Icon framing like the Twilight icons: zoom onto the signature part and let the rest run past the
# frame. (focus x, y in the art crop, window size in rotated pixels)
ICON_FOCUS = {'sword': (316, 860, 760), 'longbow': (190, 690, 900), 'staff': (226, 260, 560)}


def icon(kind):
    """The weapon art laid diagonally (tip to the upper right) inside the gold-framed black tile."""
    from PIL import ImageEnhance
    spec = next(s for s in json.loads(models.SPEC.read_text(encoding='utf-8'))['weapons'] if s['kind'] == kind)
    art = Image.open(R/spec['source']).convert('RGB').crop(spec['box'])
    art = ImageEnhance.Contrast(ImageEnhance.Color(art).enhance(1.35)).enhance(1.12)
    rgb = np.asarray(art, float)
    alpha = np.clip((rgb.max(2)-18)/40, 0, 1)
    rgba = Image.fromarray(np.dstack([rgb, alpha*255]).astype(np.uint8), 'RGBA')
    w, h = rgba.size
    rotated = rgba.rotate(-45, Image.BICUBIC, expand=True)
    fx, fy, window = ICON_FOCUS[kind]
    # Same rotation for the focus point: about the image centre, clockwise 45 degrees, then shifted
    # into the expanded canvas.
    a = np.radians(45)
    cx, cy = fx-w/2, fy-h/2
    rx, ry = cx*np.cos(a)-cy*np.sin(a), cx*np.sin(a)+cy*np.cos(a)
    px, py = rx+rotated.width/2, ry+rotated.height/2
    half = window/2
    view = rotated.crop((round(px-half), round(py-half), round(px+half), round(py+half)))
    inner, ss = 26, 8
    side = inner*ss
    view = view.resize((side, side), Image.LANCZOS)
    tile = Image.new('RGB', (side, side), (0, 0, 0))
    tile.paste(view, (0, 0), view)
    small = tile.resize((inner, inner), Image.BOX)
    bk, gold = armor_icons.frame()
    out = np.zeros((28, 28, 3))
    out[1:27, 1:27] = np.asarray(small, float)
    out = np.where(gold[..., None], bk, out)
    sharp = np.asarray(Image.fromarray(np.clip(out+.5, 0, 255).astype(np.uint8)).filter(ImageFilter.UnsharpMask(1, 60, 2)), float)
    return Image.fromarray(np.where(gold[..., None], bk, sharp).astype(np.uint8))


def item_rows():
    source = rows((R/'game-data/DATA/ITEM_DATA.txt').read_bytes())
    new, definitions = [], []
    for kind, (name, first, twilight, (black, red), prefix, _, _) in FAMILIES.items():
        lines = {i: step_line([source[black+L] for L in range(21)], [source[red+L] for L in range(21)], i) for i in STATS}
        for level in range(21):
            tw = source[twilight+level]
            assert tw[1] == str(level)
            r = tw.copy()
            r[0] = str(first+level)
            r[2] = r[3] = (f'+{level} ' if level else '')+name
            assert len(r[2].encode('cp949')) < 32
            r[5] = RANK
            for i in STATS:
                if lines[i] and tw[i].isdigit() and int(tw[i]) > 0:
                    r[i] = str(round(int(tw[i])*lines[i](level)))
            r[55:58] = [f'{prefix}_{lod}.mod' for lod in (1, 2, 3)]
            r[58:60] = [f'{prefix}_icon.tga']*2
            r[61:64] = [f'{prefix}_{lod}.mod' for lod in (1, 2, 3)]
            r[64:66] = [f'{prefix}_icon.tga']*2
            new.append(r)
            definitions.append(dict(id=first+level, level=level, name=r[2], type=int(r[4]), rank=int(RANK),
                                    min=int(r[6]), max=int(r[7]), magic_min=int(r[8]), magic_max=int(r[9]),
                                    price=int(r[26]), twilight=dict(id=twilight+level, min=int(tw[6]), max=int(tw[7]))))
        for i in (6, 7, 8, 9):
            column = [int(r[i]) for r in new[-21:]]
            assert column == sorted(column), (kind, i, column)
    return new, definitions


def catalogues(new):
    for rel in ['game-data/DATA/ITEM_DATA.txt', 'client-overlay/Item/ITEM.dat']:
        p = R/rel
        p.write_bytes(merge(p.read_bytes(), new))
    p = D/'Interface/item.dat'
    raw = p.read_bytes()
    ui = []
    for r in new:
        u = r[:5]+['0']+r[5:]
        u[2] = u[3] = r[2].split(' ', 1)[1] if r[1] != '0' else r[2]
        assert len(('(+20)'+u[2]).encode('cp949')) < 32
        ui.append(u)
    data = merge(zlib.decompress(raw[20:]), ui)
    p.write_bytes(raw[:16]+struct.pack('<I', len(data))+zlib.compress(data, 9))


def enhance(new):
    p = R/'src/server/LAQIA_GameServer/gm_enhance_data.h'
    text = p.read_bytes().decode('cp949')
    head = text[:text.index('static const GMEnhanceEntry')]
    entries = [tuple(map(int, g)) for g in re.findall(r'\{(\d+),(\d+),(\d+)\}', text)]
    ids = {int(r[0]) for r in new}
    entries = [e for e in entries if e[0] not in ids]+[(int(r[0]), int(r[0])-int(r[1]), int(r[1])) for r in new]
    nl = '\r\n' if '\r\n' in text else '\n'
    body = 'static const GMEnhanceEntry gmEnhanceEntries[]={'+nl+''.join('{%d,%d,%d},' % e+nl for e in sorted(entries))+'};'+nl
    p.write_bytes((head+body).encode('cp949'))


def warehouse():
    entries = [(first, name, slot) for name, first, _, _, _, _, slot in FAMILIES.values()]
    for path, encoding in [(R/'src/server/utf8/gm_catalog_data.utf8.h', 'utf-8'),
                           (R/'src/server/LAQIA_GameServer/gm_catalog_data.h', 'cp949')]:
        text = path.read_bytes().decode(encoding)
        nl = '\r\n' if '\r\n' in text else '\n'
        groups = re.search(r'gmCatalogGroups\[\]=\{(.*)\};', text).group(1)
        names = re.findall(r'"([^"]*)"', groups)
        if GROUP not in names:
            names.append(GROUP)
            text = text.replace('gmCatalogGroups[]={'+groups+'};', 'gmCatalogGroups[]={'+','.join(f'"{n}"' for n in names)+'};')
        lines = text.split(nl)
        group = names.index(GROUP)
        lines = [l for l in lines if not re.match(r'\{\d+,"[^"]*",0,%d,\d+\},$' % group, l)]
        last = max(i for i, l in enumerate(lines) if re.match(r'\{\d+,"[^"]*",0,\d+,\d+\},$', l))
        lines[last+1:last+1] = ['{%d,"%s",0,%d,%d},' % (i, n, group, slot) for i, n, slot in entries]
        path.write_bytes(nl.join(lines).encode(encoding))
    a = (R/'src/server/utf8/gm_catalog_data.utf8.h').read_bytes().decode('utf-8')
    b = (R/'src/server/LAQIA_GameServer/gm_catalog_data.h').read_bytes().decode('cp949')
    assert a == b
    return group


def trail(start, end):
    p = D/'Equip/Trace.txt'
    raw = p.read_bytes()
    text = raw.decode('cp949')
    nl = '\r\n' if '\r\n' in text else '\n'
    lines = [l for l in text.split(nl) if not l.startswith('cw_sword_')]
    while lines and not lines[-1].strip():
        lines.pop()
    lines += [f'cw_sword_{lod}.mod {TRAIL} {start:.7f} 0.0 0.0 {end:.7f} 0.0 0.0' for lod in (1, 2, 3)]
    p.write_bytes((nl.join(lines)+nl).encode('cp949'))


def main():
    files, model_report = native()
    (O/'icons').mkdir(parents=True, exist_ok=True)
    for kind, (_, _, _, _, prefix, _, _) in FAMILIES.items():
        image = icon(kind)
        image.save(O/'icons'/f'{prefix}.png')
        for folder in ('Item', 'Texture/Body'):
            files[f'{folder}/{prefix}_icon.wtm'] = nat.wtm(image)
    for rel, data in files.items():
        (D/rel).parent.mkdir(parents=True, exist_ok=True)
        (D/rel).write_bytes(data)
    trail(*model_report['sword']['trail'])
    new, definitions = item_rows()
    catalogues(new)
    enhance(new)
    group = warehouse()
    report = dict(rank=int(RANK), formula='Twilight x fitted (Black Knight / Red Knight) step per level and column',
                  group=dict(index=group, name=GROUP), trail=TRAIL, models=model_report,
                  files={f'client-overlay/{rel}': hashlib.sha256(data).hexdigest() for rel, data in sorted(files.items())})
    (O/'registration.json').write_text(json.dumps(report, ensure_ascii=False, indent=2)+'\n', encoding='utf-8')
    (O/'item_definitions.json').write_text(json.dumps(definitions, ensure_ascii=False, indent=2)+'\n', encoding='utf-8')
    print(json.dumps({k: (v['triangles'], v['closed_surface']) for k, v in model_report.items()}))
    print(json.dumps([(d['id'], d['name'], d['min'], d['max'], d['twilight']['min'], d['twilight']['max'])
                      for d in definitions if d['level'] in (0, 20)], ensure_ascii=False))


if __name__ == '__main__':
    main()
