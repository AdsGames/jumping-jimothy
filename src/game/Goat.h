/**
 * Goat
 * Danny Van Stemp
 * The game goal
 * 06/05/2017
 **/

#pragma once

#include "Box.h"
#include "Character.h"
#include "GameAssets.h"

class Goat : public Box {
 public:
  Goat(float x, float y, const GameAssets& assets, b2World& world);

  void update(b2World& world) override;
  void draw() const override;
  BoxType getType() const override { return BoxType::Goat; }

  void setCharacter(const Character* character) {
    this->character = character;
  }

  // The character reached the goat
  bool getWinCondition() const;

 private:
  const GameAssets& assets;
  const Character* character{nullptr};

  // Animation
  int frame{0};
  int tick{0};
};
