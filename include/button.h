#ifndef BUTTON_H
#define BUTTON_H

#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <string>

#include <mouseListener.h>


class button{
  public:
    button( int x, int y, std::string text, ALLEGRO_FONT *button_font);
    button();
    ~button();

    // Getters
    int getX(){ return x; }
    int getY(){ return y; }
    int getWidth(){ return width + padding_x; }
    int getHeight(){ return height + padding_y; }
    int getPaddingX(){ return padding_x; }
    int getPaddingY(){ return padding_y; }
    std::string getText(){ return text; }

    // Setters
    void setPosition( int x, int y){ this -> x = x; this -> y = y; }
    void setSize( int width, int height){ this -> width = width; this -> height = height;}
    void setPadding( int padding_x, int padding_y){ this -> padding_x = padding_x; this -> padding_y = padding_y; }
    void setText( std::string text){ this -> text = text; }
    void setImage( ALLEGRO_BITMAP *image){ this -> image = image; }
    void setFont( ALLEGRO_FONT *font);

    bool hover();
    bool clicked();
    void update();

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

    bool hovering;

    ALLEGRO_BITMAP *image;

    // Font
    ALLEGRO_FONT *button_font;

    std::string text;
};

#endif // BUTTON_H
