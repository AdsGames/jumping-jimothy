/**
 * Check Box
 * Danny Van Stemp and Allan Legemaate
 * Check Box
 * 16/10/2017
 **/

#pragma once

#include "UIElement.h"

class CheckBox : public UIElement {
 public:
  CheckBox(float x, float y, std::string text, asw::Font font);

  void update() override;
  void draw() override;
  bool canFocus() const override { return true; }

  bool getChecked() const { return checked; }
  void setChecked(bool checked) { this->checked = checked; }

  // True on the tick the box was toggled
  bool getToggled() const { return toggled; }

 private:
  float checkbox_size{20};
  bool checked{false};
  bool toggled{false};
};
