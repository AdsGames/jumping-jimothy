#include "menu.h"

menu::menu(){
  // Title image
  ALLEGRO_BITMAP *title = tools::load_bitmap_ex("images/title.png");

  for( int i = 0; i < 10; i++){
    for( int t = 0; t < 17; t++){
      title_images[i + t*10] = al_create_sub_bitmap( title, i * 256, t * 256, 256, 256);
    }
  }

  counter = 0;
}

menu::~menu(){
}

void menu::update(){
  counter ++;
  if( counter >= 170){
    counter = 0;
  }
}

void menu::draw(){
  // Background
  al_clear_to_color( al_map_rgb(200,200,255));
  al_draw_scaled_bitmap( title_images[counter], 0, 0, 256, 256, 0, 0, 1024, 768, 0);
}
