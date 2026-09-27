/**
 * Label
 * Danny Van Stemp and Allan Legemaate
 * UI text label
 * 25/11/2018
 **/

#pragma once

#include "UIElement.h"

class Label : public UIElement {
 public:
  using UIElement::UIElement;

  void draw() override;
};
