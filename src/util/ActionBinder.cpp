#include "util/ActionBinder.h"

#include "util/KeyListener.h"
#include "util/joystickListener.h"

ActionBinder::binding ActionBinder::game_binding[10];

bool ActionBinder::actionPressed(const int action){
  for(int i=0; i<NUM_BINDABLE_BUTTONS; i++){
    if(KeyListener::keyPressed[game_binding[action].key_code[i]])
      return true;
    if(JoystickListener::stickDirections[game_binding[action].stick[i]])
      return true;

     if(JoystickListener::buttonPressed[game_binding[action].joystick_button[i]])
      return true;
    }
  return false;

}

bool ActionBinder::actionHeld(const int action){

  for(int i=0; i<NUM_BINDABLE_BUTTONS; i++){
    if(KeyListener::key[game_binding[action].key_code[i]])
      return true;
    if(JoystickListener::stickDirections[game_binding[action].stick[i]])
      return true;

    if(JoystickListener::button[game_binding[action].joystick_button[i]])
      return true;
    }
  return false;

}

void ActionBinder::setDefaults(){
  game_binding[jump].key_code[0] = ALLEGRO_KEY_W;
  game_binding[jump].joystick_button[0] = JOY_XBOX_A;
  game_binding[jump].stick[0] = DPAD_UP;


  game_binding[move_left].key_code[0]=ALLEGRO_KEY_A;
  game_binding[move_left].stick[0] = LEFT_STICK_LEFT;
  game_binding[move_left].stick[1] = DPAD_LEFT;


  game_binding[move_right].key_code[0]=ALLEGRO_KEY_D;
  game_binding[move_right].stick[0] = LEFT_STICK_RIGHT;
  game_binding[move_right].stick[1] = DPAD_RIGHT;


  game_binding[freeze].key_code[0]=ALLEGRO_KEY_SPACE;
  game_binding[freeze].joystick_button[0]=JOY_XBOX_X;
  game_binding[freeze].joystick_button[1]=JOY_XBOX_BUMPER_LEFT;
  game_binding[freeze].joystick_button[2]=JOY_XBOX_BUMPER_RIGHT;

  game_binding[restart].key_code[0] = ALLEGRO_KEY_R;
  game_binding[restart].joystick_button[0] = JOY_XBOX_B;

  game_binding[confirm].key_code[0]=ALLEGRO_KEY_ENTER;
  game_binding[confirm].joystick_button[0]=JOY_XBOX_A;


}

ActionBinder::ActionBinder()
{
  //ctor
}

ActionBinder::~ActionBinder()
{
  //dtor
}
