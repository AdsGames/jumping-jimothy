/**
 * Game UI
 * Allan Legemaate
 * The game's look and controls for asw::ui
 * 28/09/2026
 **/

#pragma once

#include <asw/asw.h>
#include <cstdint>
#include <string>

namespace GameUi {

// Action name that is never bound, for navigation steps to turn off
inline constexpr const char* NO_ACTION = "ui_none";

// Read the game's actions for navigation and give widgets the game's look:
//   grey buttons with a black border and black text, brighter when hovered
//   or focused, and a grey checkbox row with the box on the right
void setup(asw::ui::Root& ui, const asw::Font& font);

// Take only the mouse, for screens where the game's actions play the game
void pointerOnly(asw::ui::Root& ui);

// Button look with a fill, text colour and alpha for all of it
asw::ui::ButtonStyle buttonStyle(asw::Color bg, asw::Color text,
                                 uint8_t alpha = 255);

// Button at x, y. Width and height exclude the 10 pixel padding, zero fits the
// text of the theme font
asw::ui::Button& addButton(asw::ui::Root& ui, float x, float y,
                           const std::string& text, float width = 0,
                           float height = 0);

// No fill, black border and white text
void setOutline(asw::ui::Button& button);

// Text with its top left at x, y
asw::ui::Label& addLabel(asw::ui::Root& ui, float x, float y,
                         const std::string& text, const asw::Font& font,
                         asw::Color color);

// Width and height of text in the font, with padding on each side
asw::Vec2<float> fitText(const asw::Font& font, const std::string& text,
                         float padding);

}  // namespace GameUi
