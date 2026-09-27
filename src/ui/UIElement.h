/**
 * UI Element
 * Danny Van Stemp and Allan Legemaate
 * Top level UI element
 * 24/09/2017
 **/

#pragma once

#include <asw/asw.h>
#include <string>

enum class TextJustify { Left, Center };

class UIElement {
 public:
  UIElement() = default;
  UIElement(float x, float y, std::string text, asw::Font font);

  virtual ~UIElement() = default;
  UIElement(const UIElement&) = delete;
  UIElement& operator=(const UIElement&) = delete;
  UIElement(UIElement&&) = delete;
  UIElement& operator=(UIElement&&) = delete;

  virtual void update() {}
  virtual void draw() = 0;
  virtual bool canFocus() const { return false; }

  // Position
  float getX() const { return x; }
  float getY() const { return y; }
  void setPosition(float x, float y);

  // Size including padding
  float getWidth() const { return width + (padding_x * 2); }
  float getHeight() const { return height + (padding_y * 2); }

  // Size excluding padding
  void setSize(float width, float height);
  void setPadding(float x, float y);

  // Text
  const std::string& getText() const { return text; }
  void setText(const std::string& text);
  void setTextColour(asw::Color colour) { text_colour = colour; }
  void setTextJustification(TextJustify justify) { justification = justify; }

  // Look
  void setBackgroundColour(asw::Color colour) { background_colour = colour; }
  void setCellFillTransparent(bool transparent) {
    transparent_cell_fill = transparent;
  }
  void setTransparency(uint8_t alpha) { this->alpha = alpha; }
  void setImage(const asw::Texture& image);
  void setImageRotation(float angle) { image_rotation = angle; }

  // Visible elements are also enabled
  bool isVisible() const { return visible; }
  void setVisible(bool visible);
  void show() { setVisible(true); }
  void hide() { setVisible(false); }
  bool isEnabled() const { return !disabled; }

  // Keyboard and controller focus
  void focus() { focused = true; }
  void unfocus() { focused = false; }

  // Mouse over a visible element
  bool hover() const;

  // Clicked with the mouse or selected while focused
  bool clicked() const;

 protected:
  asw::Color withAlpha(asw::Color colour) const;

  float x{0};
  float y{0};
  float width{10};
  float height{10};
  float padding_x{10};
  float padding_y{10};

  asw::Color text_colour{0, 0, 0};
  asw::Color background_colour{200, 200, 200};
  uint8_t alpha{255};
  bool transparent_cell_fill{false};
  bool visible_background{true};
  float border_thickness{2};

  bool visible{true};
  bool disabled{false};
  bool focused{false};

  asw::Texture image;
  float image_rotation{0};

  asw::Font font;
  std::string text;
  TextJustify justification{TextJustify::Left};
};
