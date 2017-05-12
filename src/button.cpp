#include "button.h"

// Construct
button::button( int x, int y, std::string text){
  this -> x = x;
  this -> y = y;
  this -> text = text;

  padding_x = 5;
  padding_y = 5;
}

// Destruct
button::~button(){
  if( image != nullptr)
    al_destroy_bitmap( image);
}

// Draw
void button::draw(){
  al_draw_bitmap( image, x, y, 0);
}
