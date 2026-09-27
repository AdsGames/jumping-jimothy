#include "Box.h"

#include <cmath>

#include "../Globals.h"

namespace {
// Boxes this slow when paused go to sleep instead of keeping their velocity
constexpr float REST_LINEAR_X = 0.1F;
constexpr float REST_LINEAR_Y = 0.01F;
constexpr float REST_ANGULAR = 0.1F;
}  // namespace

Box::Box(float x, float y, float width, float height)
    : initial_position(x, y), size(width, height) {}

void Box::createBody(b2World& world, b2BodyType type) {
  b2BodyDef body_def;
  body_def.type = type;
  body_def.position = initial_position;

  b2PolygonShape shape;
  shape.SetAsBox(size.x / 2, size.y / 2);

  b2FixtureDef fixture_def;
  fixture_def.shape = &shape;
  fixture_def.density = 1.0F;
  fixture_def.friction = 0.3F;

  body = world.CreateBody(&body_def);
  body->CreateFixture(&fixture_def);
}

void Box::setPaused(bool pause, bool can_sleep) {
  if (!isPausable() || body == nullptr) {
    return;
  }

  is_paused = pause;

  if (is_paused) {
    paused_velocity = body->GetLinearVelocity();
    paused_angular_velocity = body->GetAngularVelocity();
    body->SetType(b2_staticBody);
    return;
  }

  body->SetType(b2_dynamicBody);

  const bool at_rest = can_sleep &&
                       std::abs(paused_velocity.x) <= REST_LINEAR_X &&
                       std::abs(paused_velocity.y) <= REST_LINEAR_Y &&
                       std::abs(paused_angular_velocity) <= REST_ANGULAR;

  if (at_rest) {
    body->SetAwake(false);
    body->SetLinearVelocity(b2Vec2(0, 0));
  } else {
    body->SetLinearVelocity(paused_velocity);
    body->SetAngularVelocity(paused_angular_velocity);
  }
}

float Box::getX() const {
  return body != nullptr ? body->GetPosition().x : initial_position.x;
}

float Box::getY() const {
  return body != nullptr ? body->GetPosition().y : initial_position.y;
}

float Box::getAngle() const {
  return body != nullptr ? body->GetAngle() : 0.0F;
}

asw::Vec2<float> Box::screenPosition() const {
  return {getX() * PIXELS_PER_METER, -getY() * PIXELS_PER_METER};
}

asw::Quad<float> Box::screenQuad(float w,
                                 float h,
                                 asw::Vec2<float> offset) const {
  const auto centre = screenPosition() + offset;
  return {centre.x - (w / 2), centre.y - (h / 2), w, h};
}
