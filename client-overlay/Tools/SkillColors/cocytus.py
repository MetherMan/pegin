"""Broken, stationary ice clusters erupt in outside-in bands before the monolith."""
import math
import random
import colorsys
from native_vfx import plane, arcane_texture, frames, rgb, envelope
from cocytus_shapes import crag, ring_crystal, snow_cloud, squall_arc, frozen_band, mist_planes, monolith
from skill_tuning_frames import shade_generated
from cocytus_layout import broken_ring
from ice_opacity import is_ice_surface, with_opacity


DEFAULT_ICE_COLOR = '#2374D8'


def ice_palette_color(authored, selected):
    """Keep authored face/rim shade differences; default is byte-exact identity."""
    if selected.upper() == DEFAULT_ICE_COLOR:
        return authored
    def hsv(value):
        return colorsys.rgb_to_hsv(*(int(value[i:i+2], 16) / 255 for i in (1, 3, 5)))
    base_h, base_s, base_v = hsv(DEFAULT_ICE_COLOR)
    chosen_h, chosen_s, chosen_v = hsv(selected)
    h, s, v = hsv(authored)
    channels = colorsys.hsv_to_rgb((h + chosen_h - base_h) % 1,
                                  min(1, s * chosen_s / base_s),
                                  min(1, v * chosen_v / base_v))
    return '#' + ''.join(f'{round(channel * 255):02X}' for channel in channels)


def is_ice_body_color(name):
    name = name.replace('\\', '/').rsplit('/', 1)[-1].lower()
    return is_ice_surface(name) or name == 'mf_cocytus_pillar_rim.wed' or (name.startswith('mf_burst_shard') and name.endswith('.wed'))


