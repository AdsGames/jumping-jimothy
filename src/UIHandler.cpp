#include "UIHandler.h"
#include "Button.h"

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
bool UIHandler::isHovering(){
  for(int i=0; i<ui_elements.size();i++){
    if(ui_elements[i].hover())
      return true;
  }
  return false;


}

void UIHandler::createButton(int newX, int newY,std::string newText,ALLEGRO_FONT *newFont){
    ui_elements.push_back(Button(newX, newY, newText, newFont));
}
void UIHandler::createAnchoredButton(std::string newText,ALLEGRO_FONT *newFont,std::string newAnchor, bool newJustification){
  Button newButton = Button(0,getElementByText(newAnchor).getY(),newText,newFont);
  if(newJustification)
    newButton.setX(getElementByText(newAnchor).getRightX());
  else
    newButton.setX(getElementByText(newAnchor).getRightX());
  ui_elements.push_back(newButton);


}

UIElement UIHandler::getElementByText(std::string newText){
 for(int i=0; i<ui_elements.size();i++){
    if(ui_elements[i].getText()==newText){

      return ui_elements[i];
    }
  }

  // This will probably crash if the element is not found
  // So be careful
  // Plz dont hurt me
  UIElement failure = UIElement();
  std::cout<<"WARNING: Failed to find UI element "<<newText<<", may be unpredictable\n";
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
