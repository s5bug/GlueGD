# GlueGD

GlueGD is a mod loader for Geometry Dash that does not require a modification
to any existing Geometry Dash files or an external injector or launcher.

## how it works

Windows applications search their own directory before searching System32 for
system libraries. Geometry Dash searches for `XINPUT9_1_0.dll`. We construct a
replacement DLL that injects our own code while manually opening the system's
XInput and forwarding Geometry Dash's XInput calls to the original DLL. In this
way, we can passively inject code without requiring the modification of any
existing game files.

GlueGD then does two things. The first is it retrieves all the necessary
symbols from the original `XINPUT9_1_0.dll` so that Geometry Dash thinks it
still has a valid XInput driver. The second is that GlueGD creates its own new
thread so that it can fully control the game.

## goals

- [x] Injecting an imGui Interface
- [ ] Reading/Writing Memory
- [ ] Reading/Writing Code
  - [ ] Supporting the Mega Hack v5+v6 JSONs
- [ ] Lua Scripting
