/**
 * Explosive
 * Danny Van Stemp
 * Box that pushes dynamic bodies away, or in one direction
 * 21/09/2017
 **/

#pragma once

#include "Box.h"
#include "Character.h"
#include "GameAssets.h"

class Explosive : public Box {
 public:
  // Orientation 0 pushes away in all directions, 1-4 pushes up, right, down
  // or left
  Explosive(float x,
            float y,
            int orientation,
            bool affect_character,
            const GameAssets& assets,
            b2World& world);

  void update(b2World& world) override;
  void draw(const asw::Camera& camera) const override;
  BoxType getType() const override { return BoxType::Explosive; }

  void setCharacter(const Character* character) {
    this->character = character;
  }

 private:
  // True if the target was pushed
  bool applyBlastImpulse(b2Body* target,
                         const b2Vec2& blast_centre,
                         const b2Vec2& apply_point) const;

  const GameAssets& assets;
  const Character* character{nullptr};
  int orientation{0};
  bool affect_character{false};

  // Sprays while the explosive pushes something. Drawing particles changes
  // no game state, so draw() stays const.
  mutable asw::ParticleEmitter particles;
};
