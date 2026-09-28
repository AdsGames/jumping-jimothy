#include "ExplosiveButton.h"

namespace {
constexpr float PADDING = 2;
}  // namespace

ExplosiveButton::ExplosiveButton(const asw::Texture& image, float angle)
    : image(image), angle(angle) {
  padding = PADDING;

  const auto size = asw::util::get_texture_size(image);
  transform.size = size + asw::Vec2<float>(PADDING * 2, PADDING * 2);
}

void ExplosiveButton::draw(asw::ui::Context& ctx) {
  // Background and border
  Button::draw(ctx);

  const auto size = asw::util::get_texture_size(image);
  asw::draw::stretch_sprite_rotate_blit(
      image, asw::Quad<float>(0, 0, size.x, size.y),
      asw::Quad<float>(transform.position.x + padding,
                       transform.position.y + padding, size.x, size.y),
      angle);
}
