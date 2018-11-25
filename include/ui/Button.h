#ifndef BUTTON_H
#define BUTTON_H

#include "ui/UIElement.h"

class Button : public UIElement {
  public:
    Button();
    Button(int x, int y, std::string text, std::string id, ALLEGRO_FONT *button_font);
    virtual ~Button() {};

    void setWidth(int newWidth);
    void setHeight(int newHeight);

    virtual void draw();

};

#endif // BUTTON_H
