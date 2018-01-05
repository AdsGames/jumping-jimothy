#ifndef BUTTON_H
#define BUTTON_H

#include "UIElement.h"



class Button : public UIElement
{
  public:
    Button( int x, int y, std::string text, ALLEGRO_FONT *button_font);
    Button( int x, int y, std::string text, std::string id, ALLEGRO_FONT *button_font);
    Button( int x, int y, std::string text, ALLEGRO_FONT *button_font, int w, int h);
    Button( int x, int y, std::string text, ALLEGRO_BITMAP *newImage,float newRotation);


    Button(){};

    //void setWidthTo100(){width=100;}

    void setWidth(int newWidth){width=newWidth;}
    void setHeight(int newHeight){height=newHeight;}

    virtual void draw();

   // Button();
    virtual ~Button();

  protected:

  private:

};

#endif // BUTTON_H
