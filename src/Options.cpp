#include <allegro5/allegro_ttf.h>

#include "Options.h"
#include "DisplayMode.h"
#include "Config.h"
#include "MusicManager.h"

#include "KeyListener.h"
#include "JoystickListener.h"
#include "MouseListener.h"

#include "ui/Button.h"
#include "ui/Label.h"
#include "ui/CheckBox.h"

#include "Tools.h"

// Initialize options screen
Options::Options() {
  // Load fonts
  options_font = al_load_ttf_font( "fonts/munro.ttf", 18, 0);
  title_font = al_load_ttf_font( "fonts/munro.ttf", 36, 0);

  // Load images
  cursor = tools::load_bitmap_ex("images/cursor.png");
  highlight = tools::load_bitmap_ex("images/highlight.png");

  // Options text
  OptionsUI.addElement(new Label(25, 25, "Options", "options", title_font));
  OptionsUI.getElementByText("Options") -> setTextColour(al_map_rgb(255, 255, 255));

  // Joystick data
  OptionsUI.addElement(new Label(400, 25, "Gamepad: " + Config::joystick_data, "joydata", options_font));
  OptionsUI.getElementById("joydata") -> setTextColour(al_map_rgb(255, 255, 255));

  // SFX toggle button
  OptionsUI.addElement(new CheckBox(100, 101, "SFX Enabled", "chkSFX", options_font));
  OptionsUI.getElementById("chkSFX") -> setSize(180, 18);
  OptionsUI.getElementById("chkSFX") -> setTextColour(al_map_rgb(255, 255, 255));
  OptionsUI.getElementById("chkSFX") -> setVisibleBackground(false);
  dynamic_cast<CheckBox*>(OptionsUI.getElementById("chkSFX")) -> setChecked(Config::sfx_enabled);

  // Music toggle button
  OptionsUI.addElement(new CheckBox(100, 151, "Music Enabled", "chkMusic", options_font));
  OptionsUI.getElementById("chkMusic") -> setSize(180, 18);
  OptionsUI.getElementById("chkMusic") -> setTextColour(al_map_rgb(255, 255, 255));
  OptionsUI.getElementById("chkMusic") -> setVisibleBackground(false);
  dynamic_cast<CheckBox*>(OptionsUI.getElementById("chkMusic")) -> setChecked(Config::music_enabled);

  // Graphics mode button
  OptionsUI.addElement(new Button(100, 201, "Graphics Mode", "btnGraphicsMode", options_font));
  OptionsUI.getElementByText("Graphics Mode") -> setSize(180, 18);
  OptionsUI.getElementByText("Graphics Mode") -> setTextColour(al_map_rgb(255, 255, 255));
  OptionsUI.getElementByText("Graphics Mode") -> setVisibleBackground(false);

  // Graphics label
  OptionsUI.addElement(new Label(300, 200, DisplayMode::getDisplayModeString(), "graphicsdata", options_font));
  OptionsUI.getElementById("graphicsdata") -> setVisibleBackground(false);
  OptionsUI.getElementById("graphicsdata") -> setTextColour(al_map_rgb(255, 255, 255));

  // Apply graphics
  OptionsUI.addElement(new Button(100, 251, "Apply Graphics", "btnApplyGFX", options_font));
  OptionsUI.getElementByText("Apply Graphics") -> setSize(180, 18);
  OptionsUI.getElementByText("Apply Graphics") -> setTextColour(al_map_rgb(255, 255, 255));
  OptionsUI.getElementByText("Apply Graphics") -> setVisibleBackground(false);

  // Back
  OptionsUI.addElement(new Button(100, 301, "Back", "btnBack", options_font));
  OptionsUI.getElementByText("Back") -> setSize(180, 18);
  OptionsUI.getElementByText("Back") -> setTextColour(al_map_rgb(255, 255, 255));
  OptionsUI.getElementByText("Back") -> setVisibleBackground(false);

  // Temp graphics mode
  temp_graphics_mode = DisplayMode::getDisplayMode();
}

// Destructor
Options::~Options() {

}

// Draw options menu
void Options::draw(){
  // Background colour
  al_clear_to_color(al_map_rgb(50, 50, 50));

  // Draw ui elements
  OptionsUI.draw();

  // Draw cursor if enabled
  if (Config::draw_cursor) {
    al_draw_bitmap(cursor, MouseListener::mouse_x, MouseListener::mouse_y, 0);
  }

  // Draw highlight
  al_draw_bitmap(highlight, 100, highlight_y, 0);
}

