#ifndef BUTTON_H
#define BUTTON_H

#include "ui/UIElement.h"

class Button : public UIElement {
  public:
    Button();
    Button(int x, int y, std::string text, std::string id, ALLEGRO_FONT *font);
    virtual ~Button() {};

    void draw() override;
    void update() override;
};

#endif // BUTTON_H
