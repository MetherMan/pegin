"""Deterministic broken crystal clusters inside successive circular ice bands."""
import math
import random


def broken_ring(radius, band_half_width, count, ring, phase, size, stagger_ms):
    rng = random.Random(511923 + ring * 137)
    clusters = max(3, min(8, round(count / 4.7)))
    # Unequal gaps and unequal populations leave visible breaks between groups.
    gaps = [rng.uniform(.60, 1.55) for _ in range(clusters)]
    total = sum(gaps)
    centers, angle = [], phase
    for gap in gaps:
        centers.append(angle)
        angle += math.tau * gap / total
    populations = [2] * clusters
    weights = [rng.uniform(.45, 1.7) for _ in range(clusters)]
    for _ in range(count - sum(populations)):
        populations[rng.choices(range(clusters), weights=weights)[0]] += 1
    crystals = []
    for group, (center, population) in enumerate(zip(centers, populations)):
        spread = min(.42, math.tau / clusters * rng.uniform(.13, .24))
        cluster_radius = radius + rng.uniform(-.55, .55) * band_half_width
        onset = rng.randrange(max(1, round(stagger_ms * .68)))
        for member in range(population):
            a = center + rng.uniform(-spread, spread)
            r = max(.05, min(radius + band_half_width, max(radius - band_half_width, cluster_radius + rng.uniform(-.65, .65) * band_half_width)))
            # Every group contains a jagged tall spear and squat broken slabs.
            shape = (member + group + ring) % 5
            if shape == 0:
                height, width = rng.uniform(1.65, 2.55), rng.uniform(.17, .30)
            elif shape in (1, 2):
                height, width = rng.uniform(.32, .78), rng.uniform(.31, .54)
            else:
                height, width = rng.uniform(.85, 1.65), rng.uniform(.20, .40)
            width = min(size * width, math.tau * radius / count * .68)
            width *= .65 if shape == 0 else 1.35 if shape in (1, 2) else rng.uniform(.8, 1.18)
            offset = min(stagger_ms, onset + rng.randrange(max(1, round(stagger_ms * .44))))
            crystals.append(dict(x=math.cos(a)*r, y=math.sin(a)*r, radius=r,
                at=offset, width=width, depth=width*rng.uniform(.48, 1.42),
                height=size*height, yaw=rng.uniform(0, 360), variant=rng.randrange(9),
                riseMs=rng.uniform(42, 98), gain=rng.uniform(.38, .62), cluster=group))
    # Preserve exact phase start, with a spread of later eruptions in each band.
    earliest = min(c['at'] for c in crystals)
    for crystal in crystals:
        crystal['at'] -= earliest
    return crystals
