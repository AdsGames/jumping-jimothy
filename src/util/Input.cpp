#include "Input.h"

#include <array>

namespace {
using asw::input::ControllerAxis;
using asw::input::ControllerButton;
using asw::input::Key;
using asw::input::MouseButton;

// How far a stick must move to count as held
constexpr float STICK_TOLERANCE = 0.6F;

// Only the first controller is used
constexpr uint32_t CONTROLLER = 0;

constexpr int NUM_STICK_DIRECTIONS = 4;

std::array<bool, asw::input::NUM_KEYS> keys_pressed{};
std::array<bool, asw::input::NUM_MOUSE_BUTTONS> mouse_pressed{};
std::array<bool, asw::input::NUM_MOUSE_BUTTONS> mouse_released{};
std::array<bool, asw::input::NUM_CONTROLLER_BUTTONS> buttons_pressed{};
std::array<bool, NUM_STICK_DIRECTIONS> sticks_last_tick{};
bool any_key_pressed = false;
bool mouse_moved = false;

template <typename T>
auto index(T value) {
  return static_cast<std::size_t>(value);
}
}  // namespace

void input::poll() {
  const auto& keyboard = asw::input::get_keyboard();
  for (std::size_t i = 0; i < keys_pressed.size(); i++) {
    keys_pressed[i] = keys_pressed[i] || keyboard.pressed[i];
    any_key_pressed = any_key_pressed || keyboard.pressed[i];
  }

  const auto& mouse = asw::input::get_mouse();
  for (std::size_t i = 0; i < mouse_pressed.size(); i++) {
    mouse_pressed[i] = mouse_pressed[i] || mouse.pressed[i];
    mouse_released[i] = mouse_released[i] || mouse.released[i];
  }
  mouse_moved = mouse_moved || mouse.change.x != 0.0F || mouse.change.y != 0.0F;

  for (std::size_t i = 0; i < buttons_pressed.size(); i++) {
    buttons_pressed[i] =
        buttons_pressed[i] ||
        asw::input::get_controller_button_down(
            CONTROLLER, static_cast<ControllerButton>(i));
  }
}

void input::endTick() {
  keys_pressed.fill(false);
  mouse_pressed.fill(false);
  mouse_released.fill(false);
  buttons_pressed.fill(false);
  any_key_pressed = false;
  mouse_moved = false;

  for (int i = 0; i < NUM_STICK_DIRECTIONS; i++) {
    sticks_last_tick[i] = stickHeld(static_cast<Stick>(i));
  }
}

bool input::keyPressed(Key key) {
  return keys_pressed[index(key)];
}

bool input::keyHeld(Key key) {
  return asw::input::get_key(key);
}

bool input::anyKeyPressed() {
  return any_key_pressed;
}

bool input::mousePressed(MouseButton button) {
  return mouse_pressed[index(button)];
}

bool input::mouseReleased(MouseButton button) {
  return mouse_released[index(button)];
}

bool input::mouseHeld(MouseButton button) {
  return asw::input::get_mouse_button(button);
}

bool input::mouseMoved() {
  return mouse_moved;
}

asw::Vec2<float> input::mousePosition() {
  return asw::input::get_mouse().position;
}

void input::consumeMouseRelease(MouseButton button) {
  mouse_released[index(button)] = false;
}

bool input::buttonPressed(ControllerButton button) {
  return buttons_pressed[index(button)];
}

bool input::buttonHeld(ControllerButton button) {
  return asw::input::get_controller_button(CONTROLLER, button);
}

bool input::stickMoved(Stick direction) {
  return stickHeld(direction) && !sticks_last_tick[index(direction)];
}

bool input::stickHeld(Stick direction) {
  switch (direction) {
    case Stick::Left:
      return asw::input::get_controller_axis(
                 CONTROLLER, ControllerAxis::LeftX) < -STICK_TOLERANCE;
    case Stick::Right:
      return asw::input::get_controller_axis(
                 CONTROLLER, ControllerAxis::LeftX) > STICK_TOLERANCE;
    case Stick::Up:
      return asw::input::get_controller_axis(
                 CONTROLLER, ControllerAxis::LeftY) < -STICK_TOLERANCE;
    case Stick::Down:
      return asw::input::get_controller_axis(
                 CONTROLLER, ControllerAxis::LeftY) > STICK_TOLERANCE;
  }

  return false;
}
