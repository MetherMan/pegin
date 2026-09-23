"""Tune generated native WED colours without a preview-only render override."""
import struct


def shade_generated(out, before, prefix, brightness=1, intensity=1, core=()):
    """Brightness is global light; intensity changes only auxiliary-layer weight."""
    for key in out.keys() - before:
        if not key.startswith('Effect/' + prefix) or not key.lower().endswith('.wed'):
            continue
        gain = brightness * (1 if key.rsplit('/', 1)[-1] in core else intensity)
        if gain == 1:
            continue
        raw = bytearray(out[key])
        at = 31
        for _ in range(2):
            length = struct.unpack_from('<i', raw, at)[0]
            at += 4 + length
        count = struct.unpack_from('<i', raw, at + 8)[0]
        at += 20
        for i in range(count):
            offset = at + i * 52 + 44
            rgba = struct.unpack_from('<I', raw, offset)[0]
            channels = [min(255, max(0, round(((rgba >> shift) & 255) * gain))) for shift in (16, 8, 0)]
            struct.pack_into('<I', raw, offset, (rgba & 0xff000000) | channels[0] << 16 | channels[1] << 8 | channels[2])
        out[key] = bytes(raw)
