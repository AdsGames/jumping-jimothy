/**
 * Joystick Listener
 *   Allan Legemaate
 * Listens for joystick buttons JUST pressed or JUST released
 *   since the last tick
 * 23/01/2017
**/

#ifndef JOYSTICKLISTENER_H
#define JOYSTICKLISTENER_H

#define JOY_MAX_STICKS   8
#define JOY_MAX_AXES     3
#define JOY_MAX_BUTTONS  14

#define JOY_XBOX_A            0
#define JOY_XBOX_B            1
#define JOY_XBOX_X            2
#define JOY_XBOX_Y            3
#define JOY_XBOX_BUMPER_RIGHT 4
#define JOY_XBOX_BUMPER_LEFT  5
#define JOY_XBOX_STICK_RIGHT  6
#define JOY_XBOX_STICK_LEFT   7
#define JOY_XBOX_BACK         8
#define JOY_XBOX_START        9
#define JOY_XBOX_PAD_RIGHT    10
#define JOY_XBOX_PAD_LEFT     11
#define JOY_XBOX_PAD_DOWN     12
#define JOY_XBOX_PAD_UP       13


#define LEFT_STICK_LEFT 0
#define LEFT_STICK_RIGHT 1
#define LEFT_STICK_UP 2
#define LEFT_STICK_DOWN 3
#define LEFT_TRIGGER_DOWN 4
#define RIGHT_TRIGGER_DOWN 5
#define RIGHT_STICK_LEFT 6
#define RIGHT_STICK_RIGHT 7
#define RIGHT_STICK_UP 8
#define RIGHT_STICK_DOWN 9
#define DPAD_LEFT 10
#define DPAD_RIGHT 11
#define DPAD_UP 12
#define DPAD_UP2 13
#define DPAD_DOWN 14

#include <iostream>
#include <allegro5/allegro.h>


class joystickListener
{
  public:
    joystickListener();
    virtual ~joystickListener();

    void on_event( ALLEGRO_EVENT_TYPE event_type, int keycode);
    void update();

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
