# Private mage layers: avoid unused sound objects

This optimization addresses spell creation and active spell overhead. It does
not establish the cause of stutter while standing or moving without effects.
The game and VM were not running during the read-only process inspection, so
live FPS and frame-time comparisons were unavailable.

The native `CMagicEffect::Create` constructs a SoundObject3D for every EFFECT,
including layers with no sound commands. Every active layer subsequently calls
that object's Update3D. A private layer already carrying the stationary marker
`[ENEMY]` + `[SPEED] 140031` can omit the sound object if **every** PART lacks
both `bSound` and `bBlowTiming`. Any later sound or hit-timing PART preserves the
original allocation. Unmarked legacy effects preserve the original behavior.

For the saved resources this reduces sound-object creation per cast from 105 to
19 for meteor, and 215 to 5 for frost. It also removes the corresponding no-op
Update3D calls while those visual layers are active. No WED/WEM/texture, spell
duration, damage event, density, color, cooldown, or existing sound is changed.

`tools/patch_silent_magic.py` stages Engine-silent-magic.dll by default under
`runtime/performance`; it does not replace client-overlay/Engine.dll by default.
The `.msilent` entry hook is applied after the validated `.mgnd` ground hook and
has its own undo report. Rebuilds must preserve that order; remove/rebuild the
outer `.msilent` layer before revising `.mgnd`. A mismatched input hash fails
closed instead of guessing the previous binary state.

Validation executes the shipped Create and destructor at two DLL load bases for
22 cases, including empty/null PART lists, unmarked layers, markers on moving
parts, later SOUND/BLOWTIMING parts, and purely visual marked layers. Only the
component factory, allocation/free services and sound Release are substituted;
list copying, the new branch, cleanup, stack balance and preserved registers run
as actual native code. Existing ground-orientation and target-death lifetime
paths are also executed against the staged combined binary.

`tools/verify_silent_magic_updates.py` separately executes 480 paired update
frames through the actual Engine/Common/Sound DLL code, at two Engine and Sound
load addresses. Each active silent layer omits one virtual Update3D call and
exactly **13 x86 instructions**: four in Engine and nine in Sound's normal-style
early return. Every effect state byte other than its sound pointer matches after
every frame. Layers waiting for their STARTTIME already skip this call, so their
per-frame savings are zero. The detailed machine counts are recorded in
`silent-magic-update-validation.json`.

The conservative per-cast upper bound is 86 calls / 1,118 instructions per frame
for meteor, and 210 calls / 2,730 instructions for frost. Actual counts are lower
when layers are delayed or expired; overlapping casts add their active layers.
These are executed instruction counts, not CPU cycles, wall time or measured
FPS. The fixture has no renderer or audio device. Sound's default-style update
returns before accessing its manager or device, and construction itself does
not allocate an audio sample handle. This optimization removes object creation
and a small amount of active-spell CPU work, and saves **no frame work when no
spell is active**. It cannot explain or establish a fix for idle/movement stutter.

Main-loop inspection separately found no explicit frame limiter: WinMain uses
GetTickCount and continuously updates/renders when its message queue is empty.
Fullscreen presentation requests IMMEDIATE. Windowed presentation and driver
behavior can change actual pacing, so no FPS is inferred from this code alone.
No limiter or hardware vertex-processing flag is forced: native Tracking moves
by a fixed distance per update, so changing update frequency could alter effects.
