#include "UIHandler.h"

#include "../util/Config.h"

#include "Button.h"
#include "Label.h"

#include "../util/Globals.h"
#include "../util/Tools.h"

#include "../util/ActionBinder.h"
#include "../util/MouseListener.h"

// Create UI handler
UIHandler::UIHandler() {
  // Load cursor image
  ui_cursor = tools::load_bitmap_ex("assets/images/cursor.png");
}

// Add element to handler
void UIHandler::addElement(std::shared_ptr<UIElement> elem) {
  ui_elements.emplace_back(elem);
}

// Is hovering over ui element
bool UIHandler::isHovering() {
  // Check global hover status
  for (unsigned int i = 0; i < ui_elements.size(); i++) {
    if (ui_elements.at(i)->hover()) {
      return true;
    }
  }
  return false;
}

// Get all UI elements
std::vector<std::shared_ptr<UIElement>> UIHandler::getUIElements() {
  return ui_elements;
}

// Create button
void UIHandler::createButton(const int x,
                             const int y,
                             std::string text,
                             std::string id,
                             ALLEGRO_FONT* font) {
  ui_elements.emplace_back(new Button(x, y, text, id, font));
}

// Create anchored button
void UIHandler::createAnchoredButton(std::string text,
                                     ALLEGRO_FONT* font,
                                     std::string anchorID,
                                     std::string id) {
  ui_elements.emplace_back(new Button(
      getElementById(anchorID)->getX() + getElementById(anchorID)->getWidth(),
      getElementById(anchorID)->getY(), text, id, font));
}

// Search for elemnt by ID
UIElement* UIHandler::getElementById(std::string id) {
  // Find element
  for (auto& elem : ui_elements) {
    if (elem->getId() == id) {
      return elem.get();
    }
  }
  tools::log_message("Warning: not found " + id);

  // Not found
  return nullptr;
}

// Draw UIElement to screen
void UIHandler::draw() {
  // Draw all elements
  for (auto& elem : ui_elements) {
    elem->draw();
  }

  // Draw cursor if required
  if (Config::getBooleanValue("draw_cursor") && ui_cursor) {
    al_draw_bitmap(ui_cursor, MouseListener::mouse_x, MouseListener::mouse_y,
                   0);
  }
}

// Update UIElement logic
void UIHandler::update() {
  // Update all elements
  for (const auto& elem : ui_elements) {
    elem->update();
  }

  // Move between elements
  if (!ui_elements.empty()) {
    // Key pressed
    if (ActionBinder::actionBegun(Action::UP) ||
        ActionBinder::actionBegun(Action::DOWN)) {
      // Unfocus current
      if (focusedElement >= 0 && focusedElement < (signed)ui_elements.size())
        ui_elements.at(focusedElement)->unfocus();

      // Focus direction -1 down, 0 none, 1 up
      int focusDirection = 0;

      // Choose direction
      if (ActionBinder::actionBegun(Action::UP)) {
        focusDirection = -1;
      } else if (ActionBinder::actionBegun(Action::DOWN)) {
        focusDirection = 1;
      }

      // Find next focusable
      for (const auto& _ : ui_elements) {
        // Increment focused element
        focusedElement += focusDirection;

        // Keep in bounds
        if (focusedElement >= (signed)ui_elements.size()) {
          focusedElement = 0;
        } else if (focusedElement < 0) {
          focusedElement = (signed)ui_elements.size() - 1;
        }

        // Check if focusable
        if (ui_elements.at(focusedElement)->canFocus() &&
            ui_elements.at(focusedElement)->isEnabled() &&
            ui_elements.at(focusedElement)->isVisible()) {
          ui_elements.at(focusedElement)->focus();
          break;
        }
      }
    }

    // Unfocus if mouse moved
    if (MouseListener::mouse_moved) {
      if (focusedElement >= 0 && focusedElement < (signed)ui_elements.size()) {
        ui_elements.at(focusedElement)->unfocus();
      }
      focusedElement = -1;
    }
  }
}
