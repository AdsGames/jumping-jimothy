#include "ui/UIHandler.h"

#include "ui/Button.h"
#include "ui/Label.h"

#include "util/Globals.h"
#include "util/Tools.h"

// Add element to handler
void UIHandler::addElement(UIElement *elem){
  ui_elements.push_back(elem);
}

// Is hovering over ui element
bool UIHandler::isHovering(){
  // Check global hover status
  for (unsigned int i = 0; i < ui_elements.size(); i++) {
    if (ui_elements.at(i) -> hover()) {
      return true;
    }
  }
  return false;
}

// Get all UI elements
std::vector<UIElement*> UIHandler::getUIElements() {
  return ui_elements;
}

// Create button
void UIHandler::createButton(const int x, const int y, std::string text, std::string id, ALLEGRO_FONT *font){
  ui_elements.push_back(new Button(x, y, text, id, font));
}

// Create anchored button
void UIHandler::createAnchoredButton(std::string text, ALLEGRO_FONT *font, std::string anchorID, std::string id, const bool justification){
  ui_elements.push_back(new Button(getElementById(anchorID) -> getX() + getElementById(anchorID) -> getWidth(), getElementById(anchorID) -> getY(), text, id, font));
}

// Search for element by text
UIElement* UIHandler::getElementByText(std::string text) {
  // Find element
  for (unsigned int i = 0; i < ui_elements.size(); i++) {
    if (ui_elements.at(i) -> getText() == text) {
      return ui_elements.at(i);
    }
  }
  tools::log_message("Warning: not found " + text);

  // Not found
  return nullptr;
}

// Search for elemnt by ID
UIElement* UIHandler::getElementById(std::string id) {
  // Find element
  for (unsigned int i = 0; i < ui_elements.size(); i++) {
    if (ui_elements.at(i) -> getId() == id) {
      return ui_elements.at(i);
    }
  }
  tools::log_message("Warning: not found " + id);

  // Not found
  return nullptr;
}

// Draw UIElement to screen
void UIHandler::draw(){
  // Draw all elements
  for (unsigned int i = 0; i < ui_elements.size(); i++) {
    ui_elements.at(i) -> draw();
  }
}

// Update UIElement logic
void UIHandler::update(){
  // Update all elements
  for (unsigned int i = 0; i < ui_elements.size(); i++) {
    ui_elements.at(i) -> update();
  }
}
