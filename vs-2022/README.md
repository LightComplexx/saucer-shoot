# README

This directory contains Microsoft Visual Studio v17 (2022) solution files.

The Solution assumes a directory setup as follows:

-------------------   --------------------------------------------------------
Visual Studio 2022/   Location of Visual Studio projects.
-gameX/               Source code for game.
--vs-2022/            Visual Studio solution files.
---x64/               Location of x64 architecture builds.
----Debug/            Location of executable (.exe) that runs in Debug mode.
----Release/          Location of executable (.exe) that runs in Release mode.
--df-font.txt         Fonts used by Dragonfly.
--df-config.txt       Config file used by Dragonfly (optional).
--sounds/             Sounds used for game.
--sprites/            Sprites used for game.
-SFML-3.0/            SFML directory (version 3.0+).
-dragonfly/           Location of Dragonfly include/ and lib/ sub-directories.
-------------------   --------------------------------------------------------

If the `SFML-3.0/` and/or `dragonfly/` directory locations are changed, the solution Properties will need to be changed to match.

If the `sounds/` and `sprites/` directory locations are changed, the game code in `game.cpp` will need to be changed to match.

Happy Saucer Shooting!

-- Mark
