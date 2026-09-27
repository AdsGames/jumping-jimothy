#include "Label.h"

void Label::draw() {
  if (!visible || font == nullptr || text.empty()) {
    return;
  }

  asw::draw::text(font, text,
                  asw::Vec2<float>(x + (padding_x * 2), y + padding_y),
                  withAlpha(text_colour));
}
