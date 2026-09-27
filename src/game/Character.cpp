#include "Character.h"

#include <algorithm>
#include <cmath>

#include "../util/ActionBinder.h"
#include "../util/Graphics.h"

namespace {
constexpr float WIDTH = 0.8F;
constexpr float HEIGHT = 2.5F;

// Sprite sheet layout
constexpr float FRAME_WIDTH = 32;
constexpr float FRAME_HEIGHT = 64;
constexpr int RUN_FRAMES = 14;
constexpr int IDLE_FRAME = 14;

// Sprite is drawn slightly above the centre of the body
const asw::Vec2<float> SPRITE_OFFSET{0, -6};

// Movement
constexpr float X_VELOCITY_GROUND = 6.0F;
constexpr float X_IMPULSE_AIR = 3.1F;
constexpr float X_VELOCITY_AIR_MAX = 4.0F;
constexpr float JUMP_IMPULSE = 17.0F;

// Ticks between jumps and jump sounds
constexpr int JUMP_DELAY = 20;
constexpr int SOUND_DELAY = 10;

// Ticks on the ground before a jump is allowed
constexpr int LAND_TICKS = 25;

// A contact counts as ground when its normal points at least this far up,
// which allows slopes up to about 60 degrees and never counts walls
constexpr float GROUND_NORMAL_Y = 0.5F;

// Friction while standing still holds the character on slopes up to 45
// degrees. It is off while moving so the character does not stick to walls.
constexpr float STANDING_FRICTION = 1.0F;
}  // namespace

Character::Character(float x,
                     float y,
                     const GameAssets& assets,
                     b2World& world)
    : Box(x, y, WIDTH, HEIGHT), assets(assets) {
  createBody(world, b2_dynamicBody);
  body->SetTransform(b2Vec2(x, y + 0.40F), 0);
  body->SetFixedRotation(true);

  // Contacts start without friction, standing still turns it on
  body->GetFixtureList()->SetFriction(0.0F);
}

Character::Ground Character::findGround() const {
  Ground result;
  float best_normal_y = GROUND_NORMAL_Y;

  for (auto* edge = body->GetContactList(); edge != nullptr;
       edge = edge->next) {
    auto* contact = edge->contact;
    if (!contact->IsTouching() || !contact->IsEnabled() ||
        contact->GetFixtureA()->IsSensor() ||
        contact->GetFixtureB()->IsSensor()) {
      continue;
    }

    // The normal points from fixture A to fixture B, flip it so it points
    // from the other body up to the character
    b2WorldManifold manifold;
    contact->GetWorldManifold(&manifold);
    const b2Vec2 normal = contact->GetFixtureA()->GetBody() == body
                              ? -manifold.normal
                              : manifold.normal;

    if (normal.y < best_normal_y) {
      continue;
    }

    // Middle of the contact points
    const int point_count = contact->GetManifold()->pointCount;
    b2Vec2 point(0, 0);
    for (int i = 0; i < point_count; i++) {
      point += manifold.points[i];
    }
    point *= 1.0F / static_cast<float>(std::max(point_count, 1));

    best_normal_y = normal.y;
    result.body = edge->other;
    result.velocity = edge->other->GetLinearVelocityFromWorldPoint(point);
  }

  return result;
}

void Character::setContactFriction(float friction) const {
  for (auto* edge = body->GetContactList(); edge != nullptr;
       edge = edge->next) {
    edge->contact->SetFriction(friction);
  }
}

void Character::update(b2World& /*world*/) {
  ground = findGround();
  const bool grounded = isGrounded();

  const auto velocity = body->GetLinearVelocity();
  const auto relative_velocity = velocity - ground.velocity;

  counter_ground_contact = grounded ? counter_ground_contact + 1 : 0;

  if (counter_ground_contact > LAND_TICKS) {
    landed = true;
  }

  // Just landed, louder when falling faster
  if (grounded && std::abs(relative_velocity.y) <= 0.01F &&
      velocity_old < -0.01F) {
    const float volume = std::min(-velocity_old / 20.0F, 1.0F);
    asw::sound::play(assets.land, volume);
    landed = true;
  }

  velocity_old = relative_velocity.y;

  // Standing on a box always counts as landed
  if (grounded && ground.body->GetType() == b2_dynamicBody) {
    landed = true;
  }

  // Animation runs faster in the air
  const int ticks_per_frame = grounded ? 5 : 2;

  tick++;
  if (tick >= ticks_per_frame) {
    frame = (frame + 1) % RUN_FRAMES;
    tick = 0;
  }

  // Walk on the ground, push in the air. Ground speeds are relative to what
  // the character stands on, so it rides moving boxes.
  const auto position = body->GetPosition();
  const bool left = ActionBinder::actionHeld(Action::Left);
  const bool right = !left && ActionBinder::actionHeld(Action::Right);

  if (left || right) {
    direction = right;
    const float sign = right ? 1.0F : -1.0F;

    if (grounded) {
      body->SetLinearVelocity(
          b2Vec2(ground.velocity.x + (sign * X_VELOCITY_GROUND), velocity.y));
    } else if (sign * velocity.x < X_VELOCITY_AIR_MAX) {
      body->ApplyLinearImpulse(b2Vec2(sign * X_IMPULSE_AIR, 0), position,
                               true);
    }
  } else if (grounded) {
    body->SetLinearVelocity(b2Vec2(ground.velocity.x, velocity.y));
  }

  // Only grip when standing still on the ground
  setContactFriction(grounded && !left && !right ? STANDING_FRICTION : 0.0F);

  // Jumping Jimothy
  timer_jump_delay++;

  if (ActionBinder::actionBegun(Action::A) && grounded &&
      body->GetLinearVelocity().y - ground.velocity.y < 0.1F && landed &&
      timer_jump_delay > JUMP_DELAY) {
    timer_jump_delay = 0;
    body->ApplyLinearImpulse(b2Vec2(0, JUMP_IMPULSE), position, true);
    landed = false;

    if (timer_sound_delay > SOUND_DELAY) {
      asw::sound::play(assets.jump);
      timer_sound_delay = 0;
    }
  }

  timer_sound_delay++;
}

void Character::draw() const {
  const bool moving = body->GetLinearVelocity().Length() > 0.1F;
  const int sprite_frame = moving ? frame : IDLE_FRAME;

  gfx::region(assets.character,
              asw::Quad<float>(static_cast<float>(sprite_frame) * FRAME_WIDTH,
                               0, FRAME_WIDTH, FRAME_HEIGHT),
              screenQuad(FRAME_WIDTH, FRAME_HEIGHT, SPRITE_OFFSET),
              screenAngle(), !direction);
}
