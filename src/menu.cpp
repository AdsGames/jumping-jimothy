#include "menu.h"

menu::menu(){
  // Title image
  ALLEGRO_BITMAP *title = tools::load_bitmap_ex("images/title.png");

  for( int i = 0; i < 10; i++){
    for( int t = 0; t < 17; t++){
      title_images[i + t*17] = al_create_sub_bitmap( title, i * 256, t * 256, 256, 256);
    }
  }
}

menu::~menu(){
}

void menu::update(){

}

void menu::draw(){
  for( int i = 0; i < 17; i++){
    al_draw_bitmap( title_images[i], 0, 0, 0);
  }
}
