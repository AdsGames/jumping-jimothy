#include "menu.h"

// Init menu

//ALLEGRO_SAMPLE *menu::menu_music = nullptr;

menu::menu(){

  credits_menu=false;

  menu_font = al_load_ttf_font( "fonts/munro.ttf", 18, 0);
  button_font = al_load_ttf_font( "fonts/munro.ttf", 24, 0);

  credits_font = al_load_ttf_font( "fonts/munro.ttf", 32, 0);
  cursor = tools::load_bitmap_ex("images/cursor.png");


    // Title image
  button_edit = tools::load_bitmap_ex("images/button_edit.png");
  button_help = tools::load_bitmap_ex("images/button_help.png");
  button_exit = tools::load_bitmap_ex("images/button_exit.png");
  button_options = tools::load_bitmap_ex("images/gear.png");
  highlight = tools::load_bitmap_ex("images/highlight.png");



  int button_offset_x = 40;


  menu_buttons[menu_button_edit] = Button( button_offset_x, 550, "Level Editor", button_font, 200, 20);
  menu_buttons[menu_button_edit].setVisibleBackground(false);
  menu_buttons[menu_button_edit].setTextColour(al_map_rgb(255,255,255));


  menu_buttons[menu_button_exit] = Button( button_offset_x, 700, "Exit", button_font, 200, 20);
  menu_buttons[menu_button_exit].setVisibleBackground(false);
  menu_buttons[menu_button_exit].setTextColour(al_map_rgb(255,255,255));




  menu_buttons[menu_button_play] = Button( button_offset_x, 500, "Play",button_font, 200, 20);
  menu_buttons[menu_button_play].setVisibleBackground(false);
  menu_buttons[menu_button_play].setTextColour(al_map_rgb(255,255,255));



  menu_buttons[menu_button_help] = Button( button_offset_x, 650, "Credits", button_font, 200, 20);
  menu_buttons[menu_button_help].setVisibleBackground(false);
  menu_buttons[menu_button_help].setTextColour(al_map_rgb(255,255,255));

  menu_buttons[menu_button_options] = Button( button_offset_x, 600, "Settings", button_font,200,20 );
  menu_buttons[menu_button_options].setVisibleBackground(false);
  menu_buttons[menu_button_options].setTextColour(al_map_rgb(255,255,255));



  title = tools::load_bitmap_ex("images/title_static.png");
  title_overlay = tools::load_bitmap_ex("images/title_overlay.png");
  title_shine = tools::load_bitmap_ex("images/title_shine.png");
  logo = tools::load_bitmap_ex("images/logo.png");



  playbutton_frame = tools::load_bitmap_ex("images/playbutton_frame.png");
  playbutton_frame_hover = tools::load_bitmap_ex("images/playbutton_frame_hover.png");



  #if defined(RELEASE)

    if(!MusicManager::menu_music.getIsPlaying())
      MusicManager::menu_music.play();



  #endif



  // Slice up title images
 // for( int i = 0; i < 10; i++)
  //  for( int t = 0; t < 17; t++)
  //    title_images[i + t * 10] = al_create_sub_bitmap( title, i * 256, t * 256, 256, 256);

  // Load play button
  play = tools::load_bitmap_ex("images/playbutton.png");

  // Slice up play image
  for( int i = 0; i < 50; i++)
    play_images[i] = al_create_sub_bitmap( play, i * 64, 0, 64, 32);

  // Load prompt
  prompt_image = tools::load_bitmap_ex("images/prompt.png");

  // Init counters
  counter_title = 251;
  counter_play = 0;
}

// Destory menu
menu::~menu(){
  // Delete images
  for( int i = 0; i < 50; i++)
    al_destroy_bitmap( play_images[i]);

  //for( int i = 0; i < 10; i++)
    //for( int t = 0; t < 17; t++)
      //al_destroy_bitmap( title_images[i + t * 10]);

  //al_destroy_bitmap( prompt_image);
  al_destroy_bitmap( play);
  al_destroy_bitmap( title);


}

