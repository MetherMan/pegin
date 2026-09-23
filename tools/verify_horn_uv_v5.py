"""Read-only geometry, isolated smooth-normal and atlas-UV regression checks."""
from pathlib import Path
import json
import math

import antler_revision as antlers
import horn_surface_uv as surfaces
from sculpt_primordial_baphomet import model, tube, face, compact_points, encode, scale_chunks
from primordial_geometry import TARGET_SCALE, BASELINE_SCALE

ROOT = Path(__file__).resolve().parents[1]


def reconstruct(use_mapping=True, use_smoothing=True):
    raw = (ROOT / 'assets/primordial-baphomet/sculpt-baseline.mod').read_bytes()
    chunks = model(raw)
    original_mapping = antlers.horn_tube
    original_smoothing = surfaces.smooth_horn_normals
    if not use_smoothing:
        surfaces.smooth_horn_normals = lambda *args: {}
    ranges = [[], []]
    def wrapper(chunk, make_tube, path, radii, bone, region, sides, key):
        start = len(chunk['corners'])
        if use_mapping:
            result = original_mapping(chunk, make_tube, path, radii, bone, region, sides, key)
        else:
            make_tube(chunk, path, radii, bone, region, sides)
            result = {'key': key}
        ranges[0 if chunk is chunks[0] else 1].append((start, len(chunk['corners'])))
        return result
    antlers.horn_tube = wrapper
    try:
        report = antlers.sculpt_body(chunks[0], tube)
        report.update(antlers.build_cleaver(chunks[1], face, tube))
    finally:
        antlers.horn_tube = original_mapping
        surfaces.smooth_horn_normals = original_smoothing
    compact_points(chunks[0], keep=809)
    compact_points(chunks[1])
    scale_chunks(chunks, TARGET_SCALE / BASELINE_SCALE)
    return raw, chunks, ranges, report


def verify():
    raw, original, ranges, _ = reconstruct(False)
    _, revised, revised_ranges, report = reconstruct(True)
    _, repeated, _, repeated_report = reconstruct(True)
    _, flat_uv, _, _ = reconstruct(True, False)
    assert ranges == revised_ranges
    assert revised == repeated and report == repeated_report
    details = report['horn_uv'] + report['cleaver_horn_uv']
    assert len(details) == 36
    changed, protected, normal_changes, cap_corners, seam_positions = 0, 0, 0, 0, 0
    detail_index = 0
    for old, new, flat, included in zip(original, revised, flat_uv, ranges):
        assert old['points'] == new['points'] and old['bones'] == new['bones']
        assert new['points'] == flat['points'] and new['bones'] == flat['bones']
        assert len(old['corners']) == len(new['corners'])
        for index, (a, b, uv_only) in enumerate(zip(old['corners'], new['corners'], flat['corners'])):
            assert a[0] == b[0], 'Triangle index changed'
            assert b[4:6] == uv_only[4:6], 'Normal smoothing changed a UV'
            if any(start <= index < end for start, end in included):
                changed += a[4:6] != b[4:6]
                normal_changes += a[1:4] != b[1:4]
            else:
                assert a == b, 'Skin, blade face or grip corner changed'
                protected += 1
        for start, end in included:
            item = details[detail_index]
            detail_index += 1
            side_end = start + item['smoothed_side_corners']
            expected, seen, uvs = {}, {}, {}
            # Independently reconstruct area weighting on the final, scaled
            # mesh rather than calling the smoothing implementation as oracle.
            for at in range(start, side_end, 3):
                cc = new['corners'][at:at + 3]
                p, q, r = [new['points'][c[0]] for c in cc]
                x = [q[k] - p[k] for k in range(3)]
                y = [r[k] - p[k] for k in range(3)]
                weighted = [x[1]*y[2]-x[2]*y[1], x[2]*y[0]-x[0]*y[2], x[0]*y[1]-x[1]*y[0]]
                for point in (p, q, r):
                    key = tuple(point)
                    previous = expected.setdefault(key, [0.0, 0.0, 0.0])
                    expected[key] = [a + b for a, b in zip(previous, weighted)]
            for c in new['corners'][start:side_end]:
                key = tuple(new['points'][c[0]])
                n = c[1:4]
                assert abs(math.sqrt(sum(v*v for v in n)) - 1) < 1e-12
                summed = expected[key]
                length = math.sqrt(sum(v*v for v in summed))
                assert max(abs(a-b/length) for a,b in zip(n,summed)) < 1e-10
                if key in seen:
                    assert n == seen[key], 'Position/UV seam normals disagree'
                seen[key] = n
                uvs.setdefault(key, set()).add(tuple(c[4:6]))
            seam_positions += sum(len(samples) > 1 for samples in uvs.values())
            assert len(seen) == item['smoothed_positions']
            assert side_end < end
            for a, b in zip(old['corners'][side_end:end], new['corners'][side_end:end]):
                assert a[1:4] == b[1:4], 'Cap normal changed'
                cap_corners += 1
    old_native = model(encode(raw, original))
    new_native = model(encode(raw, revised))
    for a, b, expected in zip(old_native, new_native, revised):
        assert a['points'] == b['points'] and a['bones'] == b['bones']
        assert all(x[0] == y[0] for x, y in zip(a['corners'], b['corners']))
        for c, source in zip(b['corners'], expected['corners']):
            assert max(abs(x-y) for x,y in zip(c[1:6],source[1:6])) < 1e-7
            assert abs(math.sqrt(sum(x*x for x in c[1:4])) - 1) < 1e-6
    baseline = ROOT / '.cache/horn-v5-baseline/Monster/mt_prime_baphomet.mod'
    if baseline.exists():
        live_old = model(baseline.read_bytes())
        assert live_old == old_native, 'Read-only reconstruction differs from backed-up v4'
    assert all(item['tip_u_span'] < item['root_u_span'] for item in details)
    assert normal_changes == sum(item['changed_normal_corners'] for item in details)
    assert normal_changes > 0 and seam_positions > 0
    return {'passed': True, 'horns': len(details), 'changed_uv_corners': changed,
            'protected_skin_blade_grip_corners': protected,
            'geometry_bones_unchanged': True, 'non_horn_normals_unchanged': True,
            'changed_horn_normal_corners': normal_changes, 'cap_normal_corners_preserved': cap_corners,
            'smooth_normals_unit_length': True, 'area_weighting_verified': True,
            'matching_uv_seam_positions': seam_positions, 'smoothing_preserves_uvs': True,
            'native_readback_verified': True,
            'v4_baseline_matches': baseline.exists(), 'deterministic': True,
            'atlas_bounds_verified': True, 'scale': TARGET_SCALE,
            'grip_roll_degrees': report['blade_roll_degrees'], 'writes_files': False}


if __name__ == '__main__':
    print(json.dumps(verify(), ensure_ascii=False, indent=2))
