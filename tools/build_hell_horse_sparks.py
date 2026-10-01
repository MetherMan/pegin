"""Build small, short-lived native flames at hell-horse hoof contacts.

The contact hook supplies each animated hoof's actual landing position.
The unique tick marker also scopes the renderer's fog correction to this row.
Rebuilding this table alone never rewrites either executable or renderer.
"""
from pathlib import Path
import argparse
import hashlib
import json
import math
import struct
import sys

R = Path(__file__).resolve().parents[1]
sys.path[:0] = [str(R/'runtime/pylibs'), str(R/'.cache/weapon-build-deps'), str(R/'tools')]

PARTICLE_NAME = 'mt_hell_hoof'
HEADER = 30
STRIDE = 761
# A multiple of (12 - 1) keeps native color/size interpolation in bounds for
# every living millisecond. The movement hook budgets one particle per emission.
LIFETIME_MS = 385
PARTICLES_PER_EMISSION = 1
HOOF_FOG_MARKER = 140041


def name(row):
    return row[:125].split(b'\0')[0].decode('cp949')


def records(raw):
    assert raw[:22] == b'#W3DParticleInfo File\0'
    version, count = struct.unpack_from('<2i', raw, 22)
    assert version == 101 and len(raw) == HEADER + count * STRIDE
    rows = [raw[HEADER+i*STRIDE:HEADER+(i+1)*STRIDE] for i in range(count)]
    assert len({name(row) for row in rows}) == count, 'Duplicate particle names'
    return rows


def fire_trail(source):
    row = bytearray(source)
    row[:125] = PARTICLE_NAME.encode().ljust(125, b'\0')
    # A stamp stays at the landing point after the hoof lifts. nTick is NOT an
    # emission interval here; it only controls randomized initial particle age.
    # A private large value gives full life and identifies the no-fog draw hook.
    struct.pack_into('<4i5f2i', row, 125,
                     100, PARTICLES_PER_EMISSION, HOOF_FOG_MARKER, LIFETIME_MS,
                     0, 0, 0, 0, 0, 0, 12)
    for i in range(12):
        age = i / 11
        glow = (1 - age) ** 1.15
        # Native SetWorld uses homogeneous W: side length = .06 / W.
        # Requested 1.2x enlargement of the former .22 -> .20 billboard.
        inverse_size = .06 / (1.2 * (.22 - .02 * age))
        struct.pack_into('<7f', row, 425+i*28,
                         160*glow, 56*glow, 4*glow,
                         0, 0, 0, inverse_size)
    return bytes(row)


def build_table(raw):
    rows = records(raw)
    source = next(row for row in rows if name(row) == '불')
    trail = fire_trail(source)
    replaced = False
    result = []
    for row in rows:
        if name(row) == PARTICLE_NAME:
            result.append(trail)
            replaced = True
        else:
            result.append(row)
    if not replaced:
        result.append(trail)
    header = bytearray(raw[:HEADER])
    struct.pack_into('<i', header, 26, len(result))
    return bytes(header) + b''.join(result)


def verify_table(before, after):
    old_rows, new_rows = records(before), records(after)
    original = [row for row in old_rows if name(row) != PARTICLE_NAME]
    assert original == [row for row in new_rows if name(row) != PARTICLE_NAME]
    assert build_table(after) == after, 'Rebuild must be byte-for-byte idempotent'
    row = next(row for row in new_rows if name(row) == PARTICLE_NAME)
    _, count, tick, life, speed, ax, ay, az, force, inverse, levels = struct.unpack_from('<4i5f2i', row, 125)
    assert count == 1 and tick == HOOF_FOG_MARKER and life == LIFETIME_MS and levels == 12
    assert (speed, ax, ay, az, force, inverse) == (0, 0, 0, 0, 0, 0)
    keys = [struct.unpack_from('<7f', row, 425+i*28) for i in range(levels)]
    assert all(math.isfinite(v) for key in keys for v in key)
    assert all(0 <= v <= 255 for key in keys for v in key[:6])
    assert all(0 < key[6] < 1 for key in keys)
    assert keys[-1][:6] == (0, 0, 0, 0, 0, 0)
    # Verify native Update indexing/positions, including the final living
    # millisecond, for every rand()%6 direction used by the shipped renderer.
    level_life = life // (levels - 1)
    for age in range(life):
        assert age // level_life + 1 < levels
        travel = (speed / 1000) * age + force * age
        for direction in range(-3, 3):
            assert (direction + ax*age) * travel == 0
            assert (direction + az*age) * travel == 0
            assert (direction + ay*age) * travel == 0
    texture = row[169:425].split(b'\0')[0].decode('cp949')
    assert texture == next(r for r in old_rows if name(r) == '불')[169:425].split(b'\0')[0].decode('cp949')
    texture_path = Path(texture).with_suffix('.wtm')
    assert any((base/texture_path).exists() for base in (R/'client-overlay', R/'runtime/client/GameClient'))
    return dict(original_particle_records_unchanged=True,
                original_record_count=len(original), table_record_count=len(new_rows),
                idempotent=True, stationary_world_space=True,
                particle_lifetime_ms=life, particles_per_emission=count,
                billboard_width_world_units=[.06/keys[0][6], .06/keys[-1][6]],
                no_shadow_tint=all(key[3:6] == (0, 0, 0) for key in keys),
                fog_scope_marker=tick,
                native_interpolation_all_living_milliseconds_checked=True,
                texture=texture, trail_sha256=hashlib.sha256(row).hexdigest(),
                in_game_visual_test=False)


