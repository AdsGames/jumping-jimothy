#include "CheckBox.h"

#include <utility>

#include "../util/Graphics.h"

CheckBox::CheckBox(float x, float y, std::string text, asw::Font font)
    : UIElement(x, y, std::move(text), std::move(font)) {
  width += checkbox_size + padding_x;
}

void CheckBox::update() {
  toggled = false;

  if (clicked()) {
    checked = !checked;
    toggled = true;
  }
}

void CheckBox::draw() {
  if (!visible) {
    return;
  }

  const auto shade =
      static_cast<uint8_t>((hover() || focused) ? 220 : 200);
  const auto black = withAlpha(asw::Color(0, 0, 0));

  // Backdrop
  if (visible_background) {
    const asw::Quad<float> bounds(x, y, getWidth(), getHeight());
    asw::draw::rect_fill(bounds, withAlpha(asw::Color(shade, shade, shade)));
    gfx::thickRect(bounds, 2, black);
  }

  // Box
  const asw::Quad<float> box(x + width - checkbox_size, y + padding_y,
                             checkbox_size, checkbox_size);
  asw::draw::rect_fill(box, withAlpha(asw::Color(shade, shade, shade)));
  gfx::thickRect(box, 2, black);

  // Tick
  if (checked) {
    asw::draw::rect_fill(
        asw::Quad<float>(box.position.x + 4, box.position.y + 4,
                         checkbox_size - 8, checkbox_size - 8),
        black);
  }

  // Label
  if (font != nullptr && !text.empty()) {
    asw::draw::text(font, text,
                    asw::Vec2<float>(x + padding_x, y + padding_y + 2),
                    withAlpha(text_colour));
  }
}
