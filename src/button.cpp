#include "Button.h"


Button::Button( int x, int y, std::string text, ALLEGRO_BITMAP *image, float newRotation){

  setDefaults();
  // Naming schemes are frickin' hard
  this -> image = image;
  this -> bitmap_rotation_angle=newRotation;
  // Literally this
  this -> x = x;
  this -> y = y;
  this -> text = text;

  // Oh Allan you can frick right the frick off
  // Just spent an hour debugging why my image was always nullptr
  // this -> image = nullptr;


  this -> UIElement_font = UIElement_font;

  if(image!=nullptr){
    this -> width = al_get_bitmap_width(image);
    this -> height = al_get_bitmap_height(image);
  }else{
    std::cout<<"WARNING: Button " << text << " has been given a nullptr image!\n";

  }

  this -> padding_x = 2;
  this -> padding_y = 2;
}



Button::Button( int x, int y, std::string text, ALLEGRO_FONT *UIElement_font){

  setDefaults();
  this -> bitmap_rotation_angle=0;
  // Literally this
  this -> x = x;
  this -> y = y;
  this -> text = text;
  this -> image = nullptr;
  this -> UIElement_font = UIElement_font;

  if( UIElement_font != nullptr){
    this -> width = al_get_text_width( UIElement_font, text.c_str());
    this -> height = al_get_font_line_height( UIElement_font);
  }
  else{
    this -> width = 10;
    this -> height = 10;
  }

}
Button::Button( int x, int y, std::string text, std::string id, ALLEGRO_FONT *UIElement_font){

  setDefaults();
  this -> bitmap_rotation_angle=0;
  // Literally this
  this -> id = id;
  this -> x = x;
  this -> y = y;
  this -> text = text;
  this -> image = nullptr;
  this -> UIElement_font = UIElement_font;

  if( UIElement_font != nullptr){
    this -> width = al_get_text_width( UIElement_font, text.c_str());
    this -> height = al_get_font_line_height( UIElement_font);
  }
  else{
    this -> width = 10;
    this -> height = 10;
  }

}

Button::Button( int x, int y, std::string text, ALLEGRO_FONT *UIElement_font, int newWidth, int newHeight){

  this -> bitmap_rotation_angle=0;
  this -> alpha = 255;
  setDefaults();
  // Literally this
  this -> x = x;
  this -> y = y;
  this -> text = text;
  this -> image = nullptr;
  this -> UIElement_font = UIElement_font;


  this -> width = newWidth;
  this -> height = newHeight;


}



Button::~Button()
{
  //dtor
}
void Button::draw(){
  if(visible){
    // Backdrop

    // This hover colour has more lines than the whole game loop
    int new_r=tools::clamp(0,255,(int)((colour.r*255)+(20 * hovering)));
    int new_g=tools::clamp(0,255,(int)((colour.g*255)+(20 * hovering)));
    int new_b=tools::clamp(0,255,(int)((colour.b*255)+(20 * hovering)));
    ALLEGRO_COLOR hover_colour = al_map_rgba(new_r,new_g,new_b,alpha);
    al_draw_filled_rectangle( x, y, x + width + padding_x * 2, y + height + padding_y * 2, hover_colour);

    al_draw_rectangle( x, y, x + width + padding_x * 2, y + height + padding_y * 2, al_map_rgba( 0, 0, 0,alpha), 2);

    // Text
    if( UIElement_font != nullptr)
      al_draw_text( UIElement_font, al_map_rgba( 0, 0, 0,alpha), x + padding_x, y + padding_y, 0, text.c_str());

    // Image if avail
    if( image != nullptr){
      if(bitmap_rotation_angle==0)
        al_draw_bitmap( image, x + padding_x, y + padding_y, 0);
      else
        al_draw_rotated_bitmap(image,width/2,width/2,x+padding_x+(width/2),y+padding_y+(height/2),bitmap_rotation_angle,0);
    }
  }
}
