#include <stdio.h>
#include <vector>
#include <fstream>
#include <iostream>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#include <mouseListener.h>
#include <keyListener.h>
#include <joystickListener.h>

#include "state.h"
#include "game.h"
#include "editor.h"

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
ALLEGRO_DISPLAY *display;


// Input listener wrapper classes
mouseListener m_listener;
keyListener k_listener;
joystickListener j_listener;

// Functions
void clean_up();
void change_state();
void setup();
void update();
void draw();

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
      case STATE_GAME:
        currentState = new game();
        break;
      case STATE_EDIT:
        currentState = new editor();
        break;
      case STATE_EXIT:
        closing = true;
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

// Setup Allegro stuffs
void al_setup(){
  al_init();

  al_install_keyboard();
  al_install_mouse();
  al_install_joystick();

  al_init_image_addon();
  al_init_primitives_addon();

  timer = al_create_timer(1.0 / MAX_FPS);

  display = al_create_display(800, 600);

  event_queue = al_create_event_queue();
  al_register_event_source( event_queue, al_get_display_event_source(display));
  al_register_event_source( event_queue, al_get_timer_event_source(timer));
  al_register_event_source( event_queue, al_get_keyboard_event_source());
  al_register_event_source( event_queue, al_get_joystick_event_source());

  al_start_timer(timer);

}

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

    // Change state?
    if( keyListener::key[ALLEGRO_KEY_P]){
      stateID = STATE_EDIT;
    }

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
  }

  // Drawing
  if( al_is_event_queue_empty(event_queue)){
    al_clear_to_color( al_map_rgb(0,0,0));
    currentState -> draw();
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
  }
}


int main(int argc, char **argv){
  al_setup();

  //Set the current state ID
  stateID = STATE_EDIT;

  //Set the current game state object
  currentState = new editor();

  while(!closing)
    update();

  al_destroy_display(display);

  return 0;
}
