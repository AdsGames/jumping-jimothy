#ifndef BUTTON_H
#define BUTTON_H

#include <allegro5/allegro5.h>
#include <string>


class button{
  public:
    button( int x, int y, std::string text);
    virtual ~button();

    // Getters
    int getX(){ return x; }
    int getY(){ return y; }
    int getWidth(){ return width; }
    int getHeight(){ return height; }
    int getPaddingX(){ return padding_x; }
    int getPaddingY(){ return padding_y; }
    std::string getText(){ return text; }

    // Setters
    void setPosition( int x, int y){ this -> x = x; this -> y = y; }
    void setSize( int width, int height){ this -> width = width; this -> height = height;}
    void setPadding( int padding_x, int padding_y){ this -> padding_x = padding_x; this -> padding_y = padding_y; }
    void setText( std::string text){ this -> text = text; }
    void setImage( ALLEGRO_BITMAP *image){ this -> image = image; }

    // Draw
    void draw();
  protected:

  private:
    // Variables
    int x;
    int y;
    int width;
    int height;
    int padding_x;
    int padding_y;

    ALLEGRO_BITMAP *image;

    std::string text;
};

#endif // BUTTON_H
