#ifndef BUTTON_H
#define BUTTON_H

#include <UIElement.h>


class Button : public UIElement
{
  public:
    Button( int x, int y, std::string text, ALLEGRO_FONT *button_font, int width = 10, int height = 10, int padding_x = 10, int padding_y = 10);
    Button(){}

    void draw();

   // Button();
    virtual ~Button();

  protected:

  private:

};

#endif // BUTTON_H
