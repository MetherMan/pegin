"""Native archer shots and arrival-linked target effects, without extra damage."""
import math
from pathlib import Path
import struct
import zlib

from native_vfx import envelope, frames, mesh, rgb


def _piercing_mesh(read, out, name, width, glow=False):
    """A tapered, straight lance on local -Z (the native firing direction)."""
    vertices, indices = [], []
    # Both ends visibly extend beyond the struck body. Two crossed ribbons
    # keep the same straight silhouette from the overhead and side cameras.
    stations = [(2.2, 0), (1.65, width), (0, width), (-4.4, width * .45), (-5.2, 0)]
    for plane in range(2):
        start = len(vertices)
        for z, half_width in stations:
            v = .24 + .5 * (2.2 - z) / 7.4
            for side in (-1, 1):
                u = .5 + side * (.37 if glow else .035)
                vertices.append([side * half_width if plane == 0 else 0,
                                 side * half_width if plane == 1 else 0, z, u, v])
        for station in range(len(stations) - 1):
            base = start + station * 2
            indices.extend(base + i for i in (0, 1, 3, 0, 3, 2, 3, 1, 0, 2, 3, 0))
    mesh(read, out, name, vertices, indices)


def _trace_resources(read, out, cfg):
    """Clone only these spells' traces; keep every other table record verbatim."""
    original = read('Effect/trace.tdf')
    version, count = struct.unpack_from('<2i', original)
    if version != 100 or not 0 < count < 1000:
        raise ValueError('Unsupported native trace table')
    at, records, lookup = 8, [], {}
    for _ in range(count):
        begin, names = at, []
        for _ in range(2):
            length = struct.unpack_from('<i', original, at)[0]
            at += 4
            names.append(original[at:at + length].rstrip(b'\0').decode('cp949'))
            at += length
        values = original[at:at + 8]
        at += 8
        records.append((names[0], original[begin:at]))
        lookup[names[0]] = (names[1], values)
    if at != len(original):
        raise ValueError('Invalid native trace table length')
    aliases = ('mh_tuned_trace', 'ms_tuned_trace', 'msb_tuned_trace')
    kept = [raw for name, raw in records if name not in aliases]

    def string(value):
        encoded = value.encode('cp949') + b'\0'
        return struct.pack('<i', len(encoded)) + encoded

    for kind, prefix, source in (('heaven', 'mh', 'mt_arrow_trace'),
                                 ('six', 'ms', '활'), ('six', 'msb', 'mt_arrow_trace')):
        texture, values = lookup[source]
        raw = read('Texture/Effect/' + Path(texture).with_suffix('.wtm').name)
        gain = cfg[kind].get('brightness', 1)
        if gain != 1:
            bmp = bytearray(zlib.decompress(raw[13:]))
            if bmp[:2] != b'BM':
                raise ValueError('Unsupported arrow trace bitmap')
            offset = struct.unpack_from('<I', bmp, 10)[0]
            width, height = struct.unpack_from('<ii', bmp, 18)
            bits = struct.unpack_from('<H', bmp, 28)[0]
            if bits not in (8, 24, 32) or struct.unpack_from('<I', bmp, 30)[0]:
                raise ValueError('Unsupported arrow trace pixel format')
            if bits == 8:
                begin = 14 + struct.unpack_from('<I', bmp, 14)[0]
                channels = (at + channel for at in range(begin, offset, 4) for channel in range(3))
            else:
                stride = ((width * bits + 31) // 32) * 4
                channels = (offset + y * stride + x * (bits // 8) + channel
                            for y in range(abs(height)) for x in range(width) for channel in range(3))
            for at in channels:
                bmp[at] = min(255, round(bmp[at] * gain))
            raw = raw[:9] + struct.pack('<I', len(bmp)) + zlib.compress(bmp, 9)
        out['Texture/Effect/' + prefix + '_tuned_trace.wtm'] = raw
        kept.append(string(prefix + '_tuned_trace') + string(prefix + '_tuned_trace.bmp') + values)
    out['Effect/trace.tdf'] = struct.pack('<2i', version, len(kept)) + b''.join(kept)


def build(read, out, wed, part, effect, sound, blow, cfg, emit, only=None):
    """Compile all three archer scripts and return heaven/six timing metadata.

    Brightness scales all light. Intensity controls the supporting charge,
    target rays, shards and area layers independently of the main projectile.
    Every target layer repeats the real BONE/ATTACK flight; no caster-relative
    timer is used to guess when a moving enemy is hit.
    """
    meta = {}
    _trace_resources(read, out, cfg)
    for kind, key, count, source, trace in (
        ('heaven', 'mt_heaven', 3, 'ps0002_1.wed', 'mt_arrow_trace'),
        ('six', 'mt_six', 6, 'arrow01.wed', '활'),
        ('six', 'mt_six_blue', 6, 'mt_arrow.wed', 'mt_arrow_trace'),
    ):
        if only is not None and kind not in only:
            continue
        v = cfg[kind]
        blue = key != 'mt_six'
        prefix = 'mh' if kind == 'heaven' else ('msb' if blue else 'ms')
        trace = prefix + '_tuned_trace'
        tint = '#72DFFF' if blue else '#EAF5FF'
        brightness = v.get('brightness', 1)
        intensity = v.get('intensity', 1)
        projectile_size = v.get('projectileSize', 1)
        launch_size = v.get('launchSize', 1)
        impact_size = v.get('impactSize', 1)
        impact_offset = v.get('impactHeight', 0)
        area_size = v.get('areaSize', 1)
        support_gain = brightness * intensity

        arrow_name = 'mh_ps0002_1.wed' if kind == 'heaven' else prefix + '_projectile.wed'
        arrow = wed(source, arrow_name, tint,
                    (1.35 if kind == 'heaven' else 1) * projectile_size,
                    brightness, origin=(0, 0, 0))
        if kind == 'heaven':
            _piercing_mesh(read, out, 'mh_pierce_core.WEM', .028)
            _piercing_mesh(read, out, 'mh_pierce_glow.WEM', .12, glow=True)

            def pierce_transform(mesh_name, glow=False):
                def transform(names, source_frames):
                    names[0] = mesh_name

                    def update(f, t, index):
                        width = v['pierceWidth'] * (1 - .55 * min(1, t / 500))
                        f[:3] = [0, 0, 0]
                        f[3:7] = [0, 1, 0, 0]
                        f[7] = 1
                        f[8:11] = [width, width, area_size * (.82 + .18 * min(1, t / 55))]
                        gain = (support_gain * .52 if glow else brightness) * envelope(t, 500, 26, 230)
                        f[11] = rgb('#72DFFF' if glow else '#D9FAFF', gain)
                        f[12] = 1

                    return names, frames(source_frames, 500, update)
                return transform

            # The original impact remains the damaging arrow's own tail.
            # Straight penetration is an additional arrival-linked layer.
            tail = wed('vb0001.wed', 'mh_vb0001.wed', tint,
                       1.8 * impact_size, brightness, origin=(0, 0, 0))
        else:
            tail = wed('ta_arrow01.wed', prefix + '_impact.wed', tint,
                       2.2 * impact_size, 1.35 * brightness, origin=(0, 0, 0))

        launch_scale = (2.6 if kind == 'heaven' else 1.8) * launch_size
        launch = wed('ps0001.wed' if kind == 'heaven' else 'arrow_a01.wed',
                     prefix + '_launch.wed', tint, launch_scale,
                     1.25 * brightness, origin=(0, 1.25 / launch_scale, 0))
        charge_scale = 2.2 * launch_size
        charge = wed('ps0002.wed', prefix + '_charge.wed', tint,
                     charge_scale, 1.2 * support_gain,
                     origin=(0, 1.25 / charge_scale, 0))

        def burst_transform(names, source_frames):
            duration = 620 if kind == 'heaven' else 430

            def update(f, t, index):
                u = min(1, t / (190 if kind == 'heaven' else 110))
                size = impact_size * ((.9 + .85 * u) if kind == 'heaven' else (.19 + .19 * u))
                f[:3] = [0, 0, 0]
                f[3:7] = [0, 1, 0, t * .06]
                f[7] = 1
                f[8:11] = [size] * 3
                f[11] = rgb(tint, support_gain * envelope(t, duration, 32, 360) * .8)
                cells = 8 if kind == 'heaven' else 16
                f[12] = min(cells, 1 + int(t * cells / duration))

            return names, frames(source_frames, duration, update)

        burst = wed('lightningarrow_a01.wed' if kind == 'heaven' else 'ta_arrowice01.wed',
                    prefix + '_target_burst.wed', tint, transform=burst_transform)

        area_layers = []
        if kind == 'heaven':
            out['Effect/mh_target_shards.WEM'] = read('Effect/mh_target_shards.WEM')
            def shards_transform(names, source_frames):
                names[0] = 'mh_target_shards.WEM'
                def update(f, t, index):
                    growth = .3 + 1.1 * min(1, t / 190)
                    f[:3] = [0, .1 * min(1, t / 150), 0]
                    f[3:7] = [0, 1, 0, t * .16]
                    f[7] = 1
                    f[8:11] = [impact_size * growth * 1.3, impact_size * growth, impact_size * growth]
                    f[11] = rgb(tint, support_gain * envelope(t, 740, 32, 400) * .66)
                    f[12] = 1
                return names, frames(source_frames, 740, update)
            shards = wed('ps0002.wed', 'mh_target_shards.wed', tint, transform=shards_transform)
            core = wed('ps0002_1.wed', 'mh_pierce_core.wed', tint,
                       transform=pierce_transform('mh_pierce_core.WEM'))
            glow = wed('ps0002_1.wed', 'mh_pierce_glow.wed', tint,
                       transform=pierce_transform('mh_pierce_glow.WEM', True))
            area_layers = [(shards, 740), (core, 500), (glow, 500)]

        s = effect(charge, part(
            f'[POS] 0 0.65 0\n[STARTTIME] {max(0, v["startDelay"] - 240)}\n[TIMELIMIT] 300'))
        base_heights = [.72, 1.35, 1.02] if count == 3 else [.7, 1.25, .9, 1.48, .78, 1.15]
        heights = [height + impact_offset for height in base_heights]

        def flight_parts(at, height, invisible=False):
            p = part(f'[NOSHOW]\n[BONE] 14\n[STARTTIME] {at}\n[TIMELIMIT] 5')
            p += part(('[NOSHOW]\n' if invisible else '') +
                      f'[ATTACK]\n[POS] 0 0 1.3\n[TARGET] 0 0 {height:.6f}\n'
                      f'[MOVETYPE] 1\n[TIMELIMIT] 0\n[SPEED] {v["flightSpeed"]}\n[TRACEDIST] {0.085 * projectile_size:.6f}')
            return p

        def arrival(at, height, tail_wed, duration):
            p = flight_parts(at, height, True)
            p += part(f'[NOSHOW]\n[ENEMY]\n[TARGET] 0 0 {height:.6f}\n'
                      f'[POS] 0 0 {0:.6f}\n[TAIL] {tail_wed}\n[TIMELIMIT] {duration}')
            return effect(None, p)

        for index, height in enumerate(heights):
            at = v['startDelay'] + index * v['shotGap']
            s += sound('arrow100048.wav', at)
            s += effect(launch, part(f'[POS] 0 0.7 0\n[STARTTIME] {at}\n[TIMELIMIT] 300'))
            p = flight_parts(at, height)
            p += part(f'[NOSHOW]\n[ENEMY]\n[TARGET] 0 0 {height:.6f}\n[POS] 0 0 0\n' +
                      blow('attack100049.wav') + f'\n[TAIL] {tail}\n[TIMELIMIT] 500')
            s += effect(arrow, p, True, trace)
            s += arrival(at, height, burst, 620 if kind == 'heaven' else 430)
            for layer, duration in area_layers:
                s += arrival(at, height, layer, duration)

        emit(key, s)
        meta[kind] = dict(
            first=v['startDelay'], last=v['startDelay'] + (count - 1) * v['shotGap'],
            targetHeights=heights, motionEnd=2500, targetExtrasPerArrow=1 + len(area_layers),
            damageEvents=count, brightness=brightness, intensity=intensity,
            projectileSize=projectile_size, launchSize=launch_size,
            impactSize=impact_size, impactHeight=impact_offset,
        )
        if kind == 'heaven':
            meta[kind].update(areaSize=area_size, targetAreaRadius=0,
                              targetShards=1, groundLayers=0, originalImpactRestored=True,
                              additivePiercingLayers=2, pierceWidth=v['pierceWidth'],
                              piercingLength=7.4 * area_size, piercingAxis='shot direction',
                              impactStyle='original impact plus straight body penetration')
    return meta
