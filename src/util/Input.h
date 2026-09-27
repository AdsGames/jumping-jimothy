/**
 * Input
 * Allan Legemaate
 * Wraps asw input for fixed ticks. Presses are held until a tick reads them,
 *   so none are lost or doubled when a frame runs zero or several ticks.
 * 27/09/2026
 **/

#pragma once

#include <asw/asw.h>

namespace input {

// Directions of the left stick
enum class Stick { Left, Right, Up, Down };

// Collect the presses of this frame, call once per frame
void poll();

// Clear the presses read by a tick, call after each tick
void endTick();

// Keyboard
bool keyPressed(asw::input::Key key);
bool keyHeld(asw::input::Key key);
bool anyKeyPressed();

// Mouse
bool mousePressed(asw::input::MouseButton button);
bool mouseReleased(asw::input::MouseButton button);
bool mouseHeld(asw::input::MouseButton button);
bool mouseMoved();
asw::Vec2<float> mousePosition();

// Mark a mouse release as used so no other element reacts to it
void consumeMouseRelease(asw::input::MouseButton button);

// First game controller
bool buttonPressed(asw::input::ControllerButton button);
bool buttonHeld(asw::input::ControllerButton button);
bool stickMoved(Stick direction);
bool stickHeld(Stick direction);

}  // namespace input
