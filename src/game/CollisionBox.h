/**
 * Collision Box
 * Danny Van Stemp
 * Invisible solid box, gives static boxes their collision.
 *   Hold G to see them.
 * 09/07/2017
 **/

#pragma once

#include "Box.h"

class CollisionBox : public Box {
 public:
  CollisionBox(float x, float y, float width, float height, b2World& world);

  void draw() const override;
  BoxType getType() const override { return BoxType::Collision; }
};
