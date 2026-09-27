/**
 * Action Binder
 * Danny Van Stemp and Allan Legemaate
 * Abstraction layer on top of key/controller
 *   codes for keybindings
 * 05/05/2017
 **/

#pragma once

enum class Action { Left, Right, Up, Down, A, B, Select };

namespace ActionBinder {

// True on the tick an action starts
bool actionBegun(Action action);

// True while an action is held
bool actionHeld(Action action);

}  // namespace ActionBinder