def verify_existing_hook(executable, report):
    """Require hoof contacts; the former center emitter is not sufficient."""
    import pefile
    data = executable.read_bytes()
    pe = pefile.PE(data=data)
    if not any(s.Name.rstrip(b'\0') == b'.hhoof' for s in pe.sections):
        return dict(movement_fix_installed=False,
                    required_command='tools/patch_hell_horse_hoof.py --apply',
                    note='The former .htrail center emitter is superseded by animated hoof contacts.')
    from patch_hell_horse_hoof import verify, REPORT
    metadata = json.loads(REPORT.read_text(encoding='utf-8'))
    for hook in metadata['hooks']:
        for va, key in ((hook['va'], 'replacement'), (hook['target'], 'code')):
            offset = pe.get_offset_from_rva(va-pe.OPTIONAL_HEADER.ImageBase)
            expected = bytes.fromhex(hook[key])
            assert data[offset:offset+len(expected)] == expected
    return dict(movement_fix_installed=True, executable_unchanged=True,
                actual_motion_validation=verify(data, metadata))


def install_hook(executable, report):
    """Explicit opt-in only; install the contact fix on the current PE."""
    import pefile
    before = executable.read_bytes()
    if any(s.Name.rstrip(b'\0') == b'.hhoof' for s in pefile.PE(data=before).sections):
        return
    from patch_hell_horse_hoof import build, verify, REPORT
    data, metadata = build(executable)
    metadata['validation'] = verify(data, metadata)
    assert executable.read_bytes() == before
    executable.write_bytes(data)
    REPORT.write_text(json.dumps(metadata, indent=2)+'\n', encoding='utf-8')


def verify_fog_hook():
    import pefile
    from patch_hell_particle_fog import REPORT, DLL, MARKER
    assert MARKER == HOOF_FOG_MARKER
    data = DLL.read_bytes()
    pe = pefile.PE(data=data)
    if not any(s.Name.rstrip(b'\0') == b'.hpfog' for s in pe.sections):
        return dict(scoped_particle_fog_fix_installed=False)
    metadata = json.loads(REPORT.read_text(encoding='utf-8'))
    for hook in metadata['hooks']:
        for va, key in ((hook['va'], 'replacement'), (hook['target'], 'code')):
            offset = pe.get_offset_from_rva(va-pe.OPTIONAL_HEADER.ImageBase)
            expected = bytes.fromhex(hook[key])
            assert data[offset:offset+len(expected)] == expected
    return dict(scoped_particle_fog_fix_installed=True)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--install-hook', action='store_true', help='Explicitly install the animated-contact .hhoof fix')
    args = parser.parse_args()
    overlay = R/'client-overlay'
    artifacts = R/'assets/hell-horse'
    target = overlay/'Effect/particle.ptc'
    baseline = target if target.exists() else R/'runtime/client/GameClient/Effect/particle.ptc'
    before = baseline.read_bytes()
    after = build_table(before)
    validation = verify_table(before, after)
    executable, hook_report = overlay/'DeicideOnline.exe', artifacts/'spark-hook.json'
    if args.install_hook:
        install_hook(executable, hook_report)
    validation.update(verify_existing_hook(executable, hook_report))
    validation.update(verify_fog_hook())
    target.write_bytes(after)
    (artifacts/'fire-trail-validation.json').write_text(json.dumps(validation, indent=2)+'\n', encoding='utf-8')
    print(json.dumps(validation))


if __name__ == '__main__':
    main()
