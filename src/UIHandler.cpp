#include "UIHandler.h"

UIHandler::UIHandler()
{
  //ctor
}

UIHandler::~UIHandler()
{
  //dtor
}

void UIHandler::addElement(UIElement newUIElement){
  ui_elements.push_back(newUIElement);

}

UIElement UIHandler::getElementByText(std::string newText){
 for(int i=0; i<ui_elements.size();i++){
    if(ui_elements[i].getText()==newText);
      return ui_elements[i];
  }

  // This will probably crash if the element is not found
  // So be careful
  // Plz dont hurt me
  UIElement failure = UIElement();
  return failure;
}

void UIHandler::draw(){
  for(int i=0; i<ui_elements.size();i++){
    ui_elements[i].draw();
  }
}

void UIHandler::update(){
  for(int i=0; i<ui_elements.size();i++){
    ui_elements[i].update();
  }
}
