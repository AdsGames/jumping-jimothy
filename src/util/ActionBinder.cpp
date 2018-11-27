#include "util/ActionBinder.h"

#include "util/KeyListener.h"
#include "util/JoystickListener.h"
#include "util/Tools.h"

std::vector<Binding*> ActionBinder::bindings;

bool ActionBinder::actionBegun(const int action) {
  std::vector<Binding*> foundBindings = findBindings(action);

  for (unsigned int i = 0; i < foundBindings.size(); i++) {
    if (foundBindings.at(i) -> getCode() != BINDING_NONE) {
      switch (foundBindings.at(i) -> getType()) {
        case TYPE_KEY:
          if (KeyListener::keyPressed[foundBindings.at(i) -> getCode()]) {
            return true;
          }
          break;
        case TYPE_JOY_BUTTON:
          if (JoystickListener::buttonPressed[foundBindings.at(i) -> getCode()]) {
            return true;
          }
          break;
        case TYPE_JOY_STICK:
          if (JoystickListener::stickDirections[foundBindings.at(i) -> getCode()]) {
            return true;
          }
          break;
      }
    }
  }

  return false;
}

bool ActionBinder::actionHeld(const int action) {
  return false;
}

std::vector<Binding*> ActionBinder::findBindings(const int action) {
  // Empty binding vector
  std::vector<Binding*> foundBindings;

  // Find
  for (unsigned int i = 0; i < ActionBinder::bindings.size(); i++) {
    if (ActionBinder::bindings.at(i) -> getAction() == action) {
      foundBindings.push_back(ActionBinder::bindings.at(i));
    }
  }

  // Return vector
  return foundBindings;
}

// Add binding
void ActionBinder::addBinding(int action, int type, int code) {
  ActionBinder::bindings.push_back(new Binding(action, type, code));
}

void ActionBinder::setDefaults(){
  // Left
  addBinding(ACTION_LEFT, TYPE_KEY, ALLEGRO_KEY_A);
  addBinding(ACTION_LEFT, TYPE_KEY, ALLEGRO_KEY_LEFT);
  addBinding(ACTION_LEFT, TYPE_JOY_STICK, LEFT_STICK_LEFT);
  addBinding(ACTION_LEFT, TYPE_JOY_STICK, DPAD_LEFT);

  // Right
  addBinding(ACTION_RIGHT, TYPE_KEY, ALLEGRO_KEY_D);
  addBinding(ACTION_RIGHT, TYPE_KEY, ALLEGRO_KEY_RIGHT);
  addBinding(ACTION_RIGHT, TYPE_JOY_STICK, LEFT_STICK_RIGHT);
  addBinding(ACTION_RIGHT, TYPE_JOY_STICK, DPAD_RIGHT);

  // Up
  addBinding(ACTION_UP, TYPE_KEY, ALLEGRO_KEY_UP);
  addBinding(ACTION_UP, TYPE_KEY, ALLEGRO_KEY_W);
  addBinding(ACTION_UP, TYPE_JOY_STICK, LEFT_STICK_UP);
  addBinding(ACTION_UP, TYPE_JOY_STICK, DPAD_UP);

  // Down
  addBinding(ACTION_DOWN, TYPE_KEY, ALLEGRO_KEY_DOWN);
  addBinding(ACTION_DOWN, TYPE_KEY, ALLEGRO_KEY_S);
  addBinding(ACTION_DOWN, TYPE_JOY_STICK, LEFT_STICK_DOWN);
  addBinding(ACTION_DOWN, TYPE_JOY_STICK, DPAD_DOWN);

  // Action A
  addBinding(ACTION_A, TYPE_KEY, ALLEGRO_KEY_W);
  addBinding(ACTION_A, TYPE_JOY_STICK, DPAD_UP);
  addBinding(ACTION_A, TYPE_JOY_BUTTON, DPAD_DOWN);

  // Action B
  addBinding(ACTION_B, TYPE_KEY, ALLEGRO_KEY_SPACE);
  addBinding(ACTION_B, TYPE_JOY_BUTTON, JOY_XBOX_X);
  addBinding(ACTION_B, TYPE_JOY_BUTTON, JOY_XBOX_BUMPER_LEFT);
  addBinding(ACTION_B, TYPE_JOY_BUTTON, JOY_XBOX_BUMPER_RIGHT);
}
