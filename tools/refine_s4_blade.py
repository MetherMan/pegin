"""Enlarge only the approved S4 blade, preserving the existing hilt and UVs.

The standalone command audits in memory unless --output is supplied. Its
input hash guard prevents accidentally applying the enlargement twice.
refine_preview_parts supplies the same deformation to future source builds.
"""
import argparse
import hashlib
import json
from pathlib import Path
import struct
import sys

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "runtime/pylibs"))
import numpy as np

SPEC = ROOT / "assets/twilight-set/noble-moon-s4-sword-geometry.json"
CORNER = np.dtype([("index", "<i4"), ("normal", "<f4", 3), ("uv", "<f4", 2)])


def parameters(spec):
    config = spec["blade_refinement"]
    root = (spec["grip"][1] - spec["blade_root_y"]) * spec["scale"]
    blend = config["blend_pixels"] * spec["scale"]
    factors = np.array([config[k] for k in ("length_factor", "width_factor", "thickness_factor")])
    if root <= 0 or blend <= 0 or not np.all((factors >= 1) & (factors <= 1.25)):
        raise ValueError("Invalid S4 blade refinement dimensions")
    return root, blend, factors - 1


def deformation(points, spec):
    points = np.asarray(points, dtype=np.float64)
    root, blend, changes = parameters(spec)
    distance = np.maximum(points[:, 0] - root, 0)
    t = np.clip(distance / blend, 0, 1)
    weight = t * t * (3 - 2 * t)
    derivative = np.where((t > 0) & (t < 1), 6 * t * (1 - t) / blend, 0)
    result = points.copy()
    result[:, 0] += changes[0] * distance * weight
    result[:, 1] *= 1 + changes[1] * weight
    result[:, 2] *= 1 + changes[2] * weight
    # Lower-triangular deformation Jacobian, for the inverse-transpose normal.
    a = 1 + changes[0] * (weight + distance * derivative)
    b = changes[1] * points[:, 1] * derivative
    c = 1 + changes[1] * weight
    d = changes[2] * points[:, 2] * derivative
    e = 1 + changes[2] * weight
    return result, (a, b, c, d, e)


def deform_normals(points, normals, spec):
    _, (a, b, c, d, e) = deformation(points, spec)
    result = np.asarray(normals, dtype=np.float64).copy()
    result[:, 1] /= c
    result[:, 2] /= e
    result[:, 0] = (result[:, 0] - b * result[:, 1] - d * result[:, 2]) / a
    lengths = np.linalg.norm(result, axis=1)
    if np.any(lengths <= 1e-12):
        raise ValueError("Invalid zero-length normal")
    return result / lengths[:, None]


def refine_preview_parts(parts, spec):
    """Return new preview parts; below-root arrays and every UV stay intact."""
    root, _, _ = parameters(spec)
    refined = []
    for name, material, (points, normals, uv) in parts:
        # Export conversion is preview (+Y) -> native (+X, -Y).
        native = np.column_stack((points[:, 1], -points[:, 0], points[:, 2]))
        native_normals = np.column_stack((normals[:, 1], -normals[:, 0], normals[:, 2]))
        mask = native[:, 0] > root
        new_points, new_normals = points.copy(), normals.copy()
        if mask.any():
            moved, _ = deformation(native[mask], spec)
            moved_normals = deform_normals(native[mask], native_normals[mask], spec)
            new_points[mask] = np.column_stack((-moved[:, 1], moved[:, 0], moved[:, 2]))
            new_normals[mask] = np.column_stack((-moved_normals[:, 1], moved_normals[:, 0], moved_normals[:, 2]))
        refined.append((name, material, (new_points, new_normals, uv)))
    return refined


def read_layout(raw):
    if struct.unpack_from("<i", raw, 32)[0] != 1:
        raise ValueError("The S4 sword must have one native mesh")
    matrix = np.frombuffer(raw, "<f4", 16, 36).reshape(4, 4)
    if not np.array_equal(matrix, np.eye(4, dtype="<f4")):
        raise ValueError("The native grip transform must remain identity")
    count = struct.unpack_from("<i", raw, 100)[0]
    point_offset = 104
    points = np.frombuffer(raw, "<f4", count * 3, point_offset).reshape(-1, 3)
    faces = struct.unpack_from("<i", raw, point_offset + count * 12)[0]
    corner_offset = point_offset + count * 12 + 4
    corners = np.frombuffer(raw, CORNER, faces * 3, corner_offset)
    if corner_offset + faces * 72 + 52 != len(raw) or struct.unpack_from("<i", raw, len(raw) - 4)[0] != 0:
        raise ValueError("Unexpected native model tail or animation tracks")
    if not (np.isfinite(points).all() and np.isfinite(corners["normal"]).all()
            and np.isfinite(corners["uv"]).all() and (corners["index"] >= 0).all()
            and (corners["index"] < count).all()):
        raise ValueError("Invalid native geometry")
    return points, corners, point_offset, corner_offset


def geometry_fingerprints(points, corners, root):
    protected = points[:, 0] <= root
    protected_corners = protected[corners["index"]]
    return dict(protected_geometry_sha256=hashlib.sha256(
                    points[protected].tobytes() + corners[protected_corners].tobytes()).hexdigest(),
                topology_uv_sha256=hashlib.sha256(
                    corners["index"].tobytes() + corners["uv"].tobytes()).hexdigest())