// Update animation and wait for input
void menu::update(){


  if(!credits_menu){
     // Update buttons
    for( int i = 0; i < BUTTON_COUNT; i++){
      menu_buttons[i].update();
    }

    if(menu_buttons[menu_button_play].mouseReleased() || (joystickListener::buttonReleased[JOY_XBOX_A] && (highlight_y_destination==500))){
      set_next_state(STATE_LEVELSELECT);
    }

    if(menu_buttons[menu_button_edit].mouseReleased() || (joystickListener::buttonReleased[JOY_XBOX_A] && (highlight_y_destination==550))){
      set_next_state(STATE_EDIT);
    }

    if(menu_buttons[menu_button_exit].mouseReleased() || (joystickListener::buttonReleased[JOY_XBOX_A] && (highlight_y_destination==700))){
      set_next_state(STATE_EXIT);
    }

    if(menu_buttons[menu_button_options].mouseReleased() || (joystickListener::buttonReleased[JOY_XBOX_A] && (highlight_y_destination==600))){
      set_next_state(STATE_OPTIONS);
    }

    if(menu_buttons[menu_button_help].mouseReleased() || (joystickListener::buttonReleased[JOY_XBOX_A] && (highlight_y_destination==650))){
      joystickListener::anyButtonReleased=false;

      credits_menu=true;
    }
  }
  if(keyListener::anyKeyPressed || joystickListener::anyButtonReleased){
    credits_menu = false;
    joystickListener::anyButtonReleased=false;
  }



  if((joystickListener::stickDirections[LEFT_STICK_UP] || joystickListener::stickDirections[DPAD_UP2]) && !joystick_direction_hit){
    if(highlight_y_destination<700)
      highlight_y_destination+=50;
  }

  if((joystickListener::stickDirections[LEFT_STICK_DOWN] || joystickListener::stickDirections[DPAD_DOWN])  && !joystick_direction_hit){
    if(highlight_y_destination>500)
      highlight_y_destination-=50;
  }

  if(joystickListener::stickDirections[LEFT_STICK_DOWN] || joystickListener::stickDirections[LEFT_STICK_UP] || joystickListener::stickDirections[DPAD_DOWN] || joystickListener::stickDirections[DPAD_UP2]){
    joystick_direction_hit=true;
    Options::joystick_mode=true;
  }else{
    joystick_direction_hit=false;
  }
  if(mouseListener::mouse_moved)
    Options::joystick_mode=false;


  if(highlight_y>highlight_y_destination)highlight_y-=10;
  if(highlight_y<highlight_y_destination)highlight_y+=10;


  // Add to counters
  counter_title ++;
  counter_play ++;

  // Animation roll arounds
  counter_title = (counter_title >= 400) ? 0 : counter_title;
  counter_prompt = (counter_play >= 50) ? !counter_prompt : counter_prompt;
  counter_play = (counter_play >= 50) ? 0 : counter_play;

  if(!Options::joystick_mode){

    if(menu_buttons[menu_button_play].hover())highlight_y_destination=500;
    if(menu_buttons[menu_button_edit].hover())highlight_y_destination=550;
    if(menu_buttons[menu_button_options].hover())highlight_y_destination=600;
    if(menu_buttons[menu_button_help].hover())highlight_y_destination=650;
    if(menu_buttons[menu_button_exit].hover())highlight_y_destination=700;
  }

  // Click anywhere
  //if( keyListener::anyKeyPressed || joystickListener::anyButtonPressed)
  //   set_next_state(STATE_GAME);
}

