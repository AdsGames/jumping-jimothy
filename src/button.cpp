#include "Button.h"

Button::Button( int x, int y, std::string text, ALLEGRO_FONT *UIElement_font, int width, int height, int padding_x, int padding_y){

  this -> alpha = 255;
  // Literally this
  this -> x = x;
  this -> y = y;
  this -> text = text;
  this -> image = nullptr;
  this -> UIElement_font = UIElement_font;

  this -> visible = true;
  this -> active = true;

  this -> mouse_released=false;
  this -> old_mouse_down=false;
  this -> hovering = false;

  if( UIElement_font != nullptr){
    this -> width = al_get_text_width( UIElement_font, text.c_str());
    this -> height = al_get_font_line_height( UIElement_font);
  }
  else{
    this -> width = width;
    this -> height = height;
  }

  this -> padding_x = padding_x;
  this -> padding_y = padding_y;
}


Button::~Button()
{
  //dtor
}
void Button::draw(){
 if(visible){
    // Backdrop
    al_draw_filled_rectangle( x, y, x + width + padding_x * 2, y + height + padding_y * 2, al_map_rgba( 200 + 20 * hovering, 200 + 20 * hovering, 200 + 20 * hovering,alpha));
    al_draw_rectangle( x, y, x + width + padding_x * 2, y + height + padding_y * 2, al_map_rgba( 0, 0, 0,alpha), 2);

    // Text
    if( UIElement_font != nullptr)
      al_draw_text( UIElement_font, al_map_rgba( 0, 0, 0,alpha), x + padding_x, y + padding_y, 0, text.c_str());

    // Image if avail
    if( image != nullptr)
      al_draw_bitmap( image, x + padding_x, y + padding_y, 0);
  }
}
