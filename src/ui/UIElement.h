/**
 * UI Element
 * Danny Van Stemp and Allan Legemaate
 * Top level UI element
 * 24/09/2017
 **/

#ifndef UIELEMENT_H
#define UIELEMENT_H

#include <allegro5/allegro_font.h>

#include <string>

class UIElement {
 public:
  // Constructor
  UIElement();
  UIElement(const int x,
            const int y,
            std::string text,
            std::string id,
            ALLEGRO_FONT* font);

  // Dtor
  virtual ~UIElement();

  // Get X
  int getX() const;

  // Get y
  int getY() const;

  // Get text of element
  std::string getText() const;

  // Get element ID
  std::string getId() const;

  // Hide element
  void hide();

  // Show element
  void show();

  // Is visible or not
  bool isVisible() const;

  // Set visibility
  void setVisibility(bool visible);

  // Disable element
  void disable();

  // Enable element
  void enable();

  // Is enabled or not
  bool isEnabled() const;

  // Set transparency level
  void setTransparency(const float alpha);

  // Set image rotation
  void setBitmapRotationAngle(const float rotation);

  // Set x
  void setX(const int x);

  // Set y
  void setY(const int y);

  // Set text colour
  void setTextColour(ALLEGRO_COLOR colour);

  // Set background colour
  void setBackgroundColour(ALLEGRO_COLOR colour);

  // Set cell fill
  void setCellFillTransparent(const bool n);

  // Set text justification
  void setTextJustification(const int justification);

  // Element width
  int getWidth() const;

  // Element height
  int getHeight() const;

  // Set padding
  void setPadding(const int x, const int y);

  // Set position
  void setPosition(const int x, const int y);

  // Set element size
  void setSize(const int width, const int height);

  // Set text
  void setText(std::string text);

  // Set id
  void setId(std::string id);

  // Set image
  void setImage(ALLEGRO_BITMAP* image);

  // Set font
  void setFont(ALLEGRO_FONT* font);

  // Set background visibility
  void setVisibleBackground(const bool b);

  // Set width
  void setWidth(const int width);

  // Set height
  void setHeight(const int height);

  // Can focus
  virtual bool canFocus();

  // Focus
  void focus();

  // Unfocus
  void unfocus();

  // Set border thickness
  void setBorderThickness(const int thickness);

  // Disable hover effect
  void disableHoverEffect();

  // Disable hover effect
  void enableHoverEffect();

  // Mouse over
  bool hover();

  // Clicked
  bool clicked();

  // Update element
  virtual void update() = 0;

  // Draw element
  virtual void draw() = 0;

  // Set callbacks
  void setOnClick(void* func);
  void setOnHover(void* func);
  void setOnFocus(void* func);

 protected:
  // Position
  int x{0};
  int y{0};

  // Dimensions of element
  int width{10};
  int height{10};

  // Padding
  int padding_x{10};
  int padding_y{10};

  // Text colour
  ALLEGRO_COLOR text_colour;

  // Background colour
  ALLEGRO_COLOR background_colour;

  // Fill with transparency
  bool transparent_cell_fill{false};

  // Primitive alpha
  float alpha{255};

  // Background visibility
  bool visible_background{true};

  // Thickness of border
  int border_thickness{2};

  // Visibility status
  bool visible{true};

  // Disabled status
  bool disabled{false};

  // Hover effect enabled
  bool hover_effect{true};

  // Image rotation
  float bitmap_rotation_angle{0};

  // Optional image
  ALLEGRO_BITMAP* image{nullptr};

  // Font
  ALLEGRO_FONT* UIElement_font{nullptr};

  // Text of element
  std::string text{""};

  // Justification of text
  int justification{0};

  // Unique id of element
  std::string id{""};

  // Focused
  bool focused{false};

  // Function pointers
  // On hover
  void* onHover{nullptr};

  // On focus
  void* onFocus{nullptr};

  // On click
  void* onClick{nullptr};
};

#endif  // UIELEMENT_H
