/**
 * Joystick Listener
 * Allan Legemaate
 * Listens for joystick buttons JUST pressed or JUST released
 *   since the last tick
 * 23/01/2017
**/

#ifndef JOYSTICKLISTENER_H
#define JOYSTICKLISTENER_H

#include <allegro5/allegro.h>
#include "util/JoystickCodes.h"

class JoystickListener {
  public:
    JoystickListener() {};
    virtual ~JoystickListener() {};

    void on_event(ALLEGRO_EVENT_TYPE event_type, const int keycode);
    void update();

    static const int JOY_MAX_STICKS = 8;
    static const int JOY_MAX_AXES = 3;
    static const int JOY_MAX_BUTTONS = 14;

    static bool button[JOY_MAX_BUTTONS];
    static bool buttonPressed[JOY_MAX_BUTTONS];
    static bool buttonReleased[JOY_MAX_BUTTONS];
    static bool anyButtonPressed;
    static bool anyButtonReleased;

    static int lastButtonPressed;
    static int lastButtonReleased;
    static bool stickDirections[20];
    static void clearButtons();

  protected:
  private:
    static bool lastTicksButton[JOY_MAX_BUTTONS];
    static ALLEGRO_JOYSTICK_STATE joyState;
};

#endif // JOYSTICKLISTENER_H
