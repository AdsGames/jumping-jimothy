#include "joystickListener.h"

bool joystickListener::button[JOY_MAX_BUTTONS] = { false };
bool joystickListener::buttonPressed[JOY_MAX_BUTTONS] = { false};
bool joystickListener::buttonReleased[JOY_MAX_BUTTONS] = { false};
bool joystickListener::lastTicksButton[JOY_MAX_BUTTONS] = { false};
int joystickListener::lastButtonPressed = -1;
int joystickListener::lastButtonReleased = -1;
bool joystickListener::anyButtonPressed=false;
bool joystickListener::stickDirections[4];
ALLEGRO_JOYSTICK_STATE joystickListener::joyState;


// Constructor
joystickListener::joystickListener(){

}

// Destructor
joystickListener::~joystickListener(){

}

// For allegro 5, we use events
void joystickListener::on_event( ALLEGRO_EVENT_TYPE event_type, int buttoncode){
  // Button down
  if( event_type == ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN){
    button[buttoncode] = true;
  }
  // Button up
  else if( event_type == ALLEGRO_EVENT_JOYSTICK_BUTTON_UP){
    button[buttoncode] = false;
  }
}


// Check those button!
void joystickListener::update(){

  stickDirections[STICK_0_AXIS_0_UP]=false;
  stickDirections[STICK_0_AXIS_0_DOWN]=false;


  if(al_get_num_joysticks() > 0){


    al_get_joystick_state(al_get_joystick(0),&joyState);
    float pos = joyState.stick[0].axis[0];

    float tolerance=0.4f;

    if(pos>tolerance)
      stickDirections[STICK_0_AXIS_0_UP]=true;

    if(pos<-tolerance)
      stickDirections[STICK_0_AXIS_0_DOWN]=true;

  }
  // Reset last button
  lastButtonPressed = -1;
  lastButtonReleased = -1;

  anyButtonPressed = false;




  // Check button just pressed
  for( int i = 0; i < JOY_MAX_BUTTONS; i++){

    if(button[i])
      anyButtonPressed=true;

    // Clear old values
    buttonPressed[i] = false;
    buttonReleased[i] = false;

    // Pressed since last tick?
    if( button[i] == true && lastTicksButton[i] == false){
      buttonPressed[i] = true;
      lastButtonPressed = i;
      //std::cout << "Button: " << i << " pressed. \n";
    }

    // Released since last tick?
    if( button[i] == false && lastTicksButton[i] == true){
      buttonReleased[i] = true;
      lastButtonReleased = i;
      //std::cout << "Button: " << i << " released. \n";
    }
  }


  // Get new values
  for( int i = 0; i < JOY_MAX_BUTTONS; i++){
    // Button changed
    if( lastTicksButton[i] != button[i]){
        //std::cout << "Button: " << i << " was " << lastTicksButton[i] << " and became " << (bool)button[i] << "\n";
        lastTicksButton[i] = button[i];
    }
  }
}

