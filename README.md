# Helicopter Game

Helicopter Game is a small SFML-based arcade prototype written in C++. You pilot a helicopter through a scrolling obstacle course, fighting gravity and surviving as long as possible.

## Current Features

- Helicopter movement with upward thrust and falling physics
- Continuous obstacle movement and respawning
- Collision handling against walls, ceiling, and floor
- Restart flow after a crash
- Bundled SFML headers and libraries for local Visual Studio builds

## Controls

- `W` or `Up Arrow`: thrust upward and start the run
- `Enter`: restart after crashing
- Close window: quit the game

## Build Requirements

- Windows
- Visual Studio 2022 with the MSVC v143 toolset
- Desktop C++ workload installed

The repository already includes the SFML dependency under `SFML/`, so no additional package install is required.

## Build And Run

1. Open `Helicopter Game.sln` in Visual Studio 2022.
2. Select `Debug | x64` or `Release | x64`.
3. Build the solution.
4. Run the `Helicopter Game` startup project.

The executable is produced at:

- `x64/Debug/Helicopter Game.exe`
- `x64/Release/Helicopter Game.exe`

## Verified Local Build

This project was validated with:

- Solution: `Helicopter Game.sln`
- Configuration: `Debug | x64`
- Toolchain: Visual Studio 2022 MSBuild

## Project Structure

- `Source.cpp`: main game loop and helicopter movement
- `Wall.h` / `Wall.cpp`: obstacle creation, movement, and respawn logic
- `Collision.h` / `Collision.cpp`: sprite collision helpers
- `Helicopter.png`: helicopter texture
- `SFML/`: bundled SFML headers and libraries

## Notes

- The project currently uses simple rectangle-vs-sprite bounds checks for wall collisions during gameplay.
- The bundled SFML static debug libraries may emit PDB link warnings in Debug builds; those do not block the build or execution.
