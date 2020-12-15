#include "JoystickListener.h"

bool JoystickListener::button[JOY_MAX_BUTTONS] = {false};
bool JoystickListener::buttonPressed[JOY_MAX_BUTTONS] = {false};
bool JoystickListener::buttonReleased[JOY_MAX_BUTTONS] = {false};
bool JoystickListener::lastTicksButton[JOY_MAX_BUTTONS] = {false};

bool JoystickListener::anyButtonPressed = false;

bool JoystickListener::stick[JOY_MAX_STICK_BOOLEANS] = {false};
bool JoystickListener::stickMoved[JOY_MAX_STICK_BOOLEANS] = {false};
bool JoystickListener::stickReleased[JOY_MAX_STICK_BOOLEANS] = {false};
bool JoystickListener::lastTicksStick[JOY_MAX_STICK_BOOLEANS] = {false};

// For allegro 5, we use events
void JoystickListener::on_event(ALLEGRO_EVENT_TYPE event_type,
                                const int buttoncode) {
  // Button down
  if (event_type == ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN) {
    button[buttoncode] = true;
  }

  // Button up
  else if (event_type == ALLEGRO_EVENT_JOYSTICK_BUTTON_UP) {
    button[buttoncode] = false;
  }
}

// For allegro 5, we use events
void JoystickListener::on_event(ALLEGRO_EVENT_TYPE event_type,
                                const int stick,
                                const int axis,
                                const float position) {
  // Axis moved
  if (event_type == ALLEGRO_EVENT_JOYSTICK_AXIS) {
    const float tolerance = 0.6f;

    // Translated axis
    int trans_axis = (stick * JOY_MAX_AXES + axis) * 2;

    // Check if moved enough
    if (position > tolerance) {
      this->stick[trans_axis] = true;
    } else if (position < -tolerance) {
      this->stick[trans_axis + 1] = true;
    } else {
      this->stick[trans_axis] = false;
      this->stick[trans_axis + 1] = false;
    }
  }
}

// Check those button!
void JoystickListener::update() {
  // Check stick just moved
  for (int i = 0; i < JOY_MAX_STICK_BOOLEANS; i++) {
    // Clear old values
    stickMoved[i] = false;
    stickReleased[i] = false;

    // Pressed since last tick?
    if (stick[i] == true && lastTicksStick[i] == false)
      stickMoved[i] = true;

    // Released since last tick?
    if (stick[i] == false && lastTicksStick[i] == true)
      stickReleased[i] = true;

    // Stick changed
    if (lastTicksStick[i] != stick[i])
      lastTicksStick[i] = stick[i];
  }

  // Reset button pressed
  anyButtonPressed = false;

  // Check button just pressed
  for (int i = 0; i < JOY_MAX_BUTTONS; i++) {
    // Clear old values
    buttonPressed[i] = false;
    buttonReleased[i] = false;

    // Pressed since last tick?
    if (button[i] == true && lastTicksButton[i] == false) {
      buttonPressed[i] = true;
      anyButtonPressed = true;
    }

    // Released since last tick?
    if (button[i] == false && lastTicksButton[i] == true)
      buttonReleased[i] = true;

    // Button changed
    if (lastTicksButton[i] != button[i])
      lastTicksButton[i] = button[i];
  }
}
