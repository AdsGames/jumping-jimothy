#include "CheckBox.h"

CheckBox::CheckBox()
{
  //ctor
}
CheckBox::CheckBox(int newX, int newY,std::string newText, ALLEGRO_FONT *newFont){
  this -> alpha = 255;

  this -> checkbox_width = 20;

  this -> image = nullptr;
  this -> UIElement_font = newFont;


  this -> x = newX;
  this -> y = newY;

  this -> text = newText;

  this -> UIElement_font = UIElement_font;

  this -> visible = true;
  this -> active = true;

  this -> mouse_released=false;
  this -> old_mouse_down=false;
  this -> hovering = false;

  this -> status = false;


  if( UIElement_font != nullptr){
    this -> width = al_get_text_width( UIElement_font, text.c_str()) + checkbox_width;
    this -> height = al_get_font_line_height( UIElement_font);
  }
  else{
    this -> width = 10;
    this -> height = 10;
  }


}

CheckBox::~CheckBox()
{
  //dtor
}

void CheckBox::draw(){
 if(visible){
    // Backdrop
    al_draw_filled_rectangle( x, y, x + width + padding_x * 2, y + height + padding_y * 2, al_map_rgba( 200 + 20 * hovering, 200 + 20 * hovering, 200 + 20 * hovering,alpha));
    al_draw_rectangle( x, y, x + width + padding_x * 2, y + height + padding_y * 2, al_map_rgba( 0, 0, 0,alpha), 2);

    // Checkbox
    al_draw_filled_rectangle( x+padding_x, y+padding_y, x + padding_x +checkbox_width, y + padding_y + checkbox_width, al_map_rgba( 200 + 20 * hovering, 200 + 20 * hovering, 200 + 20 * hovering,alpha));
    al_draw_rectangle( x + padding_x, y + padding_y, x + padding_x+checkbox_width, y + padding_y + checkbox_width, al_map_rgba( 0, 0, 0,alpha), 2);

    // Text
    if( UIElement_font != nullptr)
      al_draw_text( UIElement_font, al_map_rgba( 0, 0, 0,alpha), x + padding_x+checkbox_width, y + padding_y, 0, text.c_str());


  }
}