// Draw images to screen
void menu::draw(){

  // Background
  al_clear_to_color( al_map_rgb(50,50,50));

  if(!credits_menu){


    int title_offset_x = 150;
    int title_offset_y = 50;
    // Title
    al_draw_scaled_bitmap( title, 0,0, 175, 160, 150+title_offset_x, 50+title_offset_y, 612,560, 0);
    if(counter_title<50)al_draw_scaled_bitmap( title_shine, 0,0, 60, 150, 150+title_offset_x+counter_title*10, 50+title_offset_y, 210,525, 0);

    al_draw_scaled_bitmap( title_overlay, 0,0, 200, 160, 150+title_offset_x, 50+title_offset_y, 700,560, 0);

    //al_draw_bitmap(title,300,300,0);
    //al_draw_bitmap(playbutton_frame,297,562,0);

    int playbutton_x = 50;
    int playbutton_y = 605;


    //if(menu_buttons[menu_button_play].hover())
    //  al_draw_scaled_bitmap( playbutton_frame_hover, 0, 0, 70, 38,playbutton_x-12, playbutton_y-12, 280, 152, 0);
   // else
  //    al_draw_scaled_bitmap( playbutton_frame, 0, 0, 70, 38,playbutton_x-12, playbutton_y-12, 280, 152, 0);
//
  //  al_draw_scaled_bitmap( play_images[counter_play], 0, 0, 64, 32,playbutton_x, playbutton_y, 256, 128, 0);

      al_draw_textf( menu_font, al_map_rgb( 255, 255, 255), 1010, 15, 2, "TOJam 12, 2017");
    al_draw_textf( menu_font, al_map_rgb( 255, 255, 255), 1010, 35, 2, "Danny Van Stemp");
    al_draw_textf( menu_font, al_map_rgb( 255, 255, 255), 1010, 55, 2, "Allan Legemaate");
    al_draw_textf( menu_font, al_map_rgb( 255, 255, 255), 1010, 75, 2, "Sullivan Stobo");
    al_draw_textf( menu_font, al_map_rgb( 255, 255, 255), 1010, 95, 2, "Max Keleher");

     // Update buttons
    for( int i = 0; i < BUTTON_COUNT; i++){
      menu_buttons[i].draw();
    }



  al_draw_bitmap(highlight,40,highlight_y,0);
  }

  if(credits_menu){

    int padding=50;
    int x_location=395;
    al_draw_bitmap(logo, 730,50,0);
    al_draw_textf( credits_font, al_map_rgb( 255, 255, 255), x_location, 40+padding, 1,"Written in C++ using Code::Blocks");
    al_draw_textf( credits_font, al_map_rgb( 255, 255, 255), x_location, 80+padding, 1,"Allegro 5 for graphics");
    al_draw_textf( credits_font, al_map_rgb( 255, 255, 255), x_location, 120+padding, 1,"Box2D for physics");
    al_draw_textf( credits_font, al_map_rgb( 255, 255, 255), x_location, 160+padding, 1,"RapidXml for level loading/saving");
    al_draw_textf( credits_font, al_map_rgb( 255, 255, 255), x_location, 240+padding, 1,"Music/code by Allan Legemaate");
    al_draw_textf( credits_font, al_map_rgb( 255, 255, 255), x_location, 280+padding, 1,"Art/game design by Sullivan Stobo");
    al_draw_textf( credits_font, al_map_rgb( 255, 255, 255), x_location, 320+padding, 1,"Level/game design by Max Keleher");
    al_draw_textf( credits_font, al_map_rgb( 255, 255, 255), x_location, 360+padding, 1,"Lead code by Danny Van Stemp");
    al_draw_textf( credits_font, al_map_rgb( 255, 255, 255), x_location, 400+padding, 1, "Art made in Paint.net and Asperite");
    al_draw_textf( credits_font, al_map_rgb( 255, 255, 255), x_location, 440+padding, 1,"Music made in FL Studio");
    al_draw_textf( credits_font, al_map_rgb( 255, 255, 255), 835, 320, 1,"Made for TOJam 12");
    al_draw_textf( credits_font, al_map_rgb( 255, 255, 255), 835, 280, 1,"ADS Games, 2017");

    al_draw_textf( credits_font, al_map_rgb( 255, 100, 100), 40, 720, 0,"Press any key to return.");
  }



  if(Options::draw_cursor)
    al_draw_bitmap(cursor,mouseListener::mouse_x,mouseListener::mouse_y,0);




}
