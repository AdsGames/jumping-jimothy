#include "LevelSelect.h"

#include <format>

#include "../Globals.h"
#include "../ui/Label.h"
#include "../util/ActionBinder.h"
#include "../util/Config.h"

namespace {
constexpr float COLUMN_X = 340;
constexpr float COLUMN_WIDTH = 300;
constexpr float ROW_HEIGHT = 18;
constexpr float FIRST_ROW_Y = 66;
constexpr float ROW_SPACING = 42;

const asw::Color WHITE(255, 255, 255);
}  // namespace

void LevelSelect::init() {
  font = asw::assets::load_font("assets/fonts/munro.ttf", 24,
                                asw::FontStyle::Pixel);
  font_large = asw::assets::load_font("assets/fonts/munro.ttf", 48,
                                      asw::FontStyle::Pixel);

  ui.clear();
  level_buttons.clear();

  ui.add<Label>(375, 5, "Select a level", font_large).setTextColour(WHITE);

  // Buttons in a white outlined column
  auto add_column_button = [this](float y, const std::string& text) {
    auto& button = ui.add<Button>(COLUMN_X, y, text, font);
    button.setSize(COLUMN_WIDTH, ROW_HEIGHT);
    button.setTextJustification(TextJustify::Center);
    return &button;
  };

  btn_back = add_column_button(FIRST_ROW_Y, "Back to main menu");
  btn_back->setCellFillTransparent(true);
  btn_back->setTextColour(WHITE);

  for (int level = 1; level <= LEVEL_COUNT; level++) {
    auto* button = add_column_button(
        FIRST_ROW_Y + (ROW_SPACING * static_cast<float>(level)),
        std::format("Level {}", level));

    if (Config::getBool(std::format("level_{}_completed", level))) {
      button->setBackgroundColour(asw::Color(0, 200, 0));
      button->setTextColour(WHITE);
    }

    level_buttons.push_back(button);
  }

  btn_reset = add_column_button(
      FIRST_ROW_Y + (ROW_SPACING * static_cast<float>(LEVEL_COUNT + 1)),
      "Reset Save Game");
  btn_reset->setCellFillTransparent(true);
  btn_reset->setTextColour(WHITE);

  btn_really_reset = &ui.add<Button>(700, 651, "Really reset?", font);
  btn_really_reset->setSize(180, ROW_HEIGHT);

  btn_cancel = &ui.add<Button>(700, 696, "Cancel", font);
  btn_cancel->setSize(180, ROW_HEIGHT);

  showResetConfirm(false);
}

void LevelSelect::showResetConfirm(bool show) {
  btn_really_reset->setVisible(show);
  btn_cancel->setVisible(show);
}

void LevelSelect::tick() {
  ui.update();

  if (ActionBinder::actionBegun(Action::B) || btn_back->clicked()) {
    manager.set_next_scene(ProgramState::Menu);
    return;
  }

  if (btn_reset->clicked()) {
    showResetConfirm(true);
  }

  if (btn_really_reset->clicked()) {
    for (int level = 0; level <= LEVEL_COUNT; level++) {
      Config::setBool(std::format("level_{}_completed", level), false);
    }
    Config::save();

    // Reload to show the cleared progress
    manager.set_next_scene(ProgramState::LevelSelect);
    return;
  }

  if (btn_cancel->clicked()) {
    showResetConfirm(false);
  }

  for (std::size_t i = 0; i < level_buttons.size(); i++) {
    if (level_buttons[i]->clicked()) {
      session.level_to_start = static_cast<int>(i) + 1;
      session.editing_level = false;
      manager.set_next_scene(ProgramState::Game);
      return;
    }
  }
}

void LevelSelect::draw() {
  asw::draw::clear_color(asw::Color(75, 75, 100));
  ui.draw();
}
