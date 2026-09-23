"""Deterministic size conversion from original Baphomet motions and 1.4 sculpt."""
from pathlib import Path
import hashlib, struct, sys

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / 'client-overlay/Tools/SkillColors'))
from native_actor import animation

BASELINE_SCALE = 1.4
TARGET_SCALE = 1.7
GRIP_PIVOT = (-2.92578, 3.26140, 0.0)
HAND_BONE = 14


def scale_values(raw, offset, count, factor):
    values = struct.unpack_from('<' + str(count) + 'f', raw, offset)
    struct.pack_into('<' + str(count) + 'f', raw, offset, *(v * factor for v in values))


def scaled_animation(source, factor=TARGET_SCALE):
    raw = bytearray(source)
    frames, meshes = struct.unpack_from('<2i', raw, 32)
    at = 40
    for _ in range(meshes):
        at += 4
        scale_values(raw, at, frames * 3, factor)
        at += frames * 28
    bones = struct.unpack_from('<i', raw, at)[0]
    at += 4
    for _ in range(bones):
        at += 32
        scale_values(raw, at + 48, 3, factor)
        at += 68
    for _ in range(bones):
        scale_values(raw, at, frames * 3, factor)
        at += frames * 28
    assert at == len(raw)
    original = animation(source, list(range(frames)))
    enlarged = animation(raw, list(range(frames)))
    assert len(original['bones']) == len(enlarged['bones']) == 29
    for a, b in zip(original['bones'], enlarged['bones']):
        assert a['name'] == b['name']
        assert all(abs(y - x * (factor if i in (12, 13, 14) else 1)) < 1e-5
                   for i, (x, y) in enumerate(zip(a['rest'], b['rest'])))
        for p, q in zip(a['poses'], b['poses']):
            assert p[3:] == q[3:]
            assert all(abs(q[i] - p[i] * factor) < 1e-5 for i in range(3))
    return bytes(raw), original, enlarged


def scale_chunks(chunks, factor):
    for chunk in chunks:
        chunk['points'] = [[v * factor for v in p] for p in chunk['points']]
        for i in (12, 13, 14):
            chunk['matrix'][i] *= factor


def rebuild_motions(data, source_dir, output_dir, factor=TARGET_SCALE):
    """Always scale p-warrior originals; never scale a prior mt_prime output."""
    reports = {}
    for name in data['clips']:
        assert name.startswith('p-warrior_bla_') and name.endswith('.ani'), name
        source = (source_dir / name).read_bytes()
        raw, original, enlarged = scaled_animation(source, factor)
        output = 'mt_prime_' + name.removeprefix('p-warrior_bla_')
        destination = output_dir / output
        destination.parent.mkdir(parents=True, exist_ok=True)
        destination.write_bytes(raw)
        data['clips'][name] = dict(original=original, primordial=enlarged)
        reports[name] = dict(output=output, frames=original['frames'],
                             source_sha256=hashlib.sha256(source).hexdigest(),
                             sha256=hashlib.sha256(raw).hexdigest())
    return reports
