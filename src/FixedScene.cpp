#include "FixedScene.h"

#include <algorithm>

#include "Globals.h"
#include "util/Input.h"

namespace {
// Longest frame time simulated at once, stops a slow frame or a resumed
// browser tab from running hundreds of ticks
constexpr float MAX_LAG_SECONDS = 0.1F;

// Absorbs float error when exactly one tick of time passed
constexpr float TICK_EPSILON = 0.0001F;
}  // namespace

void FixedScene::update(float dt) {
  input::poll();

  lag = std::min(lag + dt, MAX_LAG_SECONDS);

  while (lag >= TICK_SECONDS - TICK_EPSILON) {
    tick();
    input::endTick();
    lag -= TICK_SECONDS;
  }
}
