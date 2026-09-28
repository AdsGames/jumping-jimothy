#include "Options.h"

#include <functional>
#include <utility>

#include "../ui/GameUi.h"
#include "../util/Audio.h"
#include "../util/Config.h"

namespace {
constexpr float ROW_X = 100;
constexpr float ROW_WIDTH = 180;
constexpr float ROW_HEIGHT = 18;

// Padding of a checkbox row, the box fills its height
constexpr float ROW_PADDING = 9;

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

  ui = asw::ui::Root();
  GameUi::setup(ui, options_font);
  ui.on_back = [this] { back(); };

  GameUi::addLabel(ui, 45, 35, "Options", title_font, WHITE);
  lbl_gamepad = &GameUi::addLabel(ui, 420, 35, gamepadText(), options_font,
                                  WHITE);

  auto add_checkbox = [this](float y, const std::string& text, bool checked,
                             std::function<void(bool)> on_change) {
    auto& checkbox = ui.root.add_child<asw::ui::Checkbox>();
    checkbox.transform = asw::Quad<float>(ROW_X, y, ROW_WIDTH + 20,
                                          ROW_HEIGHT + 20);
    checkbox.padding = ROW_PADDING;
    checkbox.text = text;
    checkbox.checked = checked;
    checkbox.on_change = std::move(on_change);
  };

  add_checkbox(101, "SFX Enabled", Config::getBool("sfx_enabled"),
               [](bool checked) { Audio::setSfxEnabled(checked); });
  add_checkbox(151, "Music Enabled", Config::getBool("music_enabled"),
               [](bool checked) { Audio::setMusicEnabled(checked); });
  add_checkbox(201, "Fullscreen", Config::getBool("fullscreen"),
               [](bool checked) {
                 Config::setBool("fullscreen", checked);
                 asw::display::set_fullscreen(checked);
               });

  GameUi::addButton(ui, ROW_X, 251, "Back", ROW_WIDTH, ROW_HEIGHT).on_click =
      [this] { back(); };
}

void Options::back() {
  Config::save();
  manager.set_next_scene(ProgramState::Menu);
}

void Options::update(float /*dt*/) {
  // Controllers can be plugged in while the menu is open
  lbl_gamepad->text = gamepadText();

  ui.update();
}

void Options::draw() {
  asw::draw::clear_color(asw::Color(50, 50, 50));
  ui.draw();
}
