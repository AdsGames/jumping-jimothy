/**
 * Globals
 * Allan Legemaate and Danny Van Stemp
 * Global constants and state shared between scenes
 * 05/05/2017
 **/

#pragma once

#include <string>

// Logical screen size
inline constexpr int SCREEN_WIDTH = 1024;
inline constexpr int SCREEN_HEIGHT = 768;

// Box2D units are metres, the screen is drawn in pixels
inline constexpr float PIXELS_PER_METER = 20.0F;

// The game logic and physics run at a fixed 60 ticks per second
inline constexpr float TICK_SECONDS = 1.0F / 60.0F;

// Playable levels are 1 to LEVEL_COUNT, level 0 is a test level
inline constexpr int LEVEL_COUNT = 14;

// Path of a built in level
inline std::string levelPath(int level) {
  return "assets/data/level_" + std::to_string(level) + ".xml";
}

// State passed between scenes, not saved to disk
struct Session {
  // Level the game scene starts on
  int level_to_start{1};

  // True when the game scene tests a level from the editor
  bool editing_level{false};

  // File the editor is working on
  std::string editing_file;
};

inline Session session;
