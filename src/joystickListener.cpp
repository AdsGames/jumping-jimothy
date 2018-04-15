#include "joystickListener.h"

bool joystickListener::button[JOY_MAX_BUTTONS] = { false };
bool joystickListener::buttonPressed[JOY_MAX_BUTTONS] = { false};
bool joystickListener::buttonReleased[JOY_MAX_BUTTONS] = { false};
bool joystickListener::lastTicksButton[JOY_MAX_BUTTONS] = { false};
int joystickListener::lastButtonPressed = -1;
int joystickListener::lastButtonReleased = -1;
bool joystickListener::anyButtonPressed=false;
bool joystickListener::anyButtonReleased=false;

bool joystickListener::stickDirections[20];
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

void joystickListener::clearButtons(){

 for( int i = 0; i < JOY_MAX_BUTTONS; i++){

    buttonPressed[i] = false;
    buttonReleased[i] = false;
  }

}

// Check those button!
void joystickListener::update(){

  stickDirections[LEFT_STICK_LEFT]=false;
  stickDirections[LEFT_STICK_RIGHT]=false;
  stickDirections[LEFT_STICK_UP]=false;
  stickDirections[LEFT_STICK_DOWN]=false;
  stickDirections[LEFT_TRIGGER_DOWN]=false;
  stickDirections[RIGHT_TRIGGER_DOWN]=false;
  stickDirections[RIGHT_STICK_LEFT]=false;
  stickDirections[RIGHT_STICK_RIGHT]=false;
  stickDirections[RIGHT_STICK_UP]=false;
  stickDirections[RIGHT_STICK_DOWN]=false;
  stickDirections[DPAD_UP2]=false;
  stickDirections[DPAD_DOWN]=false;
  stickDirections[DPAD_LEFT]=false;
  stickDirections[DPAD_RIGHT]=false;



   if(al_get_num_joysticks() > 0){


    al_get_joystick_state(al_get_joystick(0),&joyState);

    float tolerance=0.4f;

    if(joyState.stick[0].axis[0]>tolerance)
      stickDirections[LEFT_STICK_RIGHT]=true;

    if(joyState.stick[0].axis[0]<-tolerance)
      stickDirections[LEFT_STICK_LEFT]=true;

    if(joyState.stick[0].axis[1]>tolerance)
      stickDirections[LEFT_STICK_UP]=true;

    if(joyState.stick[0].axis[1]<-tolerance)
      stickDirections[LEFT_STICK_DOWN]=true;

    if(joyState.stick[1].axis[0]>(-1+tolerance))
      stickDirections[LEFT_TRIGGER_DOWN]=true;

    if(joyState.stick[2].axis[1]>(-1+tolerance))
      stickDirections[RIGHT_TRIGGER_DOWN]=true;

    if(joyState.stick[1].axis[1]>tolerance)
      stickDirections[RIGHT_STICK_RIGHT]=true;

    if(joyState.stick[1].axis[1]<-tolerance)
      stickDirections[RIGHT_STICK_LEFT]=true;

    if(joyState.stick[1].axis[2]>tolerance)
      stickDirections[RIGHT_STICK_UP]=true;

    if(joyState.stick[1].axis[2]<-tolerance)
      stickDirections[RIGHT_STICK_DOWN]=true;

    if(joyState.stick[3].axis[0]==-1)
      stickDirections[DPAD_LEFT]=true;

    if(joyState.stick[3].axis[0]==1)
      stickDirections[DPAD_RIGHT]=true;

   if(joyState.stick[3].axis[1]==1)
      stickDirections[DPAD_UP2]=true;

    if(joyState.stick[3].axis[1]==-1)
      stickDirections[DPAD_DOWN]=true;


  }


  // Reset last button
  lastButtonPressed = -1;
  lastButtonReleased = -1;

  anyButtonPressed = false;
  anyButtonReleased=false;




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
      anyButtonReleased=true;
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

