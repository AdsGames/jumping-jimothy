#include "UIHandler.h"

#include <algorithm>

#include "../util/Controls.h"

namespace {
bool mouseMoved() {
  const auto& change = asw::input::get_mouse().change;
  return change.x != 0.0F || change.y != 0.0F;
}
}  // namespace

void UIHandler::clear() {
  elements.clear();
  focused_element = -1;
}

void UIHandler::update() {
  UIElement::resetClick();

  for (const auto& element : elements) {
    element->update();
  }

  if (elements.empty()) {
    return;
  }

  if (asw::input::get_action_down(Controls::UP)) {
    moveFocus(-1);
  } else if (asw::input::get_action_down(Controls::DOWN)) {
    moveFocus(1);
  }

  // The mouse takes over from the keyboard
  if (mouseMoved() && focused_element >= 0) {
    elements[focused_element]->unfocus();
    focused_element = -1;
  }
}

void UIHandler::moveFocus(int direction) {
  const auto count = static_cast<int>(elements.size());

  if (focused_element >= 0) {
    elements[focused_element]->unfocus();
  } else {
    // Nothing focused, start before the first or after the last element
    focused_element = direction > 0 ? -1 : count;
  }

  // Step to the next element that can take focus, wrapping around
  for (int i = 0; i < count; i++) {
    focused_element = (focused_element + direction + count) % count;

    const auto& element = elements[focused_element];
    if (element->canFocus() && element->isEnabled() && element->isVisible()) {
      element->focus();
      return;
    }
  }

  focused_element = -1;
}

void UIHandler::draw() const {
  for (const auto& element : elements) {
    element->draw();
  }
}

bool UIHandler::isHovering() const {
  return std::ranges::any_of(
      elements, [](const auto& element) { return element->hover(); });
}
