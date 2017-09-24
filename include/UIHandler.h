#ifndef UIHANDLER_H
#define UIHANDLER_H

#include<vector>
#include<string>
#include "UIElement.h"
class UIHandler
{
  public:
    UIHandler();
    virtual ~UIHandler();
    void addElement(UIElement );
    void draw();
    void update();
    UIElement getElementByText(std::string);
  protected:

  private:
    std::vector<UIElement> ui_elements;
};

#endif // UIHANDLER_H
