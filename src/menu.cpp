#include "menu.h"

menu::menu(){
  // Title image
  ALLEGRO_BITMAP *title = tools::load_bitmap_ex("images/title2.png");

  for( int i = 0; i < 10; i++){
    for( int t = 0; t < 17; t++){
      title_images[i + t*10] = al_create_sub_bitmap( title, i * 256, t * 256, 256, 256);
    }
  }

  ALLEGRO_BITMAP *play = tools::load_bitmap_ex("images/playbutton.png");

  for( int i = 0; i < 50; i++){
    play_images[i] = al_create_sub_bitmap( play, i * 64, 0, 64, 32);
  }

  ALLEGRO_BITMAP *pressthing = tools::load_bitmap_ex("images/pressthing.png");

  pressthing_images[0] = al_create_sub_bitmap( pressthing, 0, 0, 128, 16);
  pressthing_images[1] = al_create_sub_bitmap( pressthing, 128, 0, 128, 16);

  counter_title = 0;
  counter_play = 0;
}

menu::~menu(){

}

void menu::update(){
  counter_title ++;
  counter_play ++;
  if( counter_title >= 170){
    counter_title = 0;
  }
  if( counter_play >= 50){
    counter_play = 0;
    counter_pressthing=!counter_pressthing;
  }

  if( keyListener::anyKeyPressed || joystickListener::anyButtonPressed){
    set_next_state(STATE_GAME);
  }
}

void menu::draw(){
  // Background
  al_clear_to_color( al_map_rgb(200,200,255));
  al_draw_scaled_bitmap( title_images[counter_title], 0, 0, 256, 256, 0, 0, 1024, 768, 0);
  al_draw_scaled_bitmap( play_images[counter_play], 0, 0, 64, 32, 300, 565, 256, 128, 0);
  al_draw_scaled_bitmap( pressthing_images[counter_pressthing], 0, 0, 128, 16, 290, 710, 256, 32, 0);
}
