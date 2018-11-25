#include "ui/Button.h"

#include "Tools.h"

// Default empty button
Button::Button()
  : UIElement() {
}

// Ctor
Button::Button(int x, int y, std::string text, std::string id, ALLEGRO_FONT *font)
  : UIElement(x, y, text, id, font) {
}

// Draw button
void Button::draw() {
  // Do not draw if not visible
  if (!visible)
    return;

  // This hover colour has more lines than the whole game loop
  int new_r = tools::negative_clamp_thing(0, 255, (int)((background_colour.r * 255) + (40 * hover())));
  int new_g = tools::negative_clamp_thing(0, 255, (int)((background_colour.g * 255) + (40 * hover())));
  int new_b = tools::negative_clamp_thing(0, 255, (int)((background_colour.b * 255) + (40 * hover())));

  ALLEGRO_COLOR hover_colour = al_map_rgba(new_r, new_g, new_b, alpha);

  if (!hover_effect)
    hover_colour = background_colour;

  // Draw button background
  if (visible_background) {
    if (!transparent_cell_fill)
      al_draw_filled_rectangle(x, y, x + width + padding_x * 2, y + height + padding_y * 2, hover_colour);
    al_draw_rectangle(x, y, x + width + padding_x * 2, y + height + padding_y * 2, al_map_rgba(0, 0, 0,alpha), border_thickness);
  }

  // Text
  if (UIElement_font != nullptr) {
    if (justification == 0) {
      al_draw_text(UIElement_font, text_colour, x + padding_x, y + padding_y, 0, text.c_str());
    }
    if (justification == 1) {
      int text_x = x + padding_x + width / 2;
      int text_y = y + padding_y - (tools::get_text_height(UIElement_font,text) - height) / 2 - tools::get_text_offset_y(UIElement_font, text);
      al_draw_textf(UIElement_font, text_colour, text_x, text_y, justification, text.c_str());
    }
  }

  // Image if avail
  if (image != nullptr) {
    if (bitmap_rotation_angle == 0)
      al_draw_bitmap( image, x + padding_x, y + padding_y, 0);
    else
      al_draw_rotated_bitmap(image, width / 2, width / 2, x + padding_x + width / 2, y + padding_y + height / 2, bitmap_rotation_angle, 0);
  }
}

// Update button
void Button::update() {

}