def verify_refined_native(raw, spec, reference):
    """Verify a future source rebuild against the approved immutable hilt."""
    points, corners, _, _ = read_layout(raw)
    root, _, _ = parameters(spec)
    fingerprints = geometry_fingerprints(points, corners, root)
    for key, value in fingerprints.items():
        if value != reference[key]:
            raise ValueError(f"S4 rebuild changed protected geometry or UV topology: {key}")
    if len(points) != reference["vertices"] or len(corners) // 3 != reference["triangles"]:
        raise ValueError("S4 rebuild changed the native mesh topology")
    if not np.allclose([points.min(0), points.max(0)], reference["new_bounds"], atol=3e-6, rtol=0):
        raise ValueError("S4 rebuild did not apply the approved blade-only dimensions")
    return dict(passed=True, source_model_sha256=reference["source_sha256"],
                blade_root_native_x=root, parameters=spec["blade_refinement"],
                guard_handle_pommel_identical=True, topology_identical=True,
                source_uvs_identical=True, **fingerprints)


def refine_native_bytes(raw, spec):
    digest = hashlib.sha256(raw).hexdigest()
    if digest != spec["blade_refinement"]["source_model_sha256"]:
        raise ValueError("Input is not the approved original S4 MOD; refusing repeated or unrelated refinement")
    points, corners, point_offset, corner_offset = read_layout(raw)
    root, blend, changes = parameters(spec)
    blade = points[:, 0] > root
    blade_corners = blade[corners["index"]]
    output = bytearray(raw)
    new_points, new_corners, _, _ = read_layout(output)
    moved, jacobian = deformation(points[blade], spec)
    new_points[blade] = moved
    new_corners["normal"][blade_corners] = deform_normals(
        points[corners["index"][blade_corners]], corners["normal"][blade_corners], spec)

    allowed = np.zeros(len(raw), dtype=bool)
    for index in np.flatnonzero(blade):
        start = point_offset + int(index) * 12
        allowed[start:start + 12] = True
    for index in np.flatnonzero(blade_corners):
        start = corner_offset + int(index) * CORNER.itemsize + 4
        allowed[start:start + 12] = True
    changed = np.frombuffer(raw, "u1") != np.frombuffer(output, "u1")
    if np.any(changed & ~allowed):
        raise ValueError("A protected byte outside the blade positions/normals changed")
    if not np.array_equal(corners[["index", "uv"]], new_corners[["index", "uv"]]):
        raise ValueError("Native topology or source UVs changed")
    if (points[~blade].tobytes() != new_points[~blade].tobytes()
            or corners[~blade_corners].tobytes() != new_corners[~blade_corners].tobytes()):
        raise ValueError("Guard, handle, or pommel bytes changed")

    indices = corners["index"].reshape(-1, 3)
    old_triangles, new_triangles = points[indices].astype(float), new_points[indices].astype(float)
    old_cross = np.cross(old_triangles[:, 1] - old_triangles[:, 0], old_triangles[:, 2] - old_triangles[:, 0])
    new_cross = np.cross(new_triangles[:, 1] - new_triangles[:, 0], new_triangles[:, 2] - new_triangles[:, 0])
    winding_dot = np.einsum("ij,ij->i", old_cross, new_cross)
    nonzero = np.linalg.norm(old_cross, axis=1) > 1e-12
    if not np.all(winding_dot[nonzero] > 0):
        raise ValueError("A previously valid face collapsed or reversed orientation")
    report = dict(passed=True, source_sha256=digest,
                  refined_sha256=hashlib.sha256(output).hexdigest(), bytes=len(raw),
                  vertices=len(points), triangles=len(corners) // 3,
                  changed_vertices=int(blade.sum()), protected_vertices=int((~blade).sum()),
                  changed_bytes=int(changed.sum()), protected_bytes_identical=True,
                  topology_identical=True, source_uvs_identical=True,
                  guard_handle_pommel_identical=True, native_grip_origin_preserved=True,
                  blade_root_native_x=root, blend_native_length=blend,
                  **geometry_fingerprints(points, corners, root),
                  requested_factors=(changes + 1).tolist(),
                  minimum_jacobian_determinant=float(np.min(jacobian[0] * jacobian[2] * jacobian[4])),
                  old_bounds=[points.min(0).tolist(), points.max(0).tolist()],
                  new_bounds=[new_points.min(0).tolist(), new_points.max(0).tolist()],
                  old_blade_bounds=[points[blade].min(0).tolist(), points[blade].max(0).tolist()],
                  new_blade_bounds=[new_points[blade].min(0).tolist(), new_points[blade].max(0).tolist()],
                  changed_faces_collapsed_or_reversed=0,
                  blade_length_ratio=float((new_points[:, 0].max() - root) / (points[:, 0].max() - root)),
                  trace_recommendation=dict(start_unchanged=.1458286, old_end=1.6041147,
                                            new_end=float(deformation(np.array([[1.6041147, 0, 0]]), spec)[0][0, 0])),
                  in_game_visual_test=False)
    return bytes(output), report


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--source", type=Path, default=ROOT / "client-overlay/Equip/mt_twilight_1.mod")
    parser.add_argument("--spec", type=Path, default=SPEC)
    parser.add_argument("--output", type=Path, help="Write the refined MOD; omit for an in-memory audit")
    parser.add_argument("--report", type=Path, help="Save the audit JSON")
    args = parser.parse_args()
    spec = json.loads(args.spec.read_text(encoding="utf-8"))["weapons"][0]
    refined, report = refine_native_bytes(args.source.read_bytes(), spec)
    if args.output:
        args.output.write_bytes(refined)
    report["model_file_written"] = str(args.output) if args.output else None
    if args.report:
        args.report.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(json.dumps(report, indent=2))


if __name__ == "__main__":
    main()
