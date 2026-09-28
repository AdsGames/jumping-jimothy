#include "UIElement.h"

#include <utility>

#include "../util/Controls.h"

UIElement::UIElement(float x, float y, std::string text, asw::Font font)
    : x(x), y(y), font(std::move(font)), text(std::move(text)) {
  setText(this->text);
}

void UIElement::setPosition(float x, float y) {
  this->x = x;
  this->y = y;
}

void UIElement::setSize(float width, float height) {
  this->width = width;
  this->height = height;
}

void UIElement::setPadding(float x, float y) {
  padding_x = x;
  padding_y = y;
}

// Fit the element to its text
void UIElement::setText(const std::string& text) {
  this->text = text;

  if (font != nullptr) {
    width = static_cast<float>(asw::util::get_text_size(font, text).x);
    height = static_cast<float>(asw::util::get_font_height(font));
  }
}

// Fit the element to its image
void UIElement::setImage(const asw::Texture& image) {
  this->image = image;

  if (image != nullptr) {
    const auto size = asw::util::get_texture_size(image);
    width = size.x;
    height = size.y;
  }
}

void UIElement::setVisible(bool visible) {
  this->visible = visible;
  disabled = !visible;
}

bool UIElement::hover() const {
  if (!visible) {
    return false;
  }

  const auto mouse = asw::input::get_mouse().position;
  return mouse.x >= x && mouse.x < x + getWidth() && mouse.y >= y &&
         mouse.y < y + getHeight();
}

bool UIElement::clicked() const {
  if (disabled) {
    return false;
  }

  // One element takes each click, even where elements overlap
  if (!click_taken && hover() &&
      asw::input::get_mouse_button_up(asw::input::MouseButton::Left)) {
    click_taken = true;
    return true;
  }

  return focused && asw::input::get_action_down(Controls::SELECT);
}

asw::Color UIElement::withAlpha(asw::Color colour) const {
  colour.a = alpha;
  return colour;
}
