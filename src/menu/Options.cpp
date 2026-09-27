#include "Options.h"

#include "../util/Audio.h"
#include "../util/Config.h"
#include "../util/Input.h"

namespace {
constexpr float ROW_X = 100;
constexpr float ROW_WIDTH = 180;
constexpr float ROW_HEIGHT = 18;

const asw::Color WHITE(255, 255, 255);

std::string gamepadText() {
  if (asw::input::get_controller_count() == 0) {
    return "Gamepad: None detected.";
  }

  return "Gamepad: " + asw::input::get_controller_name(0);
}
}  // namespace

void Options::init() {
  options_font = asw::assets::load_font("assets/fonts/munro.ttf", 18,
                                        asw::FontStyle::Pixel);
  title_font = asw::assets::load_font("assets/fonts/munro.ttf", 36,
                                      asw::FontStyle::Pixel);

  ui.clear();

  ui.add<Label>(25, 25, "Options", title_font).setTextColour(WHITE);

  lbl_gamepad = &ui.add<Label>(400, 25, gamepadText(), options_font);
  lbl_gamepad->setTextColour(WHITE);

  auto add_checkbox = [this](float y, const std::string& text, bool checked) {
    auto& checkbox = ui.add<CheckBox>(ROW_X, y, text, options_font);
    checkbox.setSize(ROW_WIDTH, ROW_HEIGHT);
    checkbox.setChecked(checked);
    return &checkbox;
  };

  chk_sfx = add_checkbox(101, "SFX Enabled", Config::getBool("sfx_enabled"));
  chk_music =
      add_checkbox(151, "Music Enabled", Config::getBool("music_enabled"));
  chk_fullscreen =
      add_checkbox(201, "Fullscreen", Config::getBool("fullscreen"));

  btn_back = &ui.add<Button>(ROW_X, 251, "Back", options_font);
  btn_back->setSize(ROW_WIDTH, ROW_HEIGHT);
}

void Options::tick() {
  ui.update();

  if (chk_sfx->getToggled()) {
    Audio::setSfxEnabled(chk_sfx->getChecked());
  }

  if (chk_music->getToggled()) {
    Audio::setMusicEnabled(chk_music->getChecked());
  }

  if (chk_fullscreen->getToggled()) {
    Config::setBool("fullscreen", chk_fullscreen->getChecked());
    asw::display::set_fullscreen(chk_fullscreen->getChecked());
  }

  // Controllers can be plugged in while the menu is open
  lbl_gamepad->setText(gamepadText());

  if (input::keyPressed(asw::input::Key::Escape) || btn_back->clicked()) {
    Config::save();
    manager.set_next_scene(ProgramState::Menu);
  }
}

void Options::draw() {
  asw::draw::clear_color(asw::Color(50, 50, 50));
  ui.draw();
}
