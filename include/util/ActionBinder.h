/**
 * Action Binder
 * Danny Van Stemp
 * Abstraction layer on top of key/joy
 *   codes for keybindings
 * 05/05/2017
**/

#ifndef ACTIONBINDER_H
#define ACTIONBINDER_H

#include "allegro5/keycodes.h"
#include "JoystickCodes.h"

class ActionBinder {
  public:
    ActionBinder();
    virtual ~ActionBinder();

    static const int ALLEGRO_KEY_NONE = ALLEGRO_KEY_KANJI;
    static const int NUM_BINDABLE_BUTTONS = 3;
    static const int NUM_BINDABLE_ACTIONS = 10;

    struct binding {
      int key_code[NUM_BINDABLE_BUTTONS] = {ALLEGRO_KEY_NONE,ALLEGRO_KEY_NONE,ALLEGRO_KEY_NONE};
      int joystick_button[NUM_BINDABLE_BUTTONS] = {JOY_NONE,JOY_NONE,JOY_NONE};
      int stick[NUM_BINDABLE_BUTTONS] = {NONE,NONE,NONE};
    };

    static binding game_binding[NUM_BINDABLE_ACTIONS];

    enum actions {
      jump,
      move_left,
      move_right,
      freeze,
      restart,
      confirm,
      back
    };

    static bool actionPressed(const int action);
    static bool actionHeld(const int action);
    static void setDefaults();
};

#endif // ACTIONBINDER_H
