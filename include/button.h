#ifndef BUTTON_H
#define BUTTON_H

#include "UIElement.h"



class Button : public UIElement
{
  public:
    Button( int x, int y, std::string text, ALLEGRO_FONT *button_font);
    Button( int x, int y, std::string text, ALLEGRO_FONT *button_font, int w, int h);

    Button(){};

    virtual void draw();

   // Button();
    virtual ~Button();

  protected:

  private:

};

#endif // BUTTON_H
