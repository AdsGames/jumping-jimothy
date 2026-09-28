/**
 * Controls
 * Danny Van Stemp and Allan Legemaate
 * Names of the game's input actions and their default bindings
 * 05/05/2017
 **/

#pragma once

#include <string>
#include <string_view>

namespace Controls {

inline constexpr std::string_view LEFT = "left";
inline constexpr std::string_view RIGHT = "right";
inline constexpr std::string_view UP = "up";
inline constexpr std::string_view DOWN = "down";
inline constexpr std::string_view JUMP = "jump";
inline constexpr std::string_view FREEZE = "freeze";
inline constexpr std::string_view SELECT = "select";
inline constexpr std::string_view BACK = "back";
inline constexpr std::string_view RESTART = "restart";

// Register the default bindings with asw, call once at start up
void bind();

// Replace {jump}, {freeze}, {restart} and {move} in text with the key or
// button names of the device the player used last
std::string describe(const std::string& text);

}  // namespace Controls
