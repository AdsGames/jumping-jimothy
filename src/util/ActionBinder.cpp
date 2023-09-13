#include "ActionBinder.h"

#include <allegro5/keycodes.h>

#include "JoystickListener.h"
#include "KeyListener.h"

std::vector<Binding*> ActionBinder::bindings;

bool ActionBinder::actionBegun(const Action action) {
  std::vector<Binding*> foundBindings = findBindings(action);

  for (const auto& binding : foundBindings) {
    const auto code = binding->getCode();

    if (code == BINDING_NONE) {
      continue;
    }

    switch (binding->getType()) {
      case ActionType::KEY:
        if (KeyListener::keyPressed[code]) {
          return true;
        }
        break;
      case ActionType::JOY_BUTTON:
        if (JoystickListener::buttonPressed[code]) {
          return true;
        }
        break;
      case ActionType::JOY_STICK:
        if (JoystickListener::stickMoved[code]) {
          return true;
        }
        break;
      case ActionType::NONE:
        break;
    }
  }

  return false;
}

bool ActionBinder::actionHeld(const Action action) {
  std::vector<Binding*> foundBindings = findBindings(action);

  for (const auto& binding : foundBindings) {
    const auto code = binding->getCode();

    if (code == BINDING_NONE) {
      continue;
    }

    switch (binding->getType()) {
      case ActionType::KEY:
        if (KeyListener::key[code]) {
          return true;
        }
        break;
      case ActionType::JOY_BUTTON:
        if (JoystickListener::button[code]) {
          return true;
        }
        break;
      case ActionType::JOY_STICK:
        if (JoystickListener::stick[code]) {
          return true;
        }
        break;
      case ActionType::NONE:
        break;
    }
  }

  return false;
}

std::vector<Binding*> ActionBinder::findBindings(const Action action) {
  // Empty binding vector
  std::vector<Binding*> foundBindings;

  // Find
  for (auto& binding : ActionBinder::bindings) {
    if (binding->getAction() == action) {
      foundBindings.push_back(binding);
    }
  }

  // Return vector
  return foundBindings;
}

// Add binding
void ActionBinder::addBinding(Action action, ActionType type, int code) {
  ActionBinder::bindings.push_back(new Binding(action, type, code));
}

void ActionBinder::setDefaults() {
  // Left
  addBinding(Action::LEFT, ActionType::KEY, ALLEGRO_KEY_A);
  addBinding(Action::LEFT, ActionType::KEY, ALLEGRO_KEY_LEFT);
  addBinding(Action::LEFT, ActionType::JOY_STICK, LEFT_STICK_LEFT);
  addBinding(Action::LEFT, ActionType::JOY_BUTTON, JOY_XBOX_PAD_LEFT);

  // Right
  addBinding(Action::RIGHT, ActionType::KEY, ALLEGRO_KEY_D);
  addBinding(Action::RIGHT, ActionType::KEY, ALLEGRO_KEY_RIGHT);
  addBinding(Action::RIGHT, ActionType::JOY_STICK, LEFT_STICK_RIGHT);
  addBinding(Action::RIGHT, ActionType::JOY_BUTTON, JOY_XBOX_PAD_RIGHT);

  // Up
  addBinding(Action::UP, ActionType::KEY, ALLEGRO_KEY_UP);
  addBinding(Action::UP, ActionType::KEY, ALLEGRO_KEY_W);
  addBinding(Action::UP, ActionType::JOY_STICK, LEFT_STICK_UP);
  addBinding(Action::UP, ActionType::JOY_BUTTON, JOY_XBOX_PAD_UP);

  // Down
  addBinding(Action::DOWN, ActionType::KEY, ALLEGRO_KEY_DOWN);
  addBinding(Action::DOWN, ActionType::KEY, ALLEGRO_KEY_S);
  addBinding(Action::DOWN, ActionType::JOY_STICK, LEFT_STICK_DOWN);
  addBinding(Action::DOWN, ActionType::JOY_BUTTON, JOY_XBOX_PAD_DOWN);

  // Action A
  addBinding(Action::A, ActionType::KEY, ALLEGRO_KEY_W);
  addBinding(Action::A, ActionType::JOY_BUTTON, JOY_XBOX_A);

  // Action B
  addBinding(Action::B, ActionType::KEY, ALLEGRO_KEY_SPACE);
  addBinding(Action::B, ActionType::JOY_BUTTON, JOY_XBOX_B);

  // Select
  addBinding(Action::SELECT, ActionType::KEY, ALLEGRO_KEY_ENTER);
  addBinding(Action::SELECT, ActionType::JOY_BUTTON, JOY_XBOX_A);
  addBinding(Action::SELECT, ActionType::JOY_BUTTON, JOY_XBOX_START);
}
