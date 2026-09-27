/**
 * Fixed Scene
 * Allan Legemaate
 * Scene that runs its logic at a fixed 60 ticks per second
 * 27/09/2026
 **/

#pragma once

#include <asw/asw.h>

#include "State.h"

class FixedScene : public asw::scene::Scene<ProgramState> {
 public:
  using asw::scene::Scene<ProgramState>::Scene;

  // Runs zero or more fixed ticks for the time that passed. On desktop asw
  // calls this at the tick rate, in the browser once per frame.
  void update(float dt) final;

 protected:
  // One fixed step of game logic
  virtual void tick() = 0;

 private:
  // Time not yet simulated, in seconds
  float lag{0.0F};
};
