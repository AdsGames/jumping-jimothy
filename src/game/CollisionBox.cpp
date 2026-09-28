#include "CollisionBox.h"

#include "../Globals.h"

CollisionBox::CollisionBox(float x,
                           float y,
                           float width,
                           float height,
                           b2World& world)
    : Box(x, y, width, height) {
  createBody(world, b2_kinematicBody);
}

void CollisionBox::draw(const asw::Camera& camera) const {
  if (!asw::input::get_key(asw::input::Key::G)) {
    return;
  }

  asw::draw::rect_fill(screenQuad(camera, (getWidth() * PIXELS_PER_METER) - 2,
                                  (getHeight() * PIXELS_PER_METER) - 2),
                       asw::Color(255, 0, 0));
}
