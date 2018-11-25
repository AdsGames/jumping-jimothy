#include "ui/CheckBox.h"

// Default constructor
CheckBox::CheckBox()
  : UIElement()  {
  this -> checkbox_size = 20;
  this -> checked = false;
}

// Detailed constructor
CheckBox::CheckBox(int x, int y, std::string text, std::string id, ALLEGRO_FONT *font)
  : UIElement(x, y, text, id, font) {

  this -> checkbox_size = 20;
  this -> checked = false;

  if(font != nullptr) {
    this -> width = al_get_text_width(font, text.c_str()) + checkbox_size + padding_x;
    this -> height = al_get_font_line_height(font);
  }
  else {
    this -> width = 10;
    this -> height = 10;
  }
}

// Toggle check box
void CheckBox::update() {
  if(!disabled && clicked()) {
    checked =! checked;
  }
}

// Draw check box
void CheckBox::draw() {
  if(visible) {
    // Backdrop
    al_draw_filled_rectangle(x, y, x + width + padding_x * 2, y + height + padding_y * 2, al_map_rgba(200 + 20 * hover(), 200 + 20 * hover(), 200 + 20 * hover(),alpha));
    al_draw_rectangle(x, y, x + width + padding_x * 2, y + height + padding_y * 2, al_map_rgba(0, 0, 0,alpha), 2);

    // Checkbox
    al_draw_filled_rectangle(x + padding_x, y + padding_y, x + padding_x + checkbox_size, y + padding_y + checkbox_size, al_map_rgba(200 + 20 * hover(), 200 + 20 * hover(), 200 + 20 * hover(),alpha));
    al_draw_rectangle(x + padding_x, y + padding_y, x + padding_x+checkbox_size, y + padding_y + checkbox_size, al_map_rgba(0, 0, 0,alpha), 2);

    if (checked)
      al_draw_filled_rectangle(x + padding_x + 2, y + padding_y + 2, x + padding_x +checkbox_size - 2, y + padding_y + checkbox_size - 2, al_map_rgba(0, 0, 0, alpha));

    // Text
    if (UIElement_font != nullptr)
      al_draw_text(UIElement_font, al_map_rgba(0, 0, 0,alpha), x + padding_x + checkbox_size + padding_x, y + padding_y, 0, text.c_str());
  }
}

// Get checked state
bool CheckBox::getChecked() {
  return checked;
}
