#include "Goat.h"

#include "../Globals.h"


namespace {
constexpr float WIDTH = 1.6F;
constexpr float HEIGHT = 3.2F;

constexpr float FRAME_WIDTH = 32;
constexpr float FRAME_HEIGHT = 64;
}  // namespace

Goat::Goat(float x, float y, const GameAssets& assets, b2World& world)
    : Box(x, y, WIDTH, HEIGHT), assets(assets) {
  createBody(world, b2_dynamicBody);
}

void Goat::update(b2World& /*world*/) {
  animation.update(TICK_SECONDS);
}

void Goat::draw(const asw::Camera& camera) const {
  asw::draw::stretch_sprite_rotate_blit(assets.goat,
              assets.goat_sheet.get_frame(animation.get_frame()),
              screenQuad(camera, FRAME_WIDTH, FRAME_HEIGHT), screenAngle());
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
