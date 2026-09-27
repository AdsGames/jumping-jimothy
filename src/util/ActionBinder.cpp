#include "ActionBinder.h"

#include <array>
#include <variant>

#include "Input.h"

namespace {
using asw::input::ControllerButton;
using asw::input::Key;
using input::Stick;

struct Binding {
  Action action;
  std::variant<Key, ControllerButton, Stick> code;
};

const std::array BINDINGS{
    Binding{Action::Left, Key::A},
    Binding{Action::Left, Key::Left},
    Binding{Action::Left, Stick::Left},
    Binding{Action::Left, ControllerButton::DPadLeft},

    Binding{Action::Right, Key::D},
    Binding{Action::Right, Key::Right},
    Binding{Action::Right, Stick::Right},
    Binding{Action::Right, ControllerButton::DPadRight},

    Binding{Action::Up, Key::Up},
    Binding{Action::Up, Key::W},
    Binding{Action::Up, Stick::Up},
    Binding{Action::Up, ControllerButton::DPadUp},

    Binding{Action::Down, Key::Down},
    Binding{Action::Down, Key::S},
    Binding{Action::Down, Stick::Down},
    Binding{Action::Down, ControllerButton::DPadDown},

    Binding{Action::A, Key::W},
    Binding{Action::A, ControllerButton::A},

    Binding{Action::B, Key::Space},
    Binding{Action::B, ControllerButton::B},

    Binding{Action::Select, Key::Return},
    Binding{Action::Select, ControllerButton::A},
    Binding{Action::Select, ControllerButton::Start},
};

bool begun(Key key) {
  return input::keyPressed(key);
}

bool begun(ControllerButton button) {
  return input::buttonPressed(button);
}

bool begun(Stick direction) {
  return input::stickMoved(direction);
}

bool held(Key key) {
  return input::keyHeld(key);
}

bool held(ControllerButton button) {
  return input::buttonHeld(button);
}

bool held(Stick direction) {
  return input::stickHeld(direction);
}
}  // namespace

bool ActionBinder::actionBegun(Action action) {
  for (const auto& binding : BINDINGS) {
    if (binding.action == action &&
        std::visit([](auto code) { return begun(code); }, binding.code)) {
      return true;
    }
  }

  return false;
}

bool ActionBinder::actionHeld(Action action) {
  for (const auto& binding : BINDINGS) {
    if (binding.action == action &&
        std::visit([](auto code) { return held(code); }, binding.code)) {
      return true;
    }
  }

  return false;
}