// Update options UI
void Options::update(){
  // Update UI
  OptionsUI.update();

  // Back button pressed
  if (KeyListener::keyPressed[ALLEGRO_KEY_ESCAPE] || OptionsUI.getElementByText("Back") -> clicked() ||
     (JoystickListener::buttonReleased[JOY_XBOX_A] && highlight_y_destination == 350) || JoystickListener::buttonReleased[JOY_XBOX_B]) {
    set_next_state(STATE_MENU);

    // Save settings
    Config::write_data("data/options_data.xml");
  }

  // SFX checkbox
  Config::sfx_enabled = dynamic_cast<CheckBox*>(OptionsUI.getElementById("chkSFX")) -> getChecked();

  // Music checkbox
  Config::music_enabled = dynamic_cast<CheckBox*>(OptionsUI.getElementById("chkMusic")) -> getChecked();

  // Toggle Music pressed
  if (dynamic_cast<CheckBox*>(OptionsUI.getElementById("chkMusic")) -> getToggled()) {
    if (Config::music_enabled) {
      MusicManager::menu_music.play();
    }
    else {
      MusicManager::menu_music.stop();
    }
  }

  // Cycle graphics mode pressed
  if(OptionsUI.getElementByText("Graphics Mode") &&
     OptionsUI.getElementByText("Graphics Mode") -> clicked()) {
    temp_graphics_mode = (temp_graphics_mode + 1) % NUM_GRAPHICS_MODES;
  }

  // Apply graphics pressed
  if(OptionsUI.getElementByText("Apply Graphics") &&
     OptionsUI.getElementByText("Apply Graphics") -> clicked()) {
    DisplayMode::setMode(temp_graphics_mode);
  }

  // Button highlights
  if (OptionsUI.getElementById("chkSFX") -> hover() && !Config::joystick_mode) {
    highlight_y_destination = 100;
  }
  if (OptionsUI.getElementById("chkMusic") -> hover() && !Config::joystick_mode) {
    highlight_y_destination = 150;
  }
  if (OptionsUI.getElementByText("Graphics Mode") &&
      OptionsUI.getElementByText("Graphics Mode") -> hover() && !Config::joystick_mode) {
    highlight_y_destination = 200;
  }
  if (OptionsUI.getElementByText("Apply Graphics") &&
      OptionsUI.getElementByText("Apply Graphics") -> hover() && !Config::joystick_mode) {
    highlight_y_destination = 250;
  }
  if (OptionsUI.getElementByText("Back") &&
      OptionsUI.getElementByText("Back") -> hover() && !Config::joystick_mode) {
    highlight_y_destination = 300;
  }

  // Move highlighter
  if (highlight_y > highlight_y_destination)highlight_y -= 10;
  if (highlight_y < highlight_y_destination)highlight_y += 10;

  // Joystick control
  if ((JoystickListener::stickDirections[LEFT_STICK_UP] ||
       JoystickListener::stickDirections[DPAD_UP2]) &&
      !joystick_direction_hit) {
    if(highlight_y_destination < 300)
      highlight_y_destination += 50;
  }

  if ((JoystickListener::stickDirections[LEFT_STICK_DOWN] ||
       JoystickListener::stickDirections[DPAD_DOWN])  &&
      !joystick_direction_hit) {
    if(highlight_y_destination > 100)
      highlight_y_destination -= 50;
  }

  if (JoystickListener::stickDirections[LEFT_STICK_DOWN] || JoystickListener::stickDirections[LEFT_STICK_UP] || JoystickListener::stickDirections[DPAD_DOWN] || JoystickListener::stickDirections[DPAD_UP2]){
    joystick_direction_hit=true;
    Config::joystick_mode=true;
  }
  else {
    joystick_direction_hit = false;
  }

  if (MouseListener::mouse_moved) {
    Config::joystick_mode = false;
  }

  // Keep UI in sync
  updateUI();
}

void Options::updateUI() {
  // Graphics Mode
  if (OptionsUI.getElementById("graphicsdata")) {
    OptionsUI.getElementById("graphicsdata") -> setText(DisplayMode::getDisplayModeString(temp_graphics_mode));
  }

  // Joystick config
  if (OptionsUI.getElementById("joydata")) {
    OptionsUI.getElementById("joydata") -> setText("Gamepad: "+ Config::joystick_data);
  }
}

