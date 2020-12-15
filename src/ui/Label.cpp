#include "Label.h"

// Constructor
Label::Label() : UIElement() {}

// Detailed constructor
Label::Label(const int x,
             const int y,
             std::string text,
             std::string id,
             ALLEGRO_FONT* font)
    : UIElement(x, y, text, id, font) {
  if (font != nullptr) {
    this->width = al_get_text_width(font, text.c_str()) + padding_x;
    this->height = al_get_font_line_height(font);
  } else {
    this->width = 10;
    this->height = 10;
  }
}

// Draw label
void Label::draw() {
  // Do not draw if not visible
  if (!visible)
    return;

  // Draw text
  if (UIElement_font != nullptr && getText() != "") {
    al_draw_text(UIElement_font, text_colour, x + padding_x + padding_x,
                 y + padding_y, 0, text.c_str());
  }
}

// Update label
void Label::update() {}