def build(read, out, wed, part, effect, sound, blow, f, color, ice_color=DEFAULT_ICE_COLOR):
    start = f['startDelay']
    extent = f['distance'] * .65
    outer_radius, inner_radius = extent * f['inwardStartRadius'], extent * f['inwardEndRadius']
    ring_radii = [outer_radius + (inner_radius - outer_radius) * i / 4 for i in range(5)]
    # A zero arrival radius belongs to the final central pillar, not a collapsed ring.
    ring_radii = [r for r in ring_radii if r > 1e-5]
    stagger_ms = round(min(115, 35 + f['stepGap'] * .55))
    ring_gap = round(max(110 + f['stepGap'] * 1.1, 100 / f['riseSpeed'] + stagger_ms + 20))
    ring_times = [start + i * ring_gap for i in range(len(ring_radii))]
    eruption_duration = ring_times[-1] - start + stagger_ms + round(100 / f['riseSpeed'])
    seal_start = start + round(ring_gap * 1.5)
    pillar_at = start + eruption_duration + 120
    end = pillar_at + 2400
    floor_end = end + 1650
    life = end - pillar_at
    brightness = f['brightness']
    density = f['iceDensity']
    # Only body-related layers use this palette. Cast/seals retain their own colour.
    body_colors = {value: ice_palette_color(value, ice_color)
                   for value in ('#9ADFFF', '#164BD0', '#2374D8', '#266DDC', '#AEDEFA')}
    # The original texture is itself blue. For a custom colour, use the existing
    # native compiler's neutral luminance copy so red/white do not stay blue.
    # The original blue selection keeps the exact original texture and bytes.
    body_texture_color = None if ice_color.upper() == DEFAULT_ICE_COLOR else ice_color
    s = sound('mb_0009_A.wav')
    monolith(read, out, 'mf_crystal.WEM', 'mf_crystal_rim.WEM')
    mist_planes(read, out, 'mf_cold_mist.WEM')
    # The shipped bsmoke image is copied unchanged into the effect texture path.
    out['Texture/Effect/mf_cold_mist.wtm'] = read('Texture/Particle/ob_11.wtm')
    for i in range(9):
        crag(read, out, f'mf_broken_crag{i}.WEM', 370 + i)
        ring_crystal(read, out, f'mf_ring_crystal{i}.WEM', 170 + i)
    seal_textures = [arcane_texture(read, out, 'mf', n) for n in ('jin02', 'jin03_2', 'jin01_2')]

    def enemy(name, x, y, z, at, duration, hit=False):
        return effect(name, part(f'[ENEMY]\n[TARGET] {x:.5f} {y:.5f} {z:.5f}\n[STARTTIME] {at}\n[TIMELIMIT] {duration}' + ('\n' + blow() if hit else '')))

    def ice_rgb(tint, gain):
        # Preserve the blue face hue when density and light are both pushed up.
        return rgb(tint, gain / (1 + max(0, gain - 1) * .35))

    def crystal(name, x, y, at, duration, width, height, angle, variant=0, primary=False, rim=False, ring=False, depth=None, rise_ms=None, surface_gain=None):
        def transform(n, fs):
            n[0] = ('mf_crystal_rim.WEM' if rim else 'mf_crystal.WEM') if primary else (f'mf_ring_crystal{variant % 9}.WEM' if ring else f'mf_broken_crag{variant % 9}.WEM')
            def update(g, t, j):
                rise_time = (rise_ms if rise_ms is not None else 92 if primary else 48 + (variant % 4) * 11) / f['riseSpeed']
                p = min(1, t / rise_time)
                rise = 1 - (1 - p) ** 3
                # Brief overshoot makes an impact instead of slowly extruding a pole.
                rise *= 1 + (.06 if primary else 0 if ring else .035) * max(0, 1 - abs(t - rise_time) / 125)
                g[:3] = [0, 0, 0]
                g[3:7] = [0, 1, 0, angle]
                g[7] = 1
                g[8:11] = [max(.001, height * rise / 10.2), width, width if depth is None else depth]
                if rim:
                    g[11] = rgb(body_colors['#9ADFFF'], envelope(t, duration, 25, 550) * brightness * .25)
                else:
                    gain = surface_gain if surface_gain is not None else .80 if primary else .56 + .035 * (variant % 3)
                    fade = envelope(t, duration, 24, min(400, duration * .53))
                    g[11] = with_opacity(ice_rgb(body_colors['#164BD0' if primary else '#2374D8'], fade * brightness ** .55 * density * gain), f['iceOpacity'], fade)
                g[12] = 1
            return n, frames(fs, duration, update)
        return enemy(wed('spikeice02.wed', name, body_texture_color, transform=transform), x, y, .025, at, duration)

    # Circular phases still progress outside-in, but each contains uneven groups,
    # gaps, broken slabs and leaning spears. Roots never slide or wash back out.
    rng = random.Random(230926)
    stations, ring_counts, fog_puffs = 0, [], 0
    band_half_widths, ring_end_times, cluster_counts = [], [], []
    for ring, (radius, at) in enumerate(zip(ring_radii, ring_times)):
        count = max(8, min(64, round(math.tau * radius / (f['size'] * .82))))
        ring_counts.append(count)
        phase = math.radians(f['spreadAngle']) + ring * .19
        half_width = min((outer_radius - inner_radius) / 4 * .42, radius * .38)
        band_half_widths.append(half_width)
        crystals = broken_ring(radius, half_width, count, ring, phase, f['size'], stagger_ms)
        ring_end_times.append(at + max(c['at'] for c in crystals))
        cluster_counts.append(len({c['cluster'] for c in crystals}))
        for c in crystals:
            crystal_at = at + c['at']
            s += crystal(f'mf_eruption_{stations}.wed', c['x'], c['y'], crystal_at, end - crystal_at, c['width'], c['height'], c['yaw'], c['variant'], ring=True, depth=c['depth'], rise_ms=c['riseMs'], surface_gain=c['gain'])
            stations += 1
        # Fog stays around its own ring. No inward/outward advection or sliding.
        for i in range(0 if f['mistAmount'] == 0 else max(1, round(2 * f['mistAmount']))):
            count_mist = max(1, round(2 * f['mistAmount']))
            c = crystals[(i * 7 + ring) % len(crystals)]
            a = math.atan2(c['y'], c['x'])
            duration = end - at
            sx = min(f['size'] * 1.15, math.tau * radius / count_mist * .36)
            def ring_mist(n, fs, a=a, sx=sx, duration=duration):
                n[0], n[1] = 'mf_cold_mist.WEM', 'mf_cold_mist.bmp'
                def update(g, t, j):
                    g[:3] = [0, 0, 0]
                    g[3:7] = [0, 1, 0, math.degrees(a)]
                    g[7] = 1; g[8:11] = [1.45, sx, sx]
                    g[11] = rgb('#71B7E8', envelope(t, duration, 130, 600) * math.sqrt(f['mistAmount']) * .24)
                    g[12] = 1
                return n, frames(fs, duration, update)
            s += enemy(wed('cloudkill01.wed', f'mf_ring_mist{ring}_{i}.wed', transform=ring_mist), c['x'], c['y'], .03, at, duration)
            fog_puffs += 1
    aftershocks = 0

    seal_heights = [.14 + f['sealLowerHeight'], f['size'] * 3.0 + f['sealMiddleHeight'], f['size'] * 6.4 + f['sealUpperHeight']]
    for layer, (radius, height) in enumerate(zip([extent * .97, extent * .66, extent * .45], seal_heights)):
        radius *= f['sealSize']
        name = f'mf_cocytus_seal{layer}'
        plane(read, out, name + '.WEM')
        at = seal_start + layer * 100
        duration = end - at
        def seal(n, fs, name=name, layer=layer, radius=radius, duration=duration):
            n[0], n[1] = name + '.WEM', seal_textures[layer]
            def update(g, t, j):
                g[:3] = [0, 0, 0]
                g[3:7] = [0, 1, 0, (1 if layer != 1 else -1) * t * .026 + layer * 31]
                g[7] = 1
                g[8:11] = [radius * (.66 + .34 * min(1, t / 150))] * 3
                g[11] = rgb('#528AE1' if layer != 1 else color, envelope(t, duration, 90, 500) * (.31 if layer == 0 else .43))
                g[12] = 1
            return n, frames(fs, duration, update)
        s += enemy(wed('hell02.wed', name + '.wed', '#A9DEFF', transform=seal), f['sealOffsetX'], f['sealOffsetY'], height, at, duration)

    s += sound('mb_0009_B.wav', pillar_at)
    s += crystal('mf_cocytus_pillar.wed', 0, 0, pillar_at, life, f['size'] * 1.56, f['size'] * 8.7, 19, primary=True)
    s += crystal('mf_cocytus_pillar_rim.wed', 0, 0, pillar_at, life, f['size'] * 1.56, f['size'] * 8.7, 19, primary=True, rim=True)
    radial_spikes = 11
    for i in range(radial_spikes):
        a = i * 2.399963 + rng.uniform(-.15, .15)
        at = pillar_at + 35 + rng.randrange(0, 95)
        duration = end - at
        tilt, width, length = rng.uniform(32, 64), rng.uniform(.28, .47), rng.uniform(.29, .49)
        def crown(n, fs, i=i, a=a, duration=duration, tilt=tilt, width=width, length=length):
            n[0] = f'mf_broken_crag{i % 9}.WEM'
            def update(g, t, j):
                grow = 1 - (1 - min(1, t / (130 / f['riseSpeed']))) ** 3
                g[:3] = [0, 0, 0]
                g[7] = 1
                g[3:7] = [math.cos(a), 0, math.sin(a), tilt]
                g[8:11] = [max(.001, f['size'] * length * grow), f['size'] * width, f['size'] * width]
                fade = envelope(t, duration, 28, 550)
                g[11] = with_opacity(ice_rgb(body_colors['#266DDC'], fade * brightness ** .55 * density * .46), f['iceOpacity'], fade)
                g[12] = 1
            return n, frames(fs, duration, update)
        s += enemy(wed('spikeice02.wed', f'mf_crown_spike{i}.wed', body_texture_color, transform=crown), math.sin(a) * 1.35, math.cos(a) * 1.35, .03, at, duration)

    def ripple(n, fs):
        def update(g, t, j):
            p = min(1, t / 560)
            g[:3] = [0, 0, 0]
            g[7] = 1
            g[8:11] = [1 + extent * 4.6 * (1 - (1 - p) ** 2)] * 3
            g[11] = rgb('#59A5E5', envelope(t, 850, 28, 650) * f['mistStrength'] * .10)
        return n, frames(fs, 850, update)
    wave = wed('pajang02.wed', 'mf_pajang02.wed', '#79D8FA', transform=ripple)
    for i in range(3):
        s += enemy(wave, 0, 0, .07, pillar_at + i * 180, 850, True)

    # Native WED trajectories fling shards away from the central rupture.
    burst_shards = 18
    for i in range(burst_shards):
        a, reach = rng.random() * math.tau, extent * rng.uniform(.8, 1.2)
        peak, width = rng.uniform(1.5, 4.3), rng.uniform(.13, .24) * f['size']
        duration = 690 + rng.randrange(0, 200)
        def burst(n, fs, i=i, a=a, reach=reach, peak=peak, width=width, duration=duration):
            n[0] = 'ice02.WEM'
            def update(g, t, j):
                p = min(1, t / duration)
                radius = .7 + reach * (1 - (1 - p) ** 1.7)
                scale = width * (1 - .35 * p)
                g[:3] = [math.cos(a) * radius / scale, (.5 + 4 * peak * p * (1 - p)) / scale, math.sin(a) * radius / scale]
                g[3:7] = [.7, 1, .3, i * 37 + t * .40]
                g[7] = 1
                g[8:11] = [scale] * 3
                g[11] = rgb(body_colors['#AEDEFA'], envelope(t, duration, 22, 350) * brightness * .62)
                g[12] = 1
            return n, frames(fs, duration, update)
        s += enemy(wed('spikeice01.wed', f'mf_burst_shard{i}.wed', body_texture_color, transform=burst), 0, 0, .10, pillar_at + 20 + i * 5, duration)

    blizzard_at, blizzard_end = pillar_at - 110, end + 260
    snow_count = max(1, round(96 * f['blizzardStrength']))
    for layer in range(3):
        name = f'mf_blizzard{layer}'
        snow_cloud(read, out, name + '.WEM', 897 + layer, extent, snow_count)
        at = blizzard_at + layer * 65
        duration = blizzard_end - at
        def snow(n, fs, name=name, layer=layer, duration=duration):
            n[0] = name + '.WEM'
            def update(g, t, j):
                scale = 1 + layer * .025
                g[:3] = [0, (.22 * math.sin(t * .006 + layer) - .00018 * t) / scale, 0]
                g[3:7] = [0, 1, 0, layer * 107 + t * (.23 + layer * .05)]
                g[7] = 1
                g[8:11] = [scale] * 3
                g[11] = rgb('#EDF7FF', envelope(t, duration, 160, 650) * min(1.8, f['mistStrength']) * .53)
                g[12] = 1
            return n, frames(fs, duration, update)
        s += enemy(wed('spikeice01.wed', name + '.wed', '#EDF7FF', transform=snow), 0, 0, .2, at, duration)
    for i in range(4):
        name = f'mf_squall{i}'
        squall_arc(read, out, name + '.WEM', 564 + i, extent)
        at, duration = pillar_at + i * 55, end - (pillar_at + i * 55)
        def gust(n, fs, name=name, i=i, duration=duration):
            n[0] = name + '.WEM'
            def update(g, t, j):
                g[:3] = [0, 0, 0]
                g[3:7] = [0, 1, 0, i * 97 + t * (.14 + i * .023)]
                g[7] = 1
                g[8:11] = [1] * 3
                g[11] = rgb('#C0E9FA', envelope(t, duration, 190, 750) * min(1.8, f['mistStrength']) * .33 * math.sqrt(f['blizzardStrength']))
                g[12] = 1
            return n, frames(fs, duration, update)
        s += enemy(wed('winds_a01.wed', name + '.wed', '#8DDDF3', transform=gust), 0, 0, .15, at, duration)

    read('Texture/Effect/mf_frozen_ground.wtm')
    floor_times = []
    for band in range(6):
        at = start + round(eruption_duration * (.66 + [.06,.17,.02,.12,0,.20][band]))
        floor_times.append(at)
        duration = floor_end - at
        name = f'mf_frozen_ground{band}'
        frozen_band(read, out, name + '.WEM', band, 6, extent)
        def floor(n, fs, name=name, duration=duration):
            n[0], n[1] = name + '.WEM', 'mf_frozen_ground.bmp'
            def update(g, t, j):
                g[:3] = [0, 0, 0]
                g[3:7] = [0, 1, 0, 0]
                g[7] = 1
                g[8:11] = [1, 1, 1]
                g[11] = rgb('#FFFFFF', envelope(t, duration, 130, 1500) * .65)
                g[12] = 1
            return n, frames(fs, duration, update)
        s += enemy(wed('fireball01.wed', name + '.wed', transform=floor), 0, 0, .02, at, duration)
    core = tuple(key.rsplit('/', 1)[-1] for key in out if key.startswith(('Effect/mf_eruption_', 'Effect/mf_crown_spike', 'Effect/mf_cocytus_pillar')) and key.endswith('.wed'))
    shade_generated(out, set(), 'mf_', intensity=f['intensity'], core=core)
    return s, dict(first=start, last=end, sealStart=seal_start, pillarAt=pillar_at, fadeEnd=end, floorEnd=floor_end,
                   distance=f['distance'], spreadAngle=f['spreadAngle'], stations=stations, aftershocks=aftershocks,
                   radialSpikes=radial_spikes, seals=3, ringTimes=ring_times, ringRadii=ring_radii, ringCounts=ring_counts, rings=len(ring_radii), floorTimes=floor_times,
                   ringEndTimes=ring_end_times, ringBandHalfWidths=band_half_widths, ringClusters=cluster_counts,
                   eruptionDuration=eruption_duration, burstShards=burst_shards, blizzardLayers=3, squallArcs=4,
                   pressureFronts=0, outerRadius=outer_radius, innerRadius=inner_radius,
                   iceMaterial='source-alpha-solid-ice-with-additive-rim', supportsTrueOpacity=True,
                   iceOpacity=f['iceOpacity'], nativeRendererRequirement='ice-opacity-v1',
                   iceColor=ice_color, iceColorSurfaces=sorted(key.rsplit('/', 1)[-1] for key in out if key.startswith('Effect/') and is_ice_body_color(key)),
                   alphaSurfaces=sorted(key.rsplit('/', 1)[-1] for key in out if key.startswith('Effect/') and is_ice_surface(key)),
                   mistPuffs=fog_puffs, mistAmount=f['mistAmount'],
                   blizzardAt=blizzard_at, blizzardEnd=blizzard_end, snowParticlesPerLayer=snow_count,
                   sealHeights=seal_heights, sealOffset=[f['sealOffsetX'], f['sealOffsetY']], pillarRiseMs=92 / f['riseSpeed'],
                   layout='stationary-broken-ice-ring-clusters', motionEnd=1500)
