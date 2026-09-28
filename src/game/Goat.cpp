#include "Goat.h"


namespace {
constexpr float WIDTH = 1.6F;
constexpr float HEIGHT = 3.2F;

// Sprite sheet layout
constexpr float FRAME_WIDTH = 32;
constexpr float FRAME_HEIGHT = 64;
constexpr int FRAMES = 15;
constexpr int TICKS_PER_FRAME = 11;
}  // namespace

Goat::Goat(float x, float y, const GameAssets& assets, b2World& world)
    : Box(x, y, WIDTH, HEIGHT), assets(assets) {
  createBody(world, b2_dynamicBody);
}

void Goat::update(b2World& /*world*/) {
  tick++;
  if (tick >= TICKS_PER_FRAME) {
    frame = (frame + 1) % FRAMES;
    tick = 0;
  }
}

void Goat::draw() const {
  asw::draw::stretch_sprite_rotate_blit(assets.goat,
              asw::Quad<float>(static_cast<float>(frame) * FRAME_WIDTH, 0,
                               FRAME_WIDTH, FRAME_HEIGHT),
              screenQuad(FRAME_WIDTH, FRAME_HEIGHT), screenAngle());
}

bool Goat::getWinCondition() const {
  if (character == nullptr) {
    return false;
  }

  for (const auto* edge = body->GetContactList(); edge != nullptr;
       edge = edge->next) {
    if (edge->other == character->getBody() && edge->contact->IsTouching()) {
      return true;
    }
  }

  return false;
}
