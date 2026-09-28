#include "Controls.h"

#include <asw/asw.h>

namespace {
using asw::input::ControllerAxis;
using asw::input::ControllerAxisBinding;
using asw::input::ControllerButton;
using asw::input::ControllerButtonBinding;
using asw::input::Key;
using asw::input::KeyBinding;

// How far a stick must move, after the dead zone, to count as pressed
constexpr float STICK_THRESHOLD = 0.5F;

void bindKey(std::string_view action, Key key) {
  asw::input::bind_action(action, KeyBinding{key});
}

void bindButton(std::string_view action, ControllerButton button) {
  asw::input::bind_action(
      action, ControllerButtonBinding{button, asw::input::ANY_CONTROLLER});
}

void bindStick(std::string_view action, ControllerAxis axis, bool positive) {
  asw::input::bind_action(
      action, ControllerAxisBinding{axis, asw::input::ANY_CONTROLLER,
                                    STICK_THRESHOLD, positive});
}
}  // namespace

void Controls::bind() {
  asw::input::clear_actions();

  bindKey(LEFT, Key::A);
  bindKey(LEFT, Key::Left);
  bindStick(LEFT, ControllerAxis::LeftX, false);
  bindButton(LEFT, ControllerButton::DPadLeft);

  bindKey(RIGHT, Key::D);
  bindKey(RIGHT, Key::Right);
  bindStick(RIGHT, ControllerAxis::LeftX, true);
  bindButton(RIGHT, ControllerButton::DPadRight);

  bindKey(UP, Key::Up);
  bindKey(UP, Key::W);
  bindStick(UP, ControllerAxis::LeftY, false);
  bindButton(UP, ControllerButton::DPadUp);

  bindKey(DOWN, Key::Down);
  bindKey(DOWN, Key::S);
  bindStick(DOWN, ControllerAxis::LeftY, true);
  bindButton(DOWN, ControllerButton::DPadDown);

  bindKey(JUMP, Key::W);
  bindButton(JUMP, ControllerButton::A);

  bindKey(FREEZE, Key::Space);
  bindButton(FREEZE, ControllerButton::B);

  bindKey(SELECT, Key::Return);
  bindButton(SELECT, ControllerButton::A);
  bindButton(SELECT, ControllerButton::Start);
}
