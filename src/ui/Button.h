/**
 * Button
 * Danny Van Stemp and Allan Legemaate
 * UI Button
 * 11/04/2017
 **/

#pragma once

#include "UIElement.h"

class Button : public UIElement {
 public:
  using UIElement::UIElement;

  void draw() override;
  bool canFocus() const override { return true; }
};
