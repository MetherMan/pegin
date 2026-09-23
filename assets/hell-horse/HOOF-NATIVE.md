# Animated hoof hook evidence

`patch_hell_horse_hoof.py` stages the hook in memory by default. Root integration
uses `--apply` after `verify_hell_horse_hoof.py` passes. The installation report
is `hoof-hook.json`; `hoof-hook-staged.json` records the prior dry run.

The six-byte branch at `DeicideOnline.exe:0x4223a3` previously entered the
center trail. The new `.hhoof` code replaces that branch only. The original
`.htrail`, `.htdata`, `.hfire` and unrelated sections remain intact; undo
restores the previous entry and PE headers exactly. The executable code is
RX, and 64 independent, bounded-probe state slots are in RW `.hfdata`.

## Native interfaces checked

- `IW3DModel` vtable `+0x3c`: `GetBoneMatrix`. The shipped implementation is
  `Render.dll:0x10013620`. ECX holds the model and the three stack arguments
  are a hidden 64-byte result buffer, bone name and animation milliseconds.
  It returns with `ret 12` and EAX points to that result buffer.
- The native function selects `start + (tick * 35 / 1000) % frameCount`,
  searches the actual bone name, and builds its matrix from that frame's
  position/quaternion. It does not apply inverse bind a second time.
- `CVehicleAction+0x18` is the current animation clock. The original vehicle
  epilogue copies exactly this value to `CVehicle+0xbc`, which Draw uses.
  The new call uses that current clock, avoiding the previous frame's value.
- EXE import `0x46a05c`: `Matrix4::SetWorld(position, quaternion)` takes two
  pointer arguments and returns with `ret 8`.
- World vtable `+0xa4`: `GetHeight(position)` takes a vector pointer, returns
  height in x87 ST0 and cleans four stack bytes. World `+0x90` is the
  existing `AddParticle(name, position)` path and cleans eight bytes.

The four bone-local sole points are measured from the real horse mesh.
See `hoof-motion.json` for the input files, inverse-bind calculation,
bone names, sole dimensions and the normal/bow/walk contact frames.

## Landing rule and bounded work

Each foot becomes armed only above model-space height `.22`. On a later
descending sample at or below `.16`, it emits once and disarms. The mark
uses the actual animated sole XY transformed by the vehicle rotation and
position. Its Z is terrain height at that XY plus `.20`. There is no
left/right timer, fixed body-center offset or emission every render frame.

New vehicles, changed action pointers or start/end/animation contents,
animation-clock restarts, reused objects, five seconds without updates,
stopped XY movement and steps of at least eight world units clear all four
armed states. The fixed action object can change contents without changing
its address, so its pointer alone is not treated as the clip identity.
Initially low feet therefore do not emit a four-flame burst. Stationary
updates do not query bone matrices or terrain. Moving updates query four
bone matrices, and terrain only on a landing; a horse emits at most four
particles in one update and one per foot per completed lift/landing cycle.

## Verification limits

The Unicorn check maps the real Render and Common DLLs and executes their
actual matrix/bone functions, using raw ANI bytes for the animation data.
Only animation-container access, the wall clock and world terrain/particle
services are controlled substitutes. Both the model point and final world
point are checked against independent scalar transforms of the raw poses.
Native run, bow-run and walk at 10/15/30/60/144 display fps and two executable
load bases produce the expected contact events. Full vehicle epilogue,
stack cleanup, callee-saved registers, x87 balance, other-mount dust,
idle suppression, absent model/action, in-place action changes, stale
identities and teleport/object reuse resets are checked. At 10/15 fps a
short contact can fall between sampled frames and be skipped. The hook
deliberately does not emit a delayed burst of past contacts.

This is native control-flow/ABI validation, not a game capture. Terrain
appearance and the final small flame still require running-game inspection.
