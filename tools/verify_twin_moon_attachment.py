"""Audit the native bow on its male/female skill and locomotion frames.

This reconstructs the shipped ANI hand transforms. It is not an in-game
visual test, a terrain test, or a test of mounted/other weapon motions.
Models and animations are read only; --output optionally saves the audit.
"""
import argparse
import hashlib
import json
from pathlib import Path
import struct
import sys

ROOT = Path(__file__).resolve().parents[1]
sys.path[:0] = [str(ROOT / "runtime/pylibs"),
               str(ROOT / "assets/twilight"),
               str(ROOT / "client-overlay/Tools/SkillColors")]

import numpy as np
from native_actor import animation
from verify_native_animation import read_mod


BONE = "Bip01 L Finger0"


def sha256(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def attach(points, pose):
    # ANI quaternion conjugation matches the existing native actor renderer.
    # ANI bone tracks already contain the absolute bone transform.
    position = np.asarray(pose[:3], dtype=np.float64)
    vector = -np.asarray(pose[3:6], dtype=np.float64)
    scalar = float(pose[6])
    return points + 2 * np.cross(vector, np.cross(vector, points) + scalar * points) + position


def check_clip(path, points, clearance, frames=None):
    raw = path.read_bytes()
    frame_count = struct.unpack_from("<i", raw, 32)[0]
    if frames is None:
        if frame_count != 136:
            raise ValueError(f"Expected all 136 arrow frames, found {frame_count}: {path}")
        frames = list(range(frame_count))
    clip = animation(raw, frames)
    matches = [bone for bone in clip["bones"] if bone["name"] == BONE]
    if len(matches) != 1:
        raise ValueError(f"Expected exactly one native attachment bone: {path}")
    poses = np.asarray(matches[0]["poses"], dtype=np.float64)
    if not np.isfinite(poses).all():
        raise ValueError(f"Non-finite hand pose: {path}")
    quaternion_error = float(np.abs(np.linalg.norm(poses[:, 3:], axis=1) - 1).max())
    if quaternion_error > 1e-4:
        raise ValueError(f"Non-unit native quaternion ({quaternion_error}): {path}")
    low, high = np.full(3, np.inf), np.full(3, -np.inf)
    minima, worst = [], None
    pivot_error = 0.0
    for frame, pose in zip(frames, poses):
        world = attach(points, pose)
        low, high = np.minimum(low, world.min(axis=0)), np.maximum(high, world.max(axis=0))
        vertex = int(world[:, 1].argmin())
        minimum = float(world[vertex, 1])
        minima.append(minimum)
        if worst is None or minimum < worst["world_point"][1]:
            worst = dict(frame=frame, vertex=vertex, local_point=points[vertex].tolist(),
                         world_point=world[vertex].tolist())
        pivot_error = max(pivot_error, float(np.abs(attach(np.zeros((1, 3)), pose)[0] - pose[:3]).max()))
    below = [frame for frame, minimum in zip(frames, minima) if minimum < clearance - 1e-6]
    return dict(path=str(path.relative_to(ROOT)) if path.is_relative_to(ROOT) else str(path),
                sha256=hashlib.sha256(raw).hexdigest(), frames=len(frames),
                source_frame_range=[frames[0], frames[-1]],
                bone=BONE, world_bounds=[low.tolist(), high.tolist()],
                minimum_y=min(minima), maximum_frame_minimum_y=max(minima),
                below_clearance_frames=below, worst=worst,
                quaternion_norm_max_error=quaternion_error,
                native_origin_to_hand_max_error=pivot_error,
                passed=not below and pivot_error <= 1e-8)


def check_locomotion(client, points, clearance):
    # CharacterAction::GetWeaponId maps LONGBOW to SHORTBOW (15), not 16.
    # ActionMgr::LoadMode reads war, peace, then ten mount modes. Each mode
    # holds 50 actions * 3 levels, and LoadAct reads a 16-byte SMot record.
    table = client / "Body/character.wad"
    raw = table.read_bytes()
    if struct.unpack_from("<I", raw)[0] != 100:
        raise ValueError("Unsupported character.wad version")
    at, names = 8, {}
    for _ in range(struct.unpack_from("<I", raw, 4)[0]):
        key, length = struct.unpack_from("<2I", raw, at)
        at += 8
        names[key] = raw[at:at + length].split(b"\0")[0].decode("cp949")
        at += length
    clips, unused = [], []
    for sex in ("male", "female"):
        weapon_count = struct.unpack_from("<I", raw, at)[0]
        at += 4
        if weapon_count <= 15 or at + weapon_count * 28800 > len(raw):
            raise ValueError(f"Invalid weapon table for {sex}")
        for mode_index, mode in enumerate(("war", "peace")):
            for state, action in enumerate(("idle", "walk", "run")):
                for level in range(3):
                    offset = at + 15 * 28800 + mode_index * 2400 + (state * 3 + level) * 16
                    begin, end, ani, weapon_ani, _ = struct.unpack_from("<2I3H", raw, offset)
                    selection = dict(sex=sex, mode=mode, action=action, level=level,
                                     record_offset=offset, animation_id=ani,
                                     weapon_animation_id=weapon_ani)
                    if ani == 0 or end <= begin:
                        unused.append(selection)
                        continue
                    name = names[ani]
                    if not name.endswith("ani"):
                        name += ".ani"
                    clip = check_clip(client / "Body/Animation" / name, points,
                                      clearance, list(range(begin, end + 1)))
                    clips.append(dict(**selection, **clip))
        at += weapon_count * 28800
    for sex in ("male", "female"):
        for mode in ("war", "peace"):
            for action in ("idle", "walk", "run"):
                if not any(c["sex"] == sex and c["mode"] == mode and c["action"] == action for c in clips):
                    raise ValueError(f"Missing native {sex}/{mode}/{action} animation")
    return dict(passed=all(c["passed"] for c in clips), table=str(table),
                table_sha256=hashlib.sha256(raw).hexdigest(), weapon_index=15,
                weapon_mapping="dITEMTYPE_LONGBOW -> WEAPON_SHORTBOW (15)",
                source_evidence=["src/client/gameclient/Src/CharacterAction.cpp:1136",
                                 "src/client/gameclient/Src/ActionMgr.cpp:293",
                                 "src/client/gameclient/Src/Character.cpp:3285"],
                total_pose_checks=sum(c["frames"] for c in clips),
                minimum_y=min(c["minimum_y"] for c in clips),
                unused_motion_slots=unused, clips=clips)


def check_string(parts, report):
    atlas = report["model"]["atlas"]
    rect = atlas["material_rects"]["2"]
    side = atlas["atlas_size"]
    lower = np.array([rect["x"], rect["y"]]) / side
    upper = lower + np.array([rect["width"], rect["height"]]) / side
    points, triangles = [], 0
    for part in parts:
        uv = part["corners"]["uv"].reshape(-1, 3, 2)
        selected = np.all((uv >= lower - 1e-7) & (uv <= upper + 1e-7), axis=(1, 2))
        indices = part["corners"]["index"].reshape(-1, 3)[selected]
        triangles += len(indices)
        if len(indices):
            points.append(part["points"][indices.ravel()])
    if not points:
        raise ValueError("No bowstring triangles found in the dedicated string atlas material")
    points = np.concatenate(points).astype(np.float64)
    low, high = points.min(axis=0), points.max(axis=0)
    span = high - low
    expected = atlas["triangles_by_material"]["2"]
    # The long axis is native X; a thin, straight string has bounded Y/Z span.
    passed = (triangles == expected and low[0] < 0 < high[0]
              and span[0] > 1 and max(span[1:]) <= .003
              and abs((low[2] + high[2]) / 2) <= 1e-5)
    return dict(triangles=triangles, bounds=[low.tolist(), high.tolist()],
                span=span.tolist(), same_rigid_hand_transform_as_bow=True,
                passed=bool(passed))


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--model", type=Path, default=ROOT / "client-overlay/Equip/mt_longbow_1.mod")
    parser.add_argument("--client", type=Path, default=ROOT / "runtime/client/GameClient")
    parser.add_argument("--build-report", type=Path,
                        default=ROOT / "assets/twilight-set/twin-moon-greatbow/build-validation.json")
    parser.add_argument("--minimum-clearance", type=float, default=0.0)
    parser.add_argument("--output", type=Path, help="Optionally save this audit as JSON")
    args = parser.parse_args()
    parts = read_mod(args.model)
    if len(parts) != 1 or not np.allclose(parts[0]["matrix"], np.eye(4), atol=1e-7, rtol=0):
        raise ValueError("The replacement must retain one identity-transform native mesh")
    points = np.concatenate([part["points"] for part in parts]).astype(np.float64)
    triangles = sum(len(part["corners"]) // 3 for part in parts)
    digest = sha256(args.model)
    report = json.loads(args.build_report.read_text(encoding="utf-8"))
    if report["surface"]["sha256"] != digest:
        raise ValueError("Build report and model differ; rerun after the build finishes")
    string = check_string(parts, report)
    clips = {sex: check_clip(args.client / f"Skill/Ani/arrow_{letter}.ani", points, args.minimum_clearance)
             for sex, letter in (("male", "m"), ("female", "f"))}
    locomotion = check_locomotion(args.client, points, args.minimum_clearance)
    result = dict(passed=triangles <= 8500 and string["passed"] and locomotion["passed"]
                        and all(c["passed"] for c in clips.values()),
                  model=str(args.model), sha256=digest, triangles=triangles,
                  rendered_corners=triangles * 3, triangle_budget=8500,
                  native_bounds=[points.min(axis=0).tolist(), points.max(axis=0).tolist()],
                  native_grip_origin=[0, 0, 0], native_matrix_identity=True,
                  coordinate_system="Native ANI pose space; Y is up, ground plane is Y=0",
                  minimum_clearance=args.minimum_clearance, bowstring=string, animations=clips,
                  locomotion=locomotion, in_game_visual_test=False,
                  idle_and_movement_tested=True, mounted_motion_tested=False,
                  subframe_interpolation_tested=False)
    rendered = json.dumps(result, indent=2) + "\n"
    if args.output:
        args.output.write_text(rendered, encoding="utf-8")
    print(rendered, end="")
    return 0 if result["passed"] else 1


if __name__ == "__main__":
    raise SystemExit(main())
