#include "ui/CheckBox.h"

// Default constructor
CheckBox::CheckBox()
  : UIElement()  {
  this -> checkbox_size = 20;
  this -> checked = false;
  this -> toggled = false;
}

// Detailed constructor
CheckBox::CheckBox(int x, int y, std::string text, std::string id, ALLEGRO_FONT *font)
  : UIElement(x, y, text, id, font) {

  this -> checkbox_size = 20;
  this -> checked = false;
  this -> toggled = false;

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
  toggled = false;
  if(!disabled && clicked()) {
    checked =! checked;
    toggled = true;
  }
}

// Draw check box
void CheckBox::draw() {
  // Do not draw if not visible
  if (!visible)
    return;

  // Backdrop
  if (visible_background) {
    al_draw_filled_rectangle(x, y, x + width + padding_x * 2, y + height + padding_y * 2, al_map_rgba(200 + 20 * hover(), 200 + 20 * hover(), 200 + 20 * hover(),alpha));
    al_draw_rectangle(x, y, x + width + padding_x * 2, y + height + padding_y * 2, al_map_rgba(0, 0, 0,alpha), 2);
  }

  // Checkbox
  al_draw_filled_rectangle(x + width - checkbox_size, y + padding_y, x + width, y + padding_y + checkbox_size, al_map_rgba(200 + 20 * hover(), 200 + 20 * hover(), 200 + 20 * hover(),alpha));
  al_draw_rectangle(x + width - checkbox_size, y + padding_y, x + width, y + padding_y + checkbox_size, al_map_rgba(0, 0, 0,alpha), 2);

  // Fill box
  if (checked) {
    al_draw_filled_rectangle(x + width - checkbox_size + 2, y + padding_y + 2, x + width - 2, y + padding_y + checkbox_size - 2, al_map_rgba(0, 0, 0, alpha));
  }

  // Label
  if (UIElement_font != nullptr && getText() != "") {
    al_draw_text(UIElement_font, text_colour, x + padding_x, y + padding_y + 2, 0, text.c_str());
  }
}

// Get checked state
bool CheckBox::getChecked() {
  return checked;
}

// Set checked state
void CheckBox::setChecked(bool checked) {
  this -> checked = checked;
}

// Set checked state
bool CheckBox::getToggled() {
  return toggled;
}
