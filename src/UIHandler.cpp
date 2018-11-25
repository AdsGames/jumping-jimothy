#include "UIHandler.h"

#include "Button.h"
#include "Globals.h"
#include "Tools.h"

// Add element to handler
void UIHandler::addElement(UIElement *elem){
  ui_elements.push_back(elem);
}

// Is hovering over ui element
bool UIHandler::isHovering(){
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
void UIHandler::createButton(int newX, int newY, std::string newText, std::string id, ALLEGRO_FONT *newFont){
  Button *newButton = new Button(newX, newY, newText, id, newFont);
  ui_elements.push_back(newButton);
}

// Create anchored button
void UIHandler::createAnchoredButton(std::string newText, ALLEGRO_FONT *newFont, std::string newAnchor, std::string newId, bool newJustification){
  Button *newButton = new Button(0, getElementByText(newAnchor) -> getY(), newText, newId, newFont);

  if (newJustification)
    newButton -> setX(getElementByText(newAnchor) -> getX() + getElementByText(newAnchor) -> getWidth());
  else
    newButton -> setX(getElementByText(newAnchor) -> getX() + getElementByText(newAnchor) -> getWidth());

  ui_elements.push_back(newButton);
}

// Search for element by text
UIElement* UIHandler::getElementByText(std::string text) {
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
  for (unsigned int i = 0; i < ui_elements.size(); i++) {
    ui_elements.at(i) -> draw();
  }
}

// Update UIElement logic
void UIHandler::update(){
  for (unsigned int i = 0; i < ui_elements.size(); i++) {
    ui_elements.at(i) -> update();
  }
}
