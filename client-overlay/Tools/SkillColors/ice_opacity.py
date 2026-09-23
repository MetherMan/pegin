"""The same narrow solid-ice selection used by the native renderer patch."""


def is_ice_surface(name):
    name = name.replace('\\', '/').rsplit('/', 1)[-1].lower()
    return (name == 'mf_cocytus_pillar.wed'
            or (name.startswith(('mf_eruption_', 'mf_crown_spike')) and name.endswith('.wed')))


def with_opacity(color, percent, fade):
    """Keep RGB exactly as authored; only the native ARGB alpha byte changes."""
    alpha = round(255 * percent / 100 * max(0, min(1, fade)))
    return (color & 0x00ffffff) | (alpha << 24)
