#include "UIHandler.h"
#include "Button.h"

void UIHandler::addElement(UIElement *newUIElement){
  ui_elements.push_back(newUIElement);
}

bool UIHandler::isHovering(){
  for( unsigned int i=0; i<ui_elements.size();i++)
    if(ui_elements.at(i) -> hover())
      return true;
  return false;
}

void UIHandler::createButton(int newX, int newY,std::string newText,ALLEGRO_FONT *newFont){
  Button *newButton = new Button(newX, newY, newText, newFont);
  ui_elements.push_back(newButton);
}

void UIHandler::createAnchoredButton(std::string newText,ALLEGRO_FONT *newFont,std::string newAnchor, bool newJustification){
  Button *newButton = new Button(0,getElementByText(newAnchor) -> getY(),newText,newFont);

  if(newJustification)
    newButton -> setX(getElementByText(newAnchor) -> getRightX());
  else
    newButton -> setX(getElementByText(newAnchor) -> getRightX());

  ui_elements.push_back(newButton);
}

UIElement* UIHandler::getElementByText(std::string newText){
  for( unsigned int i = 0; i < ui_elements.size();i++)
    if( ui_elements.at(i) -> getText() == newText)
      return ui_elements.at(i);

  // This will probably crash if the element is not found
  // So be careful
  // Plz dont hurt me
  //  Edit: I do give ample warning before the tortured aftereffects
  //  Just watch the console
  UIElement* failure = new UIElement();
  std::cout << "WARNING: Failed to find UI element " << newText << ", may(hint:will) be unpredictable\n";
  return failure;
}

void UIHandler::draw(){
  for( unsigned int i=0; i<ui_elements.size();i++)
    ui_elements.at(i) -> draw();
}

void UIHandler::update(){
  for( unsigned int i=0; i<ui_elements.size();i++)
    ui_elements.at(i) -> update();
}
