#ifndef BUTTON_H
#define BUTTON_H

#include "UIElement.h"

class Button : public UIElement {
  public:
    Button(int x, int y, std::string text, ALLEGRO_FONT *button_font);
    Button(int x, int y, std::string text, std::string id, ALLEGRO_FONT *button_font);
    Button(int x, int y, std::string text, ALLEGRO_FONT *button_font, int w, int h);
    Button(int x, int y, std::string text, ALLEGRO_BITMAP *newImage,float newRotation);
    Button() {};
    virtual ~Button() {};

    void setWidth(int newWidth);
    void setHeight(int newHeight);

    virtual void draw();
};

#endif // BUTTON_H
