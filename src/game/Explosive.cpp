#include "Explosive.h"

#include <algorithm>
#include <numbers>
#include <vector>

#include "../Globals.h"

namespace {
constexpr float SIZE = 1.55F;
constexpr float IMAGE_SIZE = 32;

constexpr float BLAST_RADIUS = 10.0F;
constexpr float BLAST_POWER = 1000.0F * 0.05F;
constexpr float MAX_IMPULSE = 500.0F;

// Impulse direction of the one way explosives
constexpr float DIRECTIONAL_MAGNITUDE = 0.2F;

// Particles per second while pushing
constexpr float PARTICLE_RATE = 40.0F;

// Half the spread of a one way spray, in radians
constexpr float PARTICLE_SPREAD = 0.35F;

// Spray matching the push, screen angles so up is -pi/2
asw::ParticleConfig particleConfig(int orientation, asw::Color colour) {
  asw::ParticleConfig config;
  config.lifetime_min = 0.3F;
  config.lifetime_max = 0.6F;
  config.speed_min = 60.0F;
  config.speed_max = 140.0F;
  config.size_start = 4.0F;
  config.size_end = 1.0F;
  config.color_start = colour;
  config.color_end = asw::Color(colour.r, colour.g, colour.b, 0);

  if (orientation >= 1 && orientation <= 4) {
    const float centre = (std::numbers::pi_v<float> / 2.0F) *
                         static_cast<float>(orientation - 2);
    config.angle_min = centre - PARTICLE_SPREAD;
    config.angle_max = centre + PARTICLE_SPREAD;
  }

  return config;
}

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

  const auto colour =
      affect_character ? asw::Color(255, 80, 80) : asw::Color(80, 255, 80);
  particles = asw::ParticleEmitter(particleConfig(orientation, colour), 64);
  particles.transform.position = pixelPosition();
  particles.set_emission_rate(PARTICLE_RATE);
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
  bool pushing = false;
  for (auto* target : query.bodies) {
    const auto target_centre = target->GetWorldCenter();

    if ((target_centre - centre).Length() < BLAST_RADIUS) {
      pushing = applyBlastImpulse(target, centre, target_centre) || pushing;
    }
  }

  if (pushing) {
    particles.start();
  } else {
    particles.stop();
  }

  particles.update(TICK_SECONDS);
}

bool Explosive::applyBlastImpulse(b2Body* target,
                                  const b2Vec2& blast_centre,
                                  const b2Vec2& apply_point) const {
  // Ignore itself, non dynamic bodies, and the character unless allowed
  if (target == body || target->GetType() != b2_dynamicBody) {
    return false;
  }

  if (!affect_character && character != nullptr &&
      target == character->getBody()) {
    return false;
  }

  b2Vec2 direction = apply_point - blast_centre;
  const float distance = direction.Normalize();

  // Direction is undefined at the centre
  if (distance < 0.01F) {
    return false;
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
  return true;
}

void Explosive::draw(const asw::Camera& camera) const {
  const float fill = (SIZE * PIXELS_PER_METER) - 2;
  const auto colour =
      affect_character ? asw::Color(255, 0, 0) : asw::Color(0, 255, 0);
  asw::draw::rect_fill_rotate(screenQuad(camera, fill, fill), screenAngle(), colour);

  // Directional image points up, turn a quarter per orientation step
  const auto& image =
      orientation == 0 ? assets.box_repel : assets.box_repel_direction;
  const float angle = (std::numbers::pi_v<float> / 2.0F) *
                      static_cast<float>(orientation - 1);

  asw::draw::stretch_sprite_rotate_blit(
      image, asw::Quad<float>(0, 0, IMAGE_SIZE, IMAGE_SIZE),
      screenQuad(camera, IMAGE_SIZE, IMAGE_SIZE), angle + screenAngle());

  particles.draw(camera);
}
