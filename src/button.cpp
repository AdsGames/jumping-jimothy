#include "button.h"


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
    int new_r=tools::negative_clamp_thing(0,255,(int)((background_colour.r*255)+(40 * hovering)));
    int new_g=tools::negative_clamp_thing(0,255,(int)((background_colour.g*255)+(40 * hovering)));
    int new_b=tools::negative_clamp_thing(0,255,(int)((background_colour.b*255)+(40 * hovering)));

    ALLEGRO_COLOR hover_colour = al_map_rgba(new_r,new_g,new_b,alpha);

    if(disabled_hover_effect)hover_colour=background_colour;

    if(visible_background){
      if(!transparent_cell_fill)al_draw_filled_rectangle( x, y, x + width + padding_x * 2, y + height + padding_y * 2, hover_colour);
      al_draw_rectangle( x, y, x + width + padding_x * 2, y + height + padding_y * 2, al_map_rgba( 0, 0, 0,alpha), outline_thickness);
    }
    // Text

    if( UIElement_font != nullptr){

      if(justification==0){
        al_draw_text( UIElement_font, text_colour, x + padding_x, y + padding_y, 0, text.c_str());
      }
      if(justification==1){

        int text_x;
        int text_y;

        text_x=x+padding_x+width/2;
        text_y=y+padding_y-(tools::get_text_height(UIElement_font,text)-height)/2;

        al_draw_textf(UIElement_font,text_colour,text_x,(text_y-tools::get_text_offset_y(UIElement_font,text)),justification,text.c_str());

      }


    }

    // Image if avail
    if( image != nullptr){
      if(bitmap_rotation_angle==0)
        al_draw_bitmap( image, x + padding_x, y + padding_y, 0);
      else
        al_draw_rotated_bitmap(image,width/2,width/2,x+padding_x+(width/2),y+padding_y+(height/2),bitmap_rotation_angle,0);
    }
  }
}
