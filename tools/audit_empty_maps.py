"""Read native map spawn markers and portal links without changing game data."""
from pathlib import Path
import argparse, collections, hashlib, json, re, struct

ROOT = Path(__file__).resolve().parents[1]


def audit(server):
    folder = server / 'MAP'
    decode = lambda p: p.read_bytes().decode('cp949')
    maps = [int(s) for s in decode(folder / 'map_list.txt').splitlines() if s.isdigit()]
    names = {int(a): b for a, b in re.findall(r'^(\d+)\s+(\S+)', decode(folder / 'map_name.txt'), re.M)}
    mobs = {int(c[0]): c for line in decode(server / 'DATA/MOB_DATA.txt').splitlines()
            if (c := line.split('\t'))[0].isdigit()}
    links = [list(map(int, line.split())) for line in decode(folder / 'loadingpoint.txt').splitlines()
             if line.strip() and not line.startswith((';', '#'))]
    result = []
    sources = {}
    for number in maps:
        path = folder / f'land{number:04d}.map'
        raw = path.read_bytes()
        sources[path.name] = hashlib.sha256(raw).hexdigest()
        version, size = struct.unpack_from('<2I', raw)
        # Same skips and 48-byte entity structure as monster.cpp::LoadMobReset.
        at = 40 + 252 * size * size
        count = struct.unpack_from('<I', raw, at)[0]
        at += 4
        assert at + count * 48 <= len(raw), path
        counts, valid, blocked = collections.Counter(), collections.Counter(), collections.Counter()
        for _ in range(count):
            model = raw[at:at + 32].split(b'\0')[0].decode('cp949')
            x, y, z, yaw = struct.unpack_from('<4f', raw, at + 32)
            at += 48
            match = re.fullmatch(r'(?:MOB_|mob_)(\d+)\.mod', model)
            if not match:
                continue
            mob = int(match[1])
            counts[mob] += 1
            assert 0 <= int(x) < size * 4 and 0 <= int(y) < size * 4
            # The server uses layer 0 movement flags for monster placement.
            if raw[40 + 73 * size * size + int(y) * size * 4 + int(x)]:
                blocked[mob] += 1
            elif any(mob + offset in mobs for offset in (0, 200, 300, 400, 500, 600)):
                valid[mob] += 1
        types = []
        for mob, n in sorted(counts.items()):
            variants = [mobs[mob + delta] for delta in (0, 200, 300, 400, 500, 600) if mob + delta in mobs]
            types.append(dict(id=mob, name=mobs.get(mob, ['', 'UNKNOWN'])[1], markers=n,
                              usable_markers=valid[mob], blocked_markers=blocked[mob],
                              possible_variants=[dict(id=int(v[0]), name=v[1], level=int(v[3])) for v in variants]))
        result.append(dict(id=number, name=names.get(number, str(number)), marker_count=sum(counts.values()),
                           usable_markers=sum(valid.values()), types=types,
                           incoming=sorted({p[0] for p in links if p[3] == number}),
                           outgoing=sorted({p[3] for p in links if p[0] == number})))
    for name in ('map_list.txt', 'map_name.txt', 'loadingpoint.txt'):
        sources[name] = hashlib.sha256((folder / name).read_bytes()).hexdigest()
    return dict(maps=result, empty_ids=[m['id'] for m in result if not m['usable_markers']],
                portals=links, source_sha256=sources,
                method='Native MAP entities + layer-0 collision + MOB_DATA definitions; no map mutation.',
                notes=['Markers are spawn locations, not the instantaneous live monster population.',
                       'Event bosses 101/102/103 use existing reset points on maps 11/12/10.',
                       'A map number alone does not establish which map precedes it.'])


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--server', type=Path, required=True)
    args = parser.parse_args()
    report = audit(args.server)
    out = ROOT / 'assets/world-audit'
    out.mkdir(exist_ok=True)
    (out / 'map-spawns.json').write_text(json.dumps(report, ensure_ascii=False, indent=2) + '\n', encoding='utf-8')
    for row in report['maps']:
        print(row['id'], row['name'], row['usable_markers'], ' / '.join(t['name'] for t in row['types']))


if __name__ == '__main__':
    main()
