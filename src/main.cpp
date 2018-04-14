/*
 * Main
 * This is the main for Jumping Jimothy
 * Calls state machine update and draw functions
 * 09/05/2017
 */
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include <mouseListener.h>
#include <keyListener.h>
#include <joystickListener.h>

#include "init.h"
#include "state.h"
#include "game.h"
#include "menu.h"
#include "editor.h"
#include "LevelSelect.h"
#include "Options.h"
#include "MusicManager.h"

// Current state object
state *currentState = nullptr;

// FPS system variables
int fps;
double old_time = 0;
const float MAX_FPS = 60;
int frames_array[100];
int frame_index = 0;

// Closing or naw
bool closing = false;
bool joystick_enabled = false;

// Allegro events
ALLEGRO_EVENT_QUEUE* event_queue = nullptr;
ALLEGRO_TIMER* timer = nullptr;
ALLEGRO_DISPLAY *display = nullptr;
ALLEGRO_BITMAP *buffer;

int graphics_mode;

// Input listener wrapper classes
mouseListener m_listener;
keyListener k_listener;
joystickListener j_listener;

float scaleW;
float scaleH;
float scaleX;
float scaleY;

int screenWidth=1024;
int screenHeight=768;

enum{
  fullscreen_window_stretch,
  fullscreen_window_letterbox,
  fullscreen_window_center,
  fullscreen_true,
  windowed,
};

// Delete game state and free state resources
void clean_up(){
  delete currentState;
}

// Change game screen
void change_state(){
  //If the state needs to be changed
  if( nextState != STATE_NULL ){
    //Delete the current state
    if( nextState != STATE_EXIT ){
      delete currentState;
    }

    //Change the state
    switch( nextState ){
      case STATE_INIT:
        currentState = new init();
        std::cout<<"Switched state to initialization.\n";
        break;
      case STATE_GAME:
        currentState = new game();
        std::cout<<"Switched state to game.\n";
        break;
      case STATE_EDIT:
        currentState = new editor();
        std::cout<<"Switched state to editor.\n";
        break;
      case STATE_MENU:
        currentState = new menu();
        std::cout<<"Switched state to main menu.\n";
        break;
      case STATE_EXIT:
        std::cout<<"Exiting program.\n";
        closing = true;
        break;
      case STATE_LEVELSELECT:
        currentState = new LevelSelect();
        std::cout<<"Switched state to level select.\n";
        break;
      case STATE_OPTIONS:
        currentState = new Options();
        std::cout<<"Switched state to options.\n";
        break;

      default:
        currentState = new game();
    }

    //Change the current state ID
    stateID = nextState;

    //NULL the next state ID
    nextState = STATE_NULL;
  }
}

// Setup game
void setup(){

  std::cout<<"Initializing Allegro.";
  // Init allegro
  if( !al_init())
    tools::abort_on_error( "Allegro could not initilize", "Error");

  // Window title
  al_set_window_title(display,"Loading...");

  // Controls
  al_install_keyboard();
  al_install_mouse();
  al_install_joystick();

  // GFX addons
  al_init_image_addon();
  al_init_primitives_addon();

  // Font
  al_init_ttf_addon();

  // Audio
  al_install_audio();
  al_init_acodec_addon();
  al_reserve_samples( 20);

  // Aquire screen

  #if defined(RELEASE)
    al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
  #endif

  #if !defined(RELEASE)
      al_set_new_display_flags(ALLEGRO_WINDOWED);
  #endif


  graphics_mode=windowed;
  Options::graphics_mode=graphics_mode;


  float windowWidth=1024;
  float windowHeight=768;


  if(graphics_mode>=1 && graphics_mode<=3){
    al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
    Options::draw_cursor=true;
  }

  else if(graphics_mode==fullscreen_true)
    al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);

  else
    al_set_new_display_flags(ALLEGRO_WINDOWED);



  display = al_create_display(windowWidth, windowHeight);

  if(graphics_mode>=1 && graphics_mode<=3)
    al_hide_mouse_cursor(display);



  windowWidth = al_get_display_width(display);
  windowHeight = al_get_display_height(display);

  buffer = al_create_bitmap(screenWidth, screenHeight);

  // calculate scaling factor
  float sx = windowWidth / screenWidth;
  float sy = windowHeight / screenHeight;
  float scale = std::min(sx, sy);

  if(graphics_mode==fullscreen_window_stretch){
    // calculate how much the buffer should be scaled
    scaleW = screenWidth * sx;
    scaleH = screenHeight * sy;
    scaleX = (windowWidth - scaleW) / 2;
    scaleY = (windowHeight - scaleH) / 2;
  }

  if(graphics_mode==fullscreen_window_center){
    scaleW = screenWidth * 1;
    scaleH = screenHeight * 1;
    scaleX = (windowHeight - scaleH) / 2;
    scaleY = (windowWidth - scaleW) / 2;;
  }

   if(graphics_mode==fullscreen_window_letterbox){
    scaleW = screenWidth * scale;
    scaleH = screenHeight * scale;
    scaleX = (windowWidth - scaleW) / 2;
    scaleY = (windowHeight - scaleH) / 2;
  }














  if( !display)
    tools::abort_on_error( "Screen could not be created", "Error");

  // Timer
  timer = al_create_timer(1.0 / MAX_FPS);

  // Register events
  event_queue = al_create_event_queue();
  al_register_event_source( event_queue, al_get_display_event_source(display));
  al_register_event_source( event_queue, al_get_timer_event_source(timer));
  al_register_event_source( event_queue, al_get_keyboard_event_source());
  al_register_event_source( event_queue, al_get_joystick_event_source());

  // Timer
  al_start_timer(timer);

  // Window title
  al_set_window_title(display,"Jumping Jimothy");

  std::cout<<" Sucesss.\n";

  #if defined(DEBUG)
    std::cout<<"Build target: Debug\n";
  #endif

  #if defined(RELEASE)
    std::cout<<"Build target: Release\n";
  #endif


  // Probably never going to be relevant but pretty cool anyways
  uint32_t version = al_get_allegro_version();
  int major = version >> 24;
  int minor = (version >> 16) & 255;
  int revision = (version >> 8) & 255;
  int release = version & 255;

  std::cout<<"Allegro version "<<major<<"."<<minor<<"."<<revision<<"."<<release<<"\n";

  // This is actually completely irrelevant other than making fun of Allan's PC when he runs this
  // Sorry, your PC is a very nice PC
  std::cout<<"Running as "<<al_get_app_name()<<", with "<<al_get_ram_size()<<" MB RAM.\n";


  joystick_enabled = (al_get_num_joysticks() > 0);

  if(joystick_enabled){
    std::cout<<al_get_joystick_name(al_get_joystick(0)) <<" is installed and being used.\n";
    Options::joystick_data=al_get_joystick_name(al_get_joystick(0));
  }else
    std::cout<<"No joystick is installed.\n";

  Options::read_data();
  MusicManager::load();


}

