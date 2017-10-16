#ifndef UIHANDLER_H
#define UIHANDLER_H

#include<vector>
#include<string>
#include "UIElement.h"
#include "globals.h"

//class UIElement;

class UIHandler
{
  public:
    UIHandler() {};
    virtual ~UIHandler() {};
    void addElement(UIElement *newUIElement);
    void draw();
    void update();
    bool isHovering();
    UIElement getElementByText(std::string);
    void createButton(int x, int y,std::string newText,ALLEGRO_FONT *newFont);
    void createAnchoredButton(std::string,ALLEGRO_FONT *,std::string,bool);
  protected:

  private:
    std::vector<UIElement*> ui_elements;
};

#endif // UIHANDLER_H
