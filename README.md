# Jumping Jimothy

[![Maintainability](https://api.codeclimate.com/v1/badges/c0a7a04523e632717de3/maintainability)](https://codeclimate.com/github/AdsGames/JumpingJimothy/maintainability)

Jumping Jimothy is a gravity modifying platformer made in C++ using [ASW](https://github.com/adsgames/asw) (SDL3) and Box2D.

## Setup

Dependencies (ASW, SDL3, Box2D and pugixml) are fetched by CMake with [CPM](https://github.com/cpm-cmake/CPM.cmake), so only CMake and a C++20 compiler are needed.

### CMake

```bash
cmake --preset debug
cmake --build --preset debug
./build/debug/target/JumpingJimothy
```

### Build Emscripten

```bash
emcmake cmake --preset release
cmake --build --preset release
```

The level editor is not available in the browser build.

## Controls

| Action           | Keyboard        | Controller |
| ---------------- | --------------- | ---------- |
| Move             | A / D, arrows   | Left stick, D-pad |
| Jump             | W               | A          |
| Freeze time      | Space           | B          |
| Restart level    | R               |            |
| Menu             | Escape          |            |

## Level Editor

Levels are XML files in `assets/data`. The editor saves new levels to the user save folder by default.