// Handle events
void update(){
  // Event checking
  ALLEGRO_EVENT ev;
  al_wait_for_event( event_queue, &ev);

  // Timer
  if( ev.type == ALLEGRO_EVENT_TIMER){
    // Change state (if needed)
    change_state();

    // Update listeners
    m_listener.update();
    k_listener.update();
    j_listener.update();

    // Update state
    currentState -> update();
  }
  // Exit
  else if( ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
    closing = true;
  }
  // Keyboard
  else if( ev.type == ALLEGRO_EVENT_KEY_DOWN || ev.type == ALLEGRO_EVENT_KEY_UP){
    k_listener.on_event( ev.type, ev.keyboard.keycode);
  }
  // Joystick
  else if( ev.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN || ev.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_UP){
    j_listener.on_event( ev.type, ev.joystick.button);
  }

  // Joystick plugged or unplugged
  else if( ev.type == ALLEGRO_EVENT_JOYSTICK_CONFIGURATION){
    al_reconfigure_joysticks();
    joystick_enabled = (al_get_num_joysticks() > 0);
    if(joystick_enabled){
      Options::joystick_data=al_get_joystick_name(al_get_joystick(0));
      std::cout<<"Joystick "<<al_get_joystick_name(al_get_joystick(0))<<" is configured.\n";
    }else{
      std::cout<<"Joystick unplugged.\n";
      Options::joystick_data="None detected.";

    }

  }

  // Drawing
  if( al_is_event_queue_empty(event_queue)){
    // Clear buffer
    al_clear_to_color( al_map_rgb(0,0,0));



      // render a frame
      if(graphics_mode>=1 && graphics_mode<=3){
        al_set_target_bitmap(buffer);
        al_clear_to_color(al_map_rgb(0, 0, 0));
      }


      currentState -> draw();


      if(graphics_mode>=1 && graphics_mode<=3){

        al_set_target_backbuffer(display);
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_scaled_bitmap(buffer, 0, 0, screenWidth, screenHeight, scaleX, scaleY, scaleW, scaleH, 0);
      }






      // Draw state graphics




    // Flip (OpenGL)
    al_flip_display();

    // Update fps buffer
    for( int i = 99; i > 0; i--)
      frames_array[i] = frames_array[i - 1];
    frames_array[0] = (1.0/(al_get_time() - old_time));
    old_time = al_get_time();

    int fps_total = 0;
    for( int i = 0; i < 100; i++)
      fps_total += frames_array[i];

    // FPS = average
    fps = fps_total/100;
   // al_set_window_title(display,tools::convertIntToString(fps).c_str());

  }
}

// Start here
int main(int argc, char **argv){
  // Basic init
  setup();

  //Set the current state ID
  stateID = STATE_INIT;
  currentState = new init();

  // Run game
  while(!closing)
    update();

  // Destory display
  MusicManager::destroy();
  al_destroy_display(display);

  return 0;
}
