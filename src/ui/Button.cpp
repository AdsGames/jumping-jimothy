#include "Button.h"

#include <algorithm>

#include "../util/Graphics.h"

namespace {
// Brighten a colour when the button is hovered or focused
constexpr int HOVER_BRIGHTEN = 40;

uint8_t brighten(uint8_t value) {
  return static_cast<uint8_t>(std::min(255, value + HOVER_BRIGHTEN));
}
}  // namespace

void Button::draw() {
  if (!visible) {
    return;
  }

  const asw::Quad<float> bounds(x, y, getWidth(), getHeight());

  // Background
  if (visible_background) {
    if (!transparent_cell_fill) {
      auto fill = background_colour;
      if (hover() || focused) {
        fill = asw::Color(brighten(fill.r), brighten(fill.g), brighten(fill.b));
      }
      asw::draw::rect_fill(bounds, withAlpha(fill));
    }

    gfx::thickRect(bounds, border_thickness, withAlpha(asw::Color(0, 0, 0)));
  }

  // Text
  if (font != nullptr && !text.empty()) {
    if (justification == TextJustify::Center) {
      const float text_y =
          y + padding_y + ((height - gfx::lineHeight(font)) / 2.0F);
      asw::draw::text(font, text,
                      asw::Vec2<float>(x + padding_x + (width / 2.0F), text_y),
                      withAlpha(text_colour), asw::TextJustify::Center);
    } else {
      asw::draw::text(font, text, asw::Vec2<float>(x + padding_x, y + padding_y),
                      withAlpha(text_colour));
    }
  }

  // Image
  if (image != nullptr) {
    const auto size = asw::util::get_texture_size(image);
    gfx::region(image, asw::Quad<float>(0, 0, size.x, size.y),
                asw::Quad<float>(x + padding_x, y + padding_y, size.x, size.y),
                image_rotation);
  }
}
