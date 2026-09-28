/**
 * Goat
 * Danny Van Stemp
 * The game goal
 * 06/05/2017
 **/

#pragma once

#include "../Globals.h"
#include "Box.h"
#include "Character.h"
#include "GameAssets.h"

class Goat : public Box {
 public:
  Goat(float x, float y, const GameAssets& assets, b2World& world);

  void update(b2World& world) override;
  void draw(const asw::Camera& camera) const override;
  BoxType getType() const override { return BoxType::Goat; }

  void setCharacter(const Character* character) {
    this->character = character;
  }

  // The character reached the goat
  bool getWinCondition() const;

 private:
  const GameAssets& assets;
  const Character* character{nullptr};

  // 15 frames, each shown for 11 ticks
  asw::Animation animation{15, 11 * TICK_SECONDS};
};
