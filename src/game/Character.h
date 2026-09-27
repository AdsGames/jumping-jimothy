/**
 * Character
 * Danny Van Stemp
 * The game character
 * 05/05/2017
 **/

#pragma once

#include "Box.h"
#include "GameAssets.h"

class Character : public Box {
 public:
  Character(float x, float y, const GameAssets& assets, b2World& world);

  void update(b2World& world) override;
  void draw() const override;
  BoxType getType() const override { return BoxType::Character; }

  // Standing on something, updated each tick
  bool isGrounded() const { return ground.body != nullptr; }

 private:
  // What the character stands on
  struct Ground {
    // Nullptr when in the air
    b2Body* body{nullptr};

    // Velocity of the ground under the character
    b2Vec2 velocity{0, 0};
  };

  // Find the ground from the contacts of the last physics step
  Ground findGround() const;

  // Set the friction of every contact the character has
  void setContactFriction(float friction) const;

  const GameAssets& assets;

  Ground ground;

  // Animation
  int tick{0};
  int frame{0};

  // Ticks since the last jump and jump sound
  int timer_jump_delay{0};
  int timer_sound_delay{0};

  // Ticks on the ground
  int counter_ground_contact{0};

  // Allowed to jump
  bool landed{false};

  // Y velocity relative to the ground last tick
  float velocity_old{0};

  // Facing right
  bool direction{false};
};
