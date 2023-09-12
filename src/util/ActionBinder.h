/**
 * Action Binder
 * Danny Van Stemp and Allan Legemaate
 * Abstraction layer on top of key/joy
 *   codes for keybindings
 * 05/05/2017
 **/

#ifndef ACTIONBINDER_H
#define ACTIONBINDER_H

#include <vector>

const int BINDING_NONE = -1;

enum class Action { NONE, LEFT, RIGHT, UP, DOWN, A, B, SELECT };

enum class ActionType { NONE, KEY, JOY_STICK, JOY_BUTTON };

class Binding {
 public:
  Binding(Action action, ActionType type, int code)
      : action(action), type(type), code(code) {}

  Binding() : Binding(Action::NONE, ActionType::NONE, BINDING_NONE) {}
  virtual ~Binding() = default;

  Action getAction() const { return action; }
  ActionType getType() const { return type; }
  int getCode() const { return code; }

 private:
  Action action;
  ActionType type;
  int code;
};

class ActionBinder {
 public:
  ActionBinder() = default;
  virtual ~ActionBinder() = default;

  static bool actionBegun(const Action action);
  static bool actionEnded(const Action action) { return false; };
  static bool actionHeld(const Action action);

  static void addBinding(Action action, ActionType type, int code);

  static void setDefaults();

 private:
  static std::vector<Binding*> bindings;
  static std::vector<Binding*> findBindings(const Action action);
};

#endif  // ACTIONBINDER_H
