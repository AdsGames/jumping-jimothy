#include "Explosive.h"

#include <algorithm>
#include <numbers>
#include <vector>

#include "../Globals.h"
#include "../util/Graphics.h"

namespace {
constexpr float SIZE = 1.55F;
constexpr float IMAGE_SIZE = 32;

constexpr float BLAST_RADIUS = 10.0F;
constexpr float BLAST_POWER = 1000.0F * 0.05F;
constexpr float MAX_IMPULSE = 500.0F;

// Impulse direction of the one way explosives
constexpr float DIRECTIONAL_MAGNITUDE = 0.2F;

// Collects every body with a fixture in the query area
class QueryCallback : public b2QueryCallback {
 public:
  std::vector<b2Body*> bodies;

  bool ReportFixture(b2Fixture* fixture) override {
    bodies.push_back(fixture->GetBody());
    return true;
  }
};
}  // namespace

Explosive::Explosive(float x,
                     float y,
                     int orientation,
                     bool affect_character,
                     const GameAssets& assets,
                     b2World& world)
    : Box(x, y, SIZE, SIZE),
      assets(assets),
      orientation(orientation),
      affect_character(affect_character) {
  createBody(world, b2_kinematicBody);
}

void Explosive::update(b2World& world) {
  const auto centre = body->GetPosition();
  const b2Vec2 extent(BLAST_RADIUS, BLAST_RADIUS);

  QueryCallback query;
  b2AABB aabb;
  aabb.lowerBound = centre - extent;
  aabb.upperBound = centre + extent;
  world.QueryAABB(&query, aabb);

  // Push bodies whose centre of mass is inside the blast radius
  for (auto* target : query.bodies) {
    const auto target_centre = target->GetWorldCenter();

    if ((target_centre - centre).Length() < BLAST_RADIUS) {
      applyBlastImpulse(target, centre, target_centre);
    }
  }
}

void Explosive::applyBlastImpulse(b2Body* target,
                                  const b2Vec2& blast_centre,
                                  const b2Vec2& apply_point) const {
  // Ignore itself, non dynamic bodies, and the character unless allowed
  if (target == body || target->GetType() != b2_dynamicBody) {
    return;
  }

  if (!affect_character && character != nullptr &&
      target == character->getBody()) {
    return;
  }

  b2Vec2 direction = apply_point - blast_centre;
  const float distance = direction.Normalize();

  // Direction is undefined at the centre
  if (distance < 0.01F) {
    return;
  }

  const float inverse_distance = 1.0F / distance;
  const float magnitude = std::min(
      BLAST_POWER * inverse_distance * inverse_distance, MAX_IMPULSE);

  switch (orientation) {
    case 1:
      direction = b2Vec2(0, DIRECTIONAL_MAGNITUDE);
      break;
    case 2:
      direction = b2Vec2(DIRECTIONAL_MAGNITUDE, 0);
      break;
    case 3:
      direction = b2Vec2(0, -DIRECTIONAL_MAGNITUDE);
      break;
    case 4:
      direction = b2Vec2(-DIRECTIONAL_MAGNITUDE, 0);
      break;
    default:
      break;
  }

  target->ApplyLinearImpulse(magnitude * direction, apply_point, true);
}

void Explosive::draw() const {
  const float fill = (SIZE * PIXELS_PER_METER) - 2;
  const auto colour =
      affect_character ? asw::Color(255, 0, 0) : asw::Color(0, 255, 0);
  gfx::rotatedRectFill(screenQuad(fill, fill), screenAngle(), colour);

  // Directional image points up, turn a quarter per orientation step
  const auto& image =
      orientation == 0 ? assets.box_repel : assets.box_repel_direction;
  const float angle = (std::numbers::pi_v<float> / 2.0F) *
                      static_cast<float>(orientation - 1);

  gfx::region(image, asw::Quad<float>(0, 0, IMAGE_SIZE, IMAGE_SIZE),
              screenQuad(IMAGE_SIZE, IMAGE_SIZE), angle + screenAngle());
}
