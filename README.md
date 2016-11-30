# bgbtech_engine2
Newer in-development engine + misc.

This engine is basically being rewritten ground-up in order to be smaller and more streamlined.

Like the old engine, it is using a voxel system.
Will also use a reimplemented version of the BGBScript VM.

This also contains an MSP430 emulator and basic electronics simulator.

Primary development environment:
VS2015 Shell + MinGW tools (make).


deeptail0.c:
Current main interface for 3D voxel engine.

isotest0.c:
Small experiment for doing an isometric game.

plattest0.c:
Platformer experiment.

btactst0.c:
Originally a test for BTAC, now it is the asset bundling tool.

bsvm2:
BGBScript2 VM.

resource:
Directory for bundled/deployed assets.
Bundled assets use the BtPAK format.

resource_dev:
Directory for in-development assets.

base:
General core code (GL/VFS/...);

voxel:
Stuff related to the voxel terrain system.

entity:
Stuff related to entities (mostly glue for the BS2 script code).
