/**
 * Main
 * Allan Legemaate and Danny Van Stemp
 * This is the main for Jumping Jimothy
 * Calls state machine update and draw functions
 * 09/05/2017
 **/
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_ttf.h>

#include "MouseListener.h"
#include "KeyListener.h"
#include "JoystickListener.h"

#include "Init.h"
#include "State.h"
#include "Game.h"
#include "Menu.h"
#include "Editor.h"
#include "LevelSelect.h"
#include "Options.h"
#include "Config.h"
#include "MusicManager.h"
#include "DisplayMode.h"
#include "Tools.h"

// Current state object
State *currentState = nullptr;

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

// Input listener wrapper classes
MouseListener m_listener;
KeyListener k_listener;
JoystickListener j_listener;

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
        currentState = new Init();
        tools::log_message("Switched state to initialization.");
        break;
      case STATE_GAME:
        currentState = new Game();
        tools::log_message("Switched state to game.");
        break;
      case STATE_EDIT:
        currentState = new Editor();
        tools::log_message("Switched state to editor.");
        break;
      case STATE_MENU:
        currentState = new Menu();
        tools::log_message("Switched state to main menu.");
        break;
      case STATE_EXIT:
        tools::log_message("Exiting program.");
        closing = true;
        break;
      case STATE_LEVELSELECT:
        currentState = new LevelSelect();
        tools::log_message("Switched state to level select.");
        break;
      case STATE_OPTIONS:
        currentState = new Options();
        tools::log_message("Switched state to options.");
        break;

      default:
        currentState = new Game();
    }

    //Change the current state ID
    stateID = nextState;

    //NULL the next state ID
    nextState = STATE_NULL;
  }
}

// Setup game
void setup(){
  tools::log_message("Initializing Allegro.");

  // Init allegro
  if( !al_init())
    tools::abort_on_error( "Allegro could not initilize", "Error");

  // Window title
  al_set_window_title(display, "Loading...");

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

  // Load config
  Config::read_data("data/options_data.xml");

  // Set display mode to windowed
  DisplayMode::setActiveDisplay(&display);
  DisplayMode::setMode(Config::graphics_mode);
  buffer = al_create_bitmap(DisplayMode::getDrawWidth(), DisplayMode::getDrawHeight());

  if (!display)
    tools::abort_on_error("Screen could not be created", "Error");

  // Timer
  timer = al_create_timer(1.0 / MAX_FPS);

  // Register events
  event_queue = al_create_event_queue();
  al_register_event_source(event_queue, al_get_display_event_source(display));
  al_register_event_source(event_queue, al_get_timer_event_source(timer));
  al_register_event_source(event_queue, al_get_keyboard_event_source());
  al_register_event_source(event_queue, al_get_joystick_event_source());

  // Timer
  al_start_timer(timer);

  // Window title
  al_set_window_title(display,"Jumping Jimothy");

  tools::log_message(" Sucesss.");


  #if defined(DEBUG)
    tools::log_message("Build target: Debug");
  #endif

  #if defined(RELEASE)
    tools::log_message("Build target: Release");
  #endif

  // Probably never going to be relevant but pretty cool anyways
  uint32_t version = al_get_allegro_version();
  int major = version >> 24;
  int minor = (version >> 16) & 255;
  int revision = (version >> 8) & 255;
  int release = version & 255;

  tools::log_message("Allegro version " + tools::toString(major) + "." + tools::toString(minor) + "." + tools::toString(revision) + "." + tools::toString(release));

  // This is actually completely irrelevant other than making fun of Allan's PC when he runs this
  // Sorry, your PC is a very nice PC
  tools::log_message("Running as " + tools::toString(al_get_app_name()) + ", with " + tools::toString(al_get_ram_size()) + " MB RAM.");


  joystick_enabled = (al_get_num_joysticks() > 0);

  if (joystick_enabled) {
    tools::log_message(tools::toString(al_get_joystick_name(al_get_joystick(0))) + " is installed and being used.");
    Config::joystick_data = al_get_joystick_name(al_get_joystick(0));
  }
  else {
    tools::log_message("No joystick is installed.");
    Config::joystick_data = "None detected.";
  }

  if (Config::music_enabled) {
    MusicManager::load();
  }
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

    if(joystick_enabled) {
      Config::joystick_data = al_get_joystick_name(al_get_joystick(0));
      tools::log_message("Joystick " + tools::toString(al_get_joystick_name(al_get_joystick(0))) + " is configured.");
    }
    else{
      tools::log_message("Joystick unplugged.");
      Config::joystick_data = "None detected.";
    }
  }

  // Drawing
  if( al_is_event_queue_empty(event_queue)) {
    // Clear buffer
    al_clear_to_color( al_map_rgb(0,0,0));

    // Render a frame
    al_set_target_bitmap(buffer);
    al_clear_to_color(al_map_rgb(0, 0, 0));
    currentState -> draw();

    al_set_target_backbuffer(display);
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_scaled_bitmap(buffer,
                          0,
                          0,
                          DisplayMode::getDrawWidth(),
                          DisplayMode::getDrawHeight(),
                          DisplayMode::getTranslationX(),
                          DisplayMode::getTranslationY(),
                          DisplayMode::getScaleWidth(),
                          DisplayMode::getScaleHeight(),
                          0);

    // Flip (OpenGL)
    al_flip_display();

    // Update fps buffer
    for( int i = 99; i > 0; i--) {
      frames_array[i] = frames_array[i - 1];
    }
    frames_array[0] = (1.0/(al_get_time() - old_time));
    old_time = al_get_time();

    int fps_total = 0;
    for( int i = 0; i < 100; i++) {
      fps_total += frames_array[i];
    }

    // FPS = average
    fps = fps_total/100;
    // al_set_window_title(display,tools::convertIntToString(fps).c_str());
  }
}

// Start here
int main(){
  // Basic init
  setup();

  //Set the current state ID
  stateID = STATE_INIT;
  currentState = new Init();

  // Run game
  while(!closing) {
    update();
  }

  // Destory display
  MusicManager::destroy();
  al_destroy_display(display);

  return 0;
}
