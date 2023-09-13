/**
 * Explosive
 * Danny Van Stemp
 * Box that applys gravity in a direction
 * 21/09/2017
 **/

#ifndef EXPLOSIVE_H
#define EXPLOSIVE_H

#include "Box.h"
#include "Character.h"

class Explosive : public Box {
 public:
  // Constructor
  Explosive(const float x,
            const float y,
            const bool affectCharacter,
            Character* character,
            b2World* world);

  // Draw
  void draw() override;

  // Update
  void update(b2World* world) override;

  // Apply impulse
  void applyBlastImpulse(b2Body* body,
                         b2Vec2 blastCenter,
                         b2Vec2 applyPoint,
                         const float blastPower);

  // Get type
  int getType() override;

 private:
  // Pointer to character
  Character* gameCharacter{nullptr};

  // Blast radius
  float blastRadius{10.0f};

  // Power of explosive
  float blastPower{1000.0f};

  // Can affect character or not
  bool affect_character{false};

  // Is currently exploding
  bool is_exploding{false};
};

#endif  // EXPLOSIVE_H
