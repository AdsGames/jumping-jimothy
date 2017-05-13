#include "button.h"


// Defaults
button::button(){
  this -> x = 0;
  this -> y = 0;
  this -> text = "";
  this -> image = nullptr;
  this -> button_font = nullptr;
  this -> hovering = false;

  padding_x = 10;
  padding_y = 10;
}

// Construct
button::button( int x, int y, std::string text, ALLEGRO_FONT *button_font){
  this -> x = x;
  this -> y = y;
  this -> text = text;
  this -> image = nullptr;
  this -> button_font = button_font;

  if( button_font != nullptr){
    this -> width = al_get_text_width( button_font, text.c_str());
    this -> height = al_get_font_line_height( button_font);
  }
  else{
    this -> width = 50;
    this -> height = 10;
  }

  padding_x = 10;
  padding_y = 10;
}

// Destruct
button::~button(){
  if( image != nullptr)
    al_destroy_bitmap( image);
}

// Set new font
void button::setFont( ALLEGRO_FONT *font){
  this -> button_font = font;
  if( button_font != nullptr){
    this -> width = al_get_text_width( button_font, text.c_str());
    this -> height = al_get_font_line_height( button_font);
  }
}

// Update
void button::update(){
  hovering = mouseListener::mouse_x > x && mouseListener::mouse_x < x + getWidth() &&
             mouseListener::mouse_y > y && mouseListener::mouse_y < y + getHeight();
}

// True if hovering
bool button::hover(){
  return hovering;
}

// True if clicked
bool button::clicked(){
  return hovering && mouseListener::mouse_pressed & 1;
}

// Draw
void button::draw(){
  // Backdrop
  al_draw_filled_rectangle( x, y, x + width + padding_x * 2, y + height + padding_y * 2, al_map_rgb( 200 + 20 * hovering, 200 + 20 * hovering, 200 + 20 * hovering));
  al_draw_rectangle( x, y, x + width + padding_x * 2, y + height + padding_y * 2, al_map_rgb( 0, 0, 0), 2);

  // Text
  if( button_font != nullptr)
    al_draw_text( button_font, al_map_rgb( 0, 0, 0), x + padding_x, y + padding_y, 0, text.c_str());

  // Image if avail
  if( image != nullptr)
    al_draw_bitmap( image, x, y, 0);
}
