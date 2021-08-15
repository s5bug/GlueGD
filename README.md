# GlueGD

GlueGD is a mod loader for Geometry Dash that does not require a modification
to any existing Geometry Dash files or an external injector or launcher.

## how it works

Most game engines that are built on DirectInput or XInput have a flaw in them
where they will look in the process directory for the needed DLLs before
correctly looking in System32. Geometry Dash is no different: the game engine
it is built on looks for an `XINPUT9_1_0.dll`, which is normally found properly
in System32. However, GlueGD creates its own `XINPUT9_1_0.dll` that can live in
the same folder as Geometry Dash. This causes Geometry Dash to find GlueGD's
DLL first.

GlueGD then does two things. The first is it retrieves all the necessary
symbols from the original `XINPUT9_1_0.dll` so that Geometry Dash thinks it
still has a valid XInput driver. The second is GlueGD creates its own new
thread that can fully control the game itself.

## goals

- [ ] Injecting an imGui Interface
- [ ] Reading/Writing Memory
- [ ] Reading/Writing Code
  - [ ] Supporting the Mega Hack v5+v6 JSONs
- [ ] Lua Scripting
