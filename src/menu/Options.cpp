#include "menu/Options.h"

#include <allegro5/allegro_ttf.h>

#include "util/DisplayMode.h"
#include "util/Config.h"
#include "util/MusicManager.h"

#include "util/KeyListener.h"

#include "ui/Button.h"
#include "ui/Label.h"
#include "ui/CheckBox.h"

// Initialize options screen
Options::Options() {
  // Load fonts
  options_font = al_load_ttf_font("fonts/munro.ttf", 18, 0);
  title_font = al_load_ttf_font("fonts/munro.ttf", 36, 0);

  // Options text
  OptionsUI.addElement(new Label(25, 25, "Options", "lblOptions", title_font));
  OptionsUI.getElementById("lblOptions") -> setTextColour(al_map_rgb(255, 255, 255));

  // Joystick data
  OptionsUI.addElement(new Label(400, 25, "Gamepad: " + Config::getValue("joystick_data"), "lblJoydata", options_font));
  OptionsUI.getElementById("lblJoydata") -> setTextColour(al_map_rgb(255, 255, 255));

  // SFX toggle button
  OptionsUI.addElement(new CheckBox(100, 101, "SFX Enabled", "chkSFX", options_font));
  OptionsUI.getElementById("chkSFX") -> setSize(180, 18);
  //OptionsUI.getElementById("chkSFX") -> setTextColour(al_map_rgb(255, 255, 255));
  //OptionsUI.getElementById("chkSFX") -> setVisibleBackground(false);
  dynamic_cast<CheckBox*>(OptionsUI.getElementById("chkSFX")) -> setChecked(Config::getBooleanValue("sfx_enabled"));

  // Music toggle button
  OptionsUI.addElement(new CheckBox(100, 151, "Music Enabled", "chkMusic", options_font));
  OptionsUI.getElementById("chkMusic") -> setSize(180, 18);
  //OptionsUI.getElementById("chkMusic") -> setTextColour(al_map_rgb(255, 255, 255));
  //OptionsUI.getElementById("chkMusic") -> setVisibleBackground(false);
  dynamic_cast<CheckBox*>(OptionsUI.getElementById("chkMusic")) -> setChecked(Config::getBooleanValue("music_enabled"));

  // Graphics mode button
  OptionsUI.addElement(new Button(100, 201, "Graphics Mode", "btnGraphicsMode", options_font));
  OptionsUI.getElementById("btnGraphicsMode") -> setSize(180, 18);
  //OptionsUI.getElementById("btnGraphicsMode") -> setTextColour(al_map_rgb(255, 255, 255));
  //OptionsUI.getElementById("btnGraphicsMode") -> setVisibleBackground(false);

  // Graphics label
  OptionsUI.addElement(new Label(300, 200, DisplayMode::getDisplayModeString(), "lblGraphicsMode", options_font));
  OptionsUI.getElementById("lblGraphicsMode") -> setTextColour(al_map_rgb(255, 255, 255));

  // Apply graphics
  OptionsUI.addElement(new Button(100, 251, "Apply Graphics", "btnApplyGFX", options_font));
  OptionsUI.getElementById("btnApplyGFX") -> setSize(180, 18);
  //OptionsUI.getElementById("btnApplyGFX") -> setTextColour(al_map_rgb(255, 255, 255));
  //OptionsUI.getElementById("btnApplyGFX") -> setVisibleBackground(false);

  // Back
  OptionsUI.addElement(new Button(100, 301, "Back", "btnBack", options_font));
  OptionsUI.getElementById("btnBack") -> setSize(180, 18);
  //OptionsUI.getElementById("btnBack") -> setTextColour(al_map_rgb(255, 255, 255));
  //OptionsUI.getElementById("btnBack") -> setVisibleBackground(false);

  // Temp graphics mode
  temp_graphics_mode = DisplayMode::getDisplayMode();
}

// Destructor
Options::~Options() {
  al_destroy_font(options_font);
  al_destroy_font(title_font);
}

// Draw options menu
void Options::draw(){
  // Background colour
  al_clear_to_color(al_map_rgb(50, 50, 50));

  // Draw ui elements
  OptionsUI.draw();
}

// Update options UI
void Options::update(){
  // Update UI
  OptionsUI.update();

  // Back button pressed
  if (KeyListener::keyPressed[ALLEGRO_KEY_ESCAPE] || OptionsUI.getElementById("btnBack") -> clicked()) {
    set_next_state(STATE_MENU);

    // Save settings
    Config::writeFile("data/config.xml");
  }

  // SFX checkbox
  Config::setValue("sfx_enabled", dynamic_cast<CheckBox*>(OptionsUI.getElementById("chkSFX")) -> getChecked());

  // Music checkbox
  Config::setValue("music_enabled", dynamic_cast<CheckBox*>(OptionsUI.getElementById("chkMusic")) -> getChecked());

  // Toggle Music pressed
  if (dynamic_cast<CheckBox*>(OptionsUI.getElementById("chkMusic")) -> getToggled()) {
    if (Config::getBooleanValue("music_enabled")) {
      MusicManager::menu_music -> play();
    }
    else {
      MusicManager::menu_music -> stop();
    }
  }

  // Cycle graphics mode pressed
  if(OptionsUI.getElementById("btnGraphicsMode") &&
     OptionsUI.getElementById("btnGraphicsMode") -> clicked()) {
    temp_graphics_mode = (temp_graphics_mode + 1) % DisplayMode::NUM_GRAPHICS_MODES;
    OptionsUI.getElementById("lblGraphicsMode") -> setText(DisplayMode::getDisplayModeString(temp_graphics_mode));
  }

  // Apply graphics pressed
  if(OptionsUI.getElementById("btnApplyGFX") &&
     OptionsUI.getElementById("btnApplyGFX") -> clicked()) {
    DisplayMode::setMode(temp_graphics_mode);
  }

  // Keep UI in sync
  updateUI();
}

void Options::updateUI() {
  // Joystick config
  if (OptionsUI.getElementById("lblJoydata")) {
    OptionsUI.getElementById("lblJoydata") -> setText("Gamepad: "+ Config::getValue("joystick_data"));
  }
}

