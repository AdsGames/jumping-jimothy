/**
 * UIHandler
 * Danny Van Stemp and Allan Legemaate
 * Owns, updates and draws UI elements, and moves
 *   focus between them with the keyboard or controller
 * 24/09/2017
 **/

#pragma once

#include <memory>
#include <vector>

#include "UIElement.h"

class UIHandler {
 public:
  // Create an element, the handler keeps it for its lifetime
  template <typename T, typename... Args>
  T& add(Args&&... args) {
    auto element = std::make_unique<T>(std::forward<Args>(args)...);
    auto& ref = *element;
    elements.push_back(std::move(element));
    return ref;
  }

  // Create a button to the right of another element
  template <typename T, typename... Args>
  T& addAfter(const UIElement& anchor, Args&&... args) {
    return add<T>(anchor.getX() + anchor.getWidth(), anchor.getY(),
                  std::forward<Args>(args)...);
  }

  void clear();
  void update();
  void draw() const;

  // True if the mouse is over a visible element
  bool isHovering() const;

 private:
  void moveFocus(int direction);

  std::vector<std::unique_ptr<UIElement>> elements;

  // Index of the focused element, -1 for none
  int focused_element{-1};
};
