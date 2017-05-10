#include "menu.h"

// Init menu
menu::menu(){
  // Title image
  title = tools::load_bitmap_ex("images/title.png");

  // Slice up title images
  for( int i = 0; i < 10; i++)
    for( int t = 0; t < 17; t++)
      title_images[i + t * 10] = al_create_sub_bitmap( title, i * 256, t * 256, 256, 256);

  // Load play button
  play = tools::load_bitmap_ex("images/playbutton.png");

  // Slice up play image
  for( int i = 0; i < 50; i++)
    play_images[i] = al_create_sub_bitmap( play, i * 64, 0, 64, 32);

  // Load prompt
  prompt_image = tools::load_bitmap_ex("images/prompt.png");

  // Init counters
  counter_title = 0;
  counter_play = 0;
}

// Destory menu
menu::~menu(){
  // Delete images
  for( int i = 0; i < 50; i++)
    al_destroy_bitmap( play_images[i]);

  for( int i = 0; i < 10; i++)
    for( int t = 0; t < 17; t++)
      al_destroy_bitmap( title_images[i + t * 10]);

  al_destroy_bitmap( prompt_image);
  al_destroy_bitmap( play);
  al_destroy_bitmap( title);
}

// Update animation and wait for input
void menu::update(){
  // Add to counters
  counter_title ++;
  counter_play ++;

  // Animation roll arounds
  counter_title = (counter_title >= 170) ? 0 : counter_title;
  counter_prompt = (counter_play >= 50) ? !counter_prompt : counter_prompt;
  counter_play = (counter_play >= 50) ? 0 : counter_play;

  // Go to game
  if( keyListener::anyKeyPressed || joystickListener::anyButtonPressed)
    set_next_state(STATE_GAME);
}

// Draw images to screen
void menu::draw(){
  // Background
  al_clear_to_color( al_map_rgb(200,200,255));

  // Title
  al_draw_scaled_bitmap( title_images[counter_title], 0, 0, 256, 256, 0, 0, 1024, 768, 0);

  // Play prompt
  al_draw_scaled_bitmap( play_images[counter_play], 0, 0, 64, 32, 300, 565, 256, 128, 0);

  if( counter_prompt)
    al_draw_scaled_bitmap( prompt_image, 0, 0, 128, 16, 290, 710, 256, 32, 0);
}
