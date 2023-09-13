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

  // Update
  void update() override;

  // Draw
  void draw() override;

  // Check
  bool getChecked() const;

  // Set checked
  void setChecked(const bool checked);

  // Get toggled
  bool getToggled() const;

  // Enable focus
  bool canFocus() override;

 private:
  // Width of checkbox
  int checkbox_size{20};

  // Checked
  bool checked{false};

  // Just toggled
  bool toggled{false};
};

#endif  // CHECKBOX_H
