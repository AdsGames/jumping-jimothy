#include "CheckBox.h"

#include "MouseListener.h"

CheckBox::CheckBox()
  : UIElement()  {
}

CheckBox::CheckBox(int newX, int newY,std::string newText, std::string id, ALLEGRO_FONT *newFont)
  : UIElement(newX, newY, newText, id, newFont){

  this -> checkbox_width = 20;
  this -> checked = false;

  if( UIElement_font != nullptr){
    this -> width = al_get_text_width( UIElement_font, text.c_str()) + checkbox_width+padding_x;
    this -> height = al_get_font_line_height( UIElement_font);
  }
  else{
    this -> width = 10;
    this -> height = 10;
  }
}

void CheckBox::update(){
  if(!disabled && clicked()){
    checked =! checked;
  }
}

void CheckBox::draw() {
  if(visible) {
    // Backdrop
    al_draw_filled_rectangle( x, y, x + width + padding_x * 2, y + height + padding_y * 2, al_map_rgba( 200 + 20 * hover(), 200 + 20 * hover(), 200 + 20 * hover(),alpha));
    al_draw_rectangle( x, y, x + width + padding_x * 2, y + height + padding_y * 2, al_map_rgba( 0, 0, 0,alpha), 2);

    // Checkbox
    al_draw_filled_rectangle( x+padding_x, y+padding_y, x + padding_x +checkbox_width, y + padding_y + checkbox_width, al_map_rgba( 200 + 20 * hover(), 200 + 20 * hover(), 200 + 20 * hover(),alpha));
    al_draw_rectangle( x + padding_x, y + padding_y, x + padding_x+checkbox_width, y + padding_y + checkbox_width, al_map_rgba( 0, 0, 0,alpha), 2);

    if(checked)
      al_draw_filled_rectangle( x+padding_x+2, y+padding_y+2, x + padding_x +checkbox_width-2, y + padding_y + checkbox_width-2, al_map_rgba( 0,0,0,alpha));

    // Text
    if( UIElement_font != nullptr)
      al_draw_text( UIElement_font, al_map_rgba( 0, 0, 0,alpha), x + padding_x+checkbox_width+padding_x, y + padding_y, 0, text.c_str());
  }
}

bool CheckBox::getChecked() {
  return checked;
}
