/**
 * Game
 * Allan Legemaate and Danny Van Stemp
 * Game state
 * 05/05/2017
 **/

#pragma once

#include <box2d/box2d.h>
#include <memory>
#include <string>
#include <vector>

#include "../State.h"
#include "../ui/Button.h"
#include "../Globals.h"
#include "../ui/UIHandler.h"
#include "Box.h"
#include "Character.h"
#include "GameAssets.h"
#include "Goat.h"

class Game : public asw::scene::Scene<ProgramState> {
 public:
  using asw::scene::Scene<ProgramState>::Scene;

  void init() override;
  void update(float dt) override;
  void draw() override;
  void cleanup() override;

 private:
  // Reload the current level
  void reset();

  // Build the world from a level file
  void loadLevel(const std::string& path);

  // Reached the goat
  void completeLevel();

  // Go to the next level, or the menu after the last one
  void nextLevel();

  // Restart the level
  void die();

  // Toggle frozen time
  void togglePause();

  // Dust, shake and so on when the character lands hard
  void updateEffects();

  // Leave for another scene, no more ticks run here
  void changeScene(ProgramState state);

  GameAssets assets;

  asw::Font game_font;
  asw::Font help_font;
  asw::Font edit_font;

  UIHandler ui;
  Button* back_button{nullptr};

  // Declared before the boxes so the boxes go first
  std::unique_ptr<b2World> world;
  std::vector<std::unique_ptr<Box>> boxes;

  Character* character{nullptr};
  Goat* goat{nullptr};

  std::vector<std::string> help_text;

  // Shakes the world on death and hard landings
  asw::Camera camera{asw::Vec2<float>(SCREEN_WIDTH, SCREEN_HEIGHT)};

  // Kicked up by hard landings
  asw::ParticleEmitter dust;

  // Left behind when the character dies
  asw::ParticleEmitter burst;

  int level{1};

  // Time is frozen
  bool static_mode{true};

  // Time has not run since the level started, boxes placed in the air must
  // fall on the first unpause instead of sleeping
  bool first_play{true};

  bool leaving{false};
};
