# Mage effect lifetime and ground orientation

The native engine stops an ordinary magic script when its target object is
removed. Both private mage B scripts now emit the existing `[ATTACKMAGIC]`
directive, which continues their remaining effects at the last target position.
Damage and the server death deadline are unchanged. Meteor's authored terminal
layer ends at 7,396 ms; the saved frost ground layers end at 5,145 ms.

The longer visual tail does not add a seven-second recast lock. Server mage
actionDelay is 1,500 ms and the requested cooldown is 4,000 ms. The shipped
Character.Update block at 0x40c86b clears m_bEndMagic on a normal frame after
StartMagic resets its timer; EndMagic's END_PLAYER_ATTACK packet branch excludes
casting magic. These existing gates were inspected without changing them.

`CMagicEffect::UpdateEnemyPos` previously included the target height offset in
its facing vector. This pitched broad ground meshes toward the caster: on flat
ground, a 7.2-unit meteor decal 0.07 above ground and five units from the caster
had a lowest point below ground. Adding more height also increased that pitch.

Stationary private layers now carry `[SPEED] 140031`. The `[ENEMY]` branch does
not use SPEED for movement; it is a narrow marker for the level-facing hook.
The hook removes only the vertical component of the facing vector. It preserves
the layer's world position, authored height, yaw, WED rotation, and mesh shape.
Meteor flight PARTs retain their actual 600/540 speed values. MOVETYPE and sound
style fields are unchanged. Native `GetDir` safely supplies its existing forward
direction when flattening a purely vertical direction produces a zero vector.

The marker is applied to frost's stationary layers and meteor's stationary
portals, impact tails, and afterglow. Intentional portal tilt remains in its
authored WEM/WED; crystal height and the vertical flame meshes remain intact.
The planes remain flat at the target's elevation, as before; they are not new
terrain-conforming meshes on slopes. All saved colours and tuning values remain
unchanged, and recompilation changes only the two B scripts.

`tools/patch_mage_ground.py` uses exact-byte guards and a reversible `.mgnd`
section in Engine.dll, preserving the pre-existing `.korean` section. Validation
runs the actual shipped `UpdateEnemyPos`, Vector3, Matrix4, and Quaternion machine
code at two DLL bases. It checks level orientation, yaw, zero horizontal distance,
world positions, stack balance, preserved registers, and 48 unchanged ordinary
effect cases. The new code has no absolute addresses or moved relocations.

`tools/verify_mage_lifetime.py` runs the native `CMagic::Update`,
`CMagicEffect::Update`, and `NextStep` paths with the target already removed.
The old script setting ends at the first 33 ms tick; the corrected setting lets
each representative terminal layer finish at its authored deadline (rounded to
the next tick). These tests exercise lifecycle and math, not the live renderer.
