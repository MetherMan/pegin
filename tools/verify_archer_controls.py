"""Check archer target followers and independent native rendering controls."""
from pathlib import Path
import base64
import copy
import json
import struct
import sys

R = Path(__file__).resolve().parents[1]
D = R / 'client-overlay'
H = D / 'Tools/SkillColors'
sys.dont_write_bytecode = True
sys.path.insert(0, str(H))
from native_assets import load_preview, png_from_wtm, read_magic, read_wed, read_wem
from tuning import compile_resources, load_settings, resource_reader


def trace_records(raw):
    version, count = struct.unpack_from('<2i', raw)
    assert version == 100
    records, at = [], 8
    for _ in range(count):
        begin, names = at, []
        for _ in range(2):
            length = struct.unpack_from('<i', raw, at)[0]
            at += 4
            names.append(raw[at:at + length].rstrip(b'\0').decode('cp949'))
            at += length
        at += 8
        records.append((names[0], raw[begin:at]))
    assert at == len(raw)
    return records


def main():
    read = resource_reader(D)
    saved = load_settings(H)
    colors = json.loads((H / 'colors.json').read_text())
    base, meta = compile_resources(read, saved, colors)
    variants = [('heaven', 'mt_heaven', 3, 'mh'),
                ('six', 'mt_six', 6, 'ms'), ('six', 'mt_six_blue', 6, 'msb')]
    follower_counts = {}
    for kind, script, count, prefix in variants:
        text = base['Magic/' + script + '.ms'].decode('cp949')
        assert text.count('[BLOWTIMING]') == count
        effects = read_magic(text)
        shots = [e for e in effects if any('HITSOUND' in p for p in e['parts'])]
        followers = [e for e in effects if not e['wed'] and any('TAIL' in p for p in e['parts'])]
        assert len(shots) == count
        assert len(followers) == count * (4 if kind == 'heaven' else 1)
        assert all(e['trace'] == prefix + '_tuned_trace' for e in shots)
        for follower in followers:
            at = follower['parts'][0]['STARTTIME']
            shot = next(e for e in shots if e['parts'][0]['STARTTIME'] == at)
            assert follower['parts'][0] == shot['parts'][0]
            flight = {k: v for k, v in follower['parts'][1].items() if k != 'NOSHOW'}
            assert flight == shot['parts'][1], (script, at)
            end = follower['parts'][-1]
            assert end['ENEMY'] and end['NOSHOW'] and 'HITSOUND' not in end
            assert end['POS'][:2] == [0, 0] and end['TARGET'][:2] == [0, 0]
            assert abs(flight['TARGET'][2] + end['POS'][2] - end['TARGET'][2]) < 1e-5
            if kind == 'heaven':
                assert end['TARGET'] == flight['TARGET'], 'Arrow hit effects must follow the struck body, not the ground'
                assert end['POS'] == [0, 0, 0]
                assert not any(tag in end['TAIL'] for tag in ('rune', 'wave', 'seal'))
            effect_data = read_wed(base['Effect/' + end['TAIL']])
            assert all(1 <= f[12] <= effect_data['cols'] * effect_data['rows'] for f in effect_data['frames'])
            assert effect_data['frames'][0][11] & 0xffffff == 0
            assert effect_data['frames'][-1][11] & 0xffffff == 0
            # The entry frame is invisible until the native ENEMY update sets
            # the current target orientation; no rotated flash appears in flight.
        follower_counts[script] = len(followers)

    aliases = {'mh_tuned_trace', 'ms_tuned_trace', 'msb_tuned_trace'}
    original_records = [(name, raw) for name, raw in trace_records(read('Effect/trace.tdf')) if name not in aliases]
    compiled_records = trace_records(base['Effect/trace.tdf'])
    assert [(name, raw) for name, raw in compiled_records if name not in aliases] == original_records
    assert all(sum(name == alias for name, _ in compiled_records) == 1 for alias in aliases)
    assert 'Texture/Effect/arw01.wtm' not in base and 'Texture/Effect/mt_arrow_trace.wtm' not in base
    repeated, _ = compile_resources(lambda name: base[name] if name in base else read(name), saved, colors)
    assert repeated['Effect/trace.tdf'] == base['Effect/trace.tdf']
    for alias in aliases:
        path = 'Texture/Effect/' + alias + '.wtm'
        assert repeated[path] == base[path]
    preview = load_preview(H, D, generated=base, metadata=meta, compact=True)
    assert aliases <= set(preview['traces'])
    full_preview = load_preview(H, D, generated=base, metadata=meta, compact=False)
    for name, data in preview['meshes'].items():
        assert data == full_preview['meshes'][name] == read_wem(base['Effect/' + name])
    for name, url in preview['textures'].items():
        assert url == full_preview['textures'][name]
        assert base64.b64decode(url.split(',', 1)[1]) == png_from_wtm(base['Texture/Effect/' + name])
    assert all(alias + '.wtm' in preview['textures'] for alias in aliases)
    assert 'mh_target_shards.WEM' in preview['meshes']
    assert {'mh_pierce_core.WEM', 'mh_pierce_glow.WEM'} <= preview['meshes'].keys()
    for name in ('mh_pierce_core.WEM', 'mh_pierce_glow.WEM'):
        verts = preview['meshes'][name]['vertices']
        assert min(v[2] for v in verts) < -5 and max(v[2] for v in verts) > 2
        assert max(abs(v[0]) for v in verts) <= .121 and max(abs(v[1]) for v in verts) <= .121
    for name in ('mh_pierce_core.wed', 'mh_pierce_glow.wed'):
        fx = read_wed(base['Effect/' + name])
        assert all(frame[:3] == [0, 0, 0] and frame[6] == 0 for frame in fx['frames'])
        assert fx['mesh'].startswith('mh_pierce_')
    assert read_wed(base['Effect/mh_vb0001.wed'])['mesh'] == read_wed(read('Effect/vb0001.wed'))['mesh']
    assert read_wed(base['Effect/mh_target_burst.wed'])['mesh'] == read_wed(read('Effect/lightningarrow_a01.wed'))['mesh']
    assert meta['heaven']['originalImpactRestored'] and meta['heaven']['additivePiercingLayers'] == 2
    assert not any(path in base for path in ('Effect/mh_target_rune.WEM', 'Effect/mh_target_rune.wed', 'Effect/mh_target_wave.wed'))
    assert meta['heaven']['groundLayers'] == 0

    controls = []
    for kind in ('heaven', 'six'):
        prefix = 'mh' if kind == 'heaven' else 'msb'
        projectile = 'mh_ps0002_1.wed' if kind == 'heaven' else 'msb_projectile.wed'
        main_tail = 'mh_vb0001.wed' if kind == 'heaven' else 'msb_impact.wed'
        expected = {
            'brightness': [projectile, prefix + '_target_burst.wed'],
            'intensity': [prefix + '_charge.wed', prefix + '_target_burst.wed'],
            'projectileSize': [projectile],
            'launchSize': [prefix + '_launch.wed', prefix + '_charge.wed'],
            'impactSize': [main_tail, prefix + '_target_burst.wed'],
        }
        if kind == 'heaven':
            expected['areaSize'] = ['mh_pierce_core.wed', 'mh_pierce_glow.wed']
            expected['pierceWidth'] = ['mh_pierce_core.wed', 'mh_pierce_glow.wed']
        for key, changed_effects in expected.items():
            cfg = copy.deepcopy(saved)
            cfg[kind][key] = .6 if key != 'impactSize' else 1.1
            if cfg[kind][key] == saved[kind][key]:
                cfg[kind][key] = 1.3
            out, changed_meta = compile_resources(read, cfg, colors)
            for name in changed_effects:
                assert out['Effect/' + name] != base['Effect/' + name], (kind, key, name)
            if kind == 'heaven' and key in ('areaSize', 'pierceWidth'):
                assert out['Effect/mh_vb0001.wed'] == base['Effect/mh_vb0001.wed']
                assert out['Effect/mh_target_burst.wed'] == base['Effect/mh_target_burst.wed']
            if key == 'intensity':
                assert out['Effect/' + projectile] == base['Effect/' + projectile]
                assert out['Effect/' + main_tail] == base['Effect/' + main_tail]
            if key == 'brightness':
                texture_name = prefix + '_tuned_trace.wtm'
                assert out['Texture/Effect/' + texture_name] != base['Texture/Effect/' + texture_name]
                update = load_preview(H, D, generated=out, metadata=changed_meta, compact=True)
                url = update['textures'][texture_name]
                assert url != preview['textures'][texture_name]
                assert base64.b64decode(url.split(',', 1)[1]) == png_from_wtm(out['Texture/Effect/' + texture_name])
            if key == 'launchSize':
                for name in (prefix + '_launch.wed', prefix + '_charge.wed'):
                    old = read_wed(base['Effect/' + name])['frames']
                    new = read_wed(out['Effect/' + name])['frames']
                    assert all(abs(a[1] * a[8] - b[1] * b[8]) < 1e-5 for a, b in zip(old, new))
            controls.append(kind + '.' + key)
        cfg = copy.deepcopy(saved)
        cfg[kind]['impactHeight'] = saved[kind]['impactHeight'] + .3
        if cfg[kind]['impactHeight'] > 4:
            cfg[kind]['impactHeight'] = saved[kind]['impactHeight'] - .3
        delta = cfg[kind]['impactHeight'] - saved[kind]['impactHeight']
        out, changed_meta = compile_resources(read, cfg, colors)
        assert all(abs(b - a - delta) < 1e-6 for a, b in zip(meta[kind]['targetHeights'], changed_meta[kind]['targetHeights']))
        controls.append(kind + '.impactHeight')

    report = dict(passed=True, controls=controls, damage_events=[3, 6, 6],
                  target_followers=follower_counts, original_trace_records_preserved=len(original_records),
                  trace_aliases_idempotent=True, compact_native_mesh_texture_parity=True,
                  in_game_visual_test=False)
    (R / 'assets/skills140/archer-controls-validation.json').write_text(
        json.dumps(report, ensure_ascii=False, indent=2) + '\n', encoding='utf-8')
    print('PASS archer:', len(controls), 'native controls; target-synchronous followers; 3/6/6 hits; trace preservation and idempotence')


if __name__ == '__main__':
    main()
