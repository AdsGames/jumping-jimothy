#include "UIElement.h"

#include <allegro5/allegro5.h>

#include "MouseListener.h"
#include "Tools.h"

// Defaults
UIElement::UIElement() {
  this -> alpha = 255;
  this -> background_colour = al_map_rgba(200, 200, 200, alpha);
  this -> text_colour = al_map_rgba(0, 0, 0, alpha);
  this -> x = 0;
  this -> y = 0;
  this -> width = 10;
  this -> height = 10;
  this -> text = "";
  this -> UIElement_font = nullptr;
  this -> visible = true;
  this -> disabled = false;
  this -> image = nullptr;
  this -> padding_x = 10;
  this -> padding_y = 10;
  this -> id = "";
  this -> visible_background = true;
  this -> justification = 0;
  this -> border_thickness = 2;
  this -> transparent_cell_fill = false;
  this -> hover_effect = true;
  this -> bitmap_rotation_angle = 0;
}

UIElement::UIElement(int x, int y, std::string text, std::string id, ALLEGRO_FONT *UIElement_font)
  : UIElement() {

  setId(id);
  setX(x);
  setY(y);
  setText(text);
  setFont(UIElement_font);
}

// Destruct
UIElement::~UIElement() {
  if(image != nullptr) {
    al_destroy_bitmap(image);
  }
}

// Get X
int UIElement::getX() {
  return x;
}

// Get y
int UIElement::getY() {
  return y;
}

// Get text of element
std::string UIElement::getText() {
  return text;
}

// Get element ID
std::string UIElement::getId() {
  return id;
}

// Hide
void UIElement::hide() {
  this -> visible = false;
}

// Show
void UIElement::show() {
  this -> visible = true;
}

// Toggle visiblity
void UIElement::toggleVisibility() {
  visible =! visible;
}

// Disable element
void UIElement::disable() {
  disabled = true;
}

// Enable element
void UIElement::enable() {
  disabled = false;
}

// Togle disabled
void UIElement::toggleDisabled() {
  disabled =! disabled;
}

// Set transparency level
void UIElement::setTransparency(float alpha) {
  this -> alpha = alpha;
}

// Set image rotation
void UIElement::setBitmapRotationAngle(float rotation) {
  bitmap_rotation_angle = rotation;
}

// Set x
void UIElement::setX(int x) {
  this -> x = x;
}

// Set y
void UIElement::setY(int y) {
  this -> y = y;
}

// Set text colour
void UIElement::setTextColour(ALLEGRO_COLOR colour) {
  text_colour = colour;
}

// Set background colour
void UIElement::setBackgroundColour(ALLEGRO_COLOR colour) {
  background_colour = colour;
}

// Set cell fill
void UIElement::setCellFillTransparent(bool n) {
  transparent_cell_fill = n;
}

// Set text justification
void UIElement::setTextJustification(int justification) {
  this -> justification = justification;
}

// Element width
int UIElement::getWidth() {
  return width + padding_x * 2;
}

// Element height
int UIElement::getHeight() {
  return height + padding_y * 2;
}

// Set padding
void UIElement::setPadding(int padding_x, int padding_y) {
  this -> padding_x = padding_x;
  this -> padding_y = padding_y;
}

// Set position
void UIElement::setPosition(int x, int y) {
  setX(x);
  setY(y);
}

// Set element size
void UIElement::setSize(int width, int height) {
  setWidth(width);
  setHeight(height);
}

// Set text
void UIElement::setText(std::string text) {
  this -> text = text;
}

// Set id
void UIElement::setId(std::string id) {
  this -> id = id;
}

// Sets an image
void UIElement::setImage(ALLEGRO_BITMAP *image) {
  if (image == nullptr)
    return;
  this -> image = image;
  this -> width = al_get_bitmap_width(this -> image);
  this -> height = al_get_bitmap_height(this -> image);
}

// Set new font
void UIElement::setFont(ALLEGRO_FONT *font) {
  if (font == nullptr)
    return;
  this -> UIElement_font = font;
  this -> width = al_get_text_width(UIElement_font, text.c_str());
  this -> height = al_get_font_line_height(UIElement_font);
}

// Set background visibility
void UIElement::setVisibleBackground(bool b) {
  visible_background = b;
}

// Set width
void UIElement::setWidth(int width) {
  this -> width = width;
}

// Set height
void UIElement::setHeight(int height) {
  this -> height = height;
}

// Set border thickness
void UIElement::setBorderThickness(int thickness) {
  border_thickness = thickness;
}

// Disable hover effect
void UIElement::disableHoverEffect() {
  hover_effect = false;
}

// Disable hover effect
void UIElement::enableHoverEffect() {
  hover_effect = true;
}

// Checked or not
bool UIElement::getChecked() {
  return false;
}

// True if hovering
bool UIElement::hover() {
  return tools::mouse_over(getX(), getY(), getWidth(), getHeight());
}

// True if clicked
bool UIElement::clicked() {
  return !disabled && hover() && tools::mouse_clicked(MOUSE_LEFT);
}

// Draw default ui element
void UIElement::draw() {
  if (visible) {
    // Backdrop
    if (visible_background) {
      al_draw_filled_rectangle(x, y, x + width + padding_x * 2, y + height + padding_y * 2, al_map_rgba( 200 + 20 * hover(), 200 + 20 * hover(), 200 + 20 * hover(),alpha));
      al_draw_rectangle(x, y, x + width + padding_x * 2, y + height + padding_y * 2, al_map_rgba( 0, 0, 0,alpha), 2);
    }
    // Text
    if (UIElement_font != nullptr) {
      if (justification == 0) {
        al_draw_text(UIElement_font, text_colour, x + padding_x, y + padding_y, 0, text.c_str());
      }

      if(justification == 1){
        int text_x;
        int text_y;

        text_x = x + padding_x + width / 2;
        text_y = y + padding_y - (tools::get_text_height(UIElement_font,text) - height) / 2 - tools::get_text_offset_y(UIElement_font,text);

        al_draw_textf(UIElement_font, text_colour, text_x, text_y, justification, text.c_str());
      }
    }

    // Image if avail
    if( image != nullptr) {
      al_draw_bitmap(image, x + padding_x, y + padding_y, 0);
    }
  }
}
