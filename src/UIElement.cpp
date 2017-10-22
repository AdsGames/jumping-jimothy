#include "UIElement.h"

// Defaults
UIElement::UIElement(){
  setDefaults();
}
void UIElement::setDefaults(){

  this -> alpha = 255;
  this -> colour = al_map_rgba(200,200,200,alpha);
  this -> x = 0;
  this -> y = 0;
  this -> width = 0;
  this -> height = 0;
  this -> text = "";
  this -> UIElement_font = nullptr;
  this -> hovering = false;
  this -> old_mouse_down = false;
  this -> mouse_released = false;
  this -> visible = true;
  this -> active = true;
  this -> image = nullptr;
  this -> padding_x = 10;
  this -> padding_y = 10;
  this -> id = "";

}


UIElement::UIElement( int x, int y, std::string text, ALLEGRO_FONT *UIElement_font){

  setDefaults();
  // Literally this
  this -> x = x;
  this -> y = y;
  this -> text = text;
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




// Destruct
UIElement::~UIElement(){
  if( image != nullptr)
    al_destroy_bitmap( image);
}

// Sets an image
void UIElement::setImage( ALLEGRO_BITMAP *image){
  this -> image = image;
  this -> width = al_get_bitmap_width( this -> image);
  this -> height = al_get_bitmap_height( this -> image);
}

// Set new font
void UIElement::setFont( ALLEGRO_FONT *font){
  this -> UIElement_font = font;
  if( UIElement_font != nullptr){
    this -> width = al_get_text_width( UIElement_font, text.c_str());
    this -> height = al_get_font_line_height( UIElement_font);
  }
}
void UIElement::toggleVisibility(){
  visible=!visible;
}


void UIElement::toggleStatus(){
  visible=!visible;
  active=!active;
}

// Update
void UIElement::update(){

  mouse_released = false;
  if(active){
    if( hovering && old_mouse_down && !mouseListener::mouse_button & 1){
      mouse_released = true;

        }
    old_mouse_down = hovering && mouseListener::mouse_button & 1;
    hovering = mouseListener::mouse_x > x && mouseListener::mouse_x < x + getWidth() &&
               mouseListener::mouse_y > y && mouseListener::mouse_y < y + getHeight();
  }else{
    hovering=false;
  }
}

// True if hovering
bool UIElement::hover(){
  return hovering;
}

// True if clicked
bool UIElement::clicked(){
  return hovering && mouseListener::mouse_pressed & 1;
}

bool UIElement::mouseReleased(){
  return mouse_released;
}


void UIElement::draw(){
  if(visible){

    std::cout<<"How do I actually oop?\n";
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
