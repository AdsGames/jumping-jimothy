/**
 * Check Box
 * Danny Van Stemp and Allan Legemaate
 * Check Box
 * 16/10/2017
 **/

#ifndef CHECKBOX_H
#define CHECKBOX_H

#include "UIElement.h"

class CheckBox : public UIElement {
 public:
  // Default constructor
  CheckBox();

  // Detailed constructor
  CheckBox(const int x,
           const int y,
           std::string text,
           std::string id,
           ALLEGRO_FONT* font);

  // Dtor
  virtual ~CheckBox(){};

  // Update
  virtual void update() override;

  // Draw
  virtual void draw() override;

  // Check
  bool getChecked();

  // Set checked
  void setChecked(const bool checked);

  // Get toggled
  bool getToggled();

  // Enable focus
  virtual bool canFocus() override;

 private:
  // Width of checkbox
  int checkbox_size;

  // Checked
  bool checked;

  // Just toggled
  bool toggled;
};

#endif  // CHECKBOX_H
