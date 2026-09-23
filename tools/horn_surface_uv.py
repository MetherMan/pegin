"""Texture coordinates and smooth normals for existing horns, without remeshing.

The native atlas is twice as wide as it is high.  Using the entire horn
material for every tine both stretches a short tine and crushes the texture
around its tapered tip.  This mapping instead uses a common world-space texel
density, accumulated centerline distance and the local ring circumference.
Each horn samples a deterministic, inset crop of its assigned material tile.
"""
import hashlib
import math


def remap_horn_uv(chunk, start, path, radii, region, sides, key):
    """Replace only UV fields of the tube() corners added after ``start``.

    No wrapping outside the atlas tile is used.  The tube's points, bones,
    normals, triangle order and end caps remain exactly as authored.
    ``region`` has the same (u0, v0, u1, v1) convention as tube().
    """
    count = (len(path) - 1) * sides * 6 + sides * 6
    assert len(chunk['corners']) - start == count, 'Unexpected tube layout'
    assert len(path) == len(radii) and len(path) >= 2
    assert all(radius >= 0 for radius in radii)
    distances = [0.0]
    for a, b in zip(path, path[1:]):
        distances.append(distances[-1] + math.dist(a, b))
    length = distances[-1]
    assert length > 0

    low_u, high_u = sorted((region[0], region[2]))
    low_v, high_v = sorted((region[1], region[3]))
    # Explicit inset remains safe with bilinear filtering and atlas mipmaps.
    margin_u, margin_v = 4 / 2048, 4 / 1024
    low_u += margin_u
    high_u -= margin_u
    low_v += margin_v
    high_v -= margin_v
    assert high_u > low_u and high_v > low_v
    randoms = [value / 255 for value in hashlib.sha256(str(key).encode('utf-8')).digest()]
    # The modest density variation hides identical sampling without painting
    # a complete, differently stretched image onto each small branch.
    density = .245 * (.92 + .16 * randoms[0])
    density = min(density, (high_v - low_v) * .94 / length)
    max_radius = max(radii)
    if max_radius:
        density = min(density, (high_u - low_u) * .92 * 2 / (math.tau * max_radius))
    span_v = length * density
    max_span_u = math.tau * max_radius * density / 2
    center_u = low_u + max_span_u / 2 + randoms[1] * (high_u - low_u - max_span_u)
    bottom_v = low_v + randoms[2] * (high_v - low_v - span_v)
    reverse_u = randoms[3] >= .5
    reverse_v = region[3] < region[1]

    def at(ring, side):
        across = side / sides - .5
        if reverse_u:
            across = -across
        u = center_u + across * math.tau * radii[ring] * density / 2
        along = distances[ring] * density
        v = bottom_v + (span_v - along if reverse_v else along)
        return [u, v]

    offset = start
    for ring in range(len(path) - 1):
        for side in range(sides):
            # Match the original tube face order, including the UV seam.
            indices = ((ring, side), (ring, side + 1), (ring + 1, side + 1),
                       (ring, side), (ring + 1, side + 1), (ring + 1, side))
            for r, s in indices:
                chunk['corners'][offset][4:6] = at(r, s)
                offset += 1
    # Hidden base and minute terminal cap get a local disc sample rather than
    # the old arbitrary bottom-left texel. Geometry and winding stay untouched.
    for ring, reverse in ((0, True), (len(path) - 1, False)):
        center = [center_u, at(ring, 0)[1]]
        for side in range(sides):
            cap = [center]
            for index in (side, (side + 1) % sides):
                theta = index * math.tau / sides
                radius_u = radii[ring] * density / 2
                radius_v = radii[ring] * density
                cap.append([center_u + radius_u * math.cos(theta),
                            max(low_v, min(high_v, center[1] + radius_v * math.sin(theta)))])
            if reverse:
                cap.reverse()
            for uv in cap:
                chunk['corners'][offset][4:6] = uv
                offset += 1
    assert offset == len(chunk['corners'])
    assert all(low_u <= corner[4] <= high_u and low_v <= corner[5] <= high_v
               for corner in chunk['corners'][start:])
    return {'key': str(key), 'corners': count, 'centerline_length': round(length, 6),
            'density_v_per_world_unit': round(density, 6), 'atlas_bounds': [low_u, low_v, high_u, high_v],
            'root_u_span': round(math.tau * radii[0] * density / 2, 6),
            'tip_u_span': round(math.tau * radii[-1] * density / 2, 6),
            'uses_arc_length': True, 'taper_aware': True}


def horn_tube(chunk, tube, path, radii, bone, region, sides, key):
    start = len(chunk['corners'])
    tube(chunk, path, radii, bone, region, sides)
    report = remap_horn_uv(chunk, start, path, radii, region, sides, key)
    report.update(smooth_horn_normals(chunk, start, (len(path) - 1) * sides * 6))
    return report


def smooth_horn_normals(chunk, start, side_corner_count):
    """Area-weight the current horn's side faces, preserving separate flat caps.

    Native MOD stores a separate normal at each face corner.  Duplicate corner
    vertices, including both sides of the texture seam, are grouped by their
    exact authored position inside this one tube.  Caps and adjacent skin or
    other tubes never enter those groups. No vertex, UV or index is changed.
    """
    end = start + side_corner_count
    assert side_corner_count > 0 and side_corner_count % 3 == 0
    assert start >= 0 and end <= len(chunk['corners'])
    accumulated = {}
    for at in range(start, end, 3):
        corners = chunk['corners'][at:at + 3]
        points = [chunk['points'][corner[0]] for corner in corners]
        a, b = ([points[j][k] - points[0][k] for k in range(3)] for j in (1, 2))
        # The cross product is twice the face area times its unit normal.
        weighted = [a[1] * b[2] - a[2] * b[1],
                    a[2] * b[0] - a[0] * b[2],
                    a[0] * b[1] - a[1] * b[0]]
        for point in points:
            total = accumulated.setdefault(tuple(point), [0.0, 0.0, 0.0])
            for axis in range(3):
                total[axis] += weighted[axis]
    normals = {}
    for point, total in accumulated.items():
        length = math.sqrt(sum(value * value for value in total))
        assert length > 1e-14, 'Degenerate horn-side normal'
        normals[point] = [value / length for value in total]
    changed = 0
    for corner in chunk['corners'][start:end]:
        normal = normals[tuple(chunk['points'][corner[0]])]
        changed += corner[1:4] != normal
        corner[1:4] = normal
    return {'smooth_horn_normals': True, 'normal_weighting': 'adjacent_side_face_area',
            'smoothed_side_corners': side_corner_count, 'changed_normal_corners': changed,
            'smoothed_positions': len(normals), 'cap_normals_preserved': True,
            'normal_seam_welded': True}
