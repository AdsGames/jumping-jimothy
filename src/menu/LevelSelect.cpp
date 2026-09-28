#include "LevelSelect.h"

#include <format>

#include "../Globals.h"
#include "../ui/GameUi.h"
#include "../util/Config.h"

namespace {
constexpr float COLUMN_X = 340;
constexpr float COLUMN_WIDTH = 300;
constexpr float ROW_HEIGHT = 18;
constexpr float FIRST_ROW_Y = 66;
constexpr float ROW_SPACING = 42;

const asw::Color GREEN(0, 200, 0);
const asw::Color WHITE(255, 255, 255);
}  // namespace

void LevelSelect::init() {
  font = asw::assets::load_font("assets/fonts/munro.ttf", 24,
                                asw::FontStyle::Pixel);
  font_large = asw::assets::load_font("assets/fonts/munro.ttf", 48,
                                      asw::FontStyle::Pixel);

  ui = asw::ui::Root();
  GameUi::setup(ui, font);
  ui.on_back = [this] { manager.set_next_scene(ProgramState::Menu); };

  GameUi::addLabel(ui, 395, 15, "Select a level", font_large, WHITE);

  // Buttons in a white outlined column
  auto add_column_button = [this](int row, const std::string& text) -> auto& {
    auto& button = GameUi::addButton(
        ui, COLUMN_X, FIRST_ROW_Y + (ROW_SPACING * static_cast<float>(row)),
        text, COLUMN_WIDTH, ROW_HEIGHT);
    auto style = ui.ctx.theme.button;
    style.text_align = asw::TextJustify::Center;
    button.style = style;
    return button;
  };

  auto& btn_back = add_column_button(0, "Back to main menu");
  GameUi::setOutline(btn_back);
  btn_back.style->text_align = asw::TextJustify::Center;
  btn_back.on_click = [this] { manager.set_next_scene(ProgramState::Menu); };

  for (int level = 1; level <= LEVEL_COUNT; level++) {
    auto& button = add_column_button(level, std::format("Level {}", level));

    if (Config::getBool(std::format("level_{}_completed", level))) {
      button.style = GameUi::buttonStyle(GREEN, WHITE);
      button.style->text_align = asw::TextJustify::Center;
    }

    button.on_click = [this, level] {
      session.level_to_start = level;
      session.editing_level = false;
      manager.set_next_scene(ProgramState::Game);
    };
  }

  auto& btn_reset = add_column_button(LEVEL_COUNT + 1, "Reset Save Game");
  GameUi::setOutline(btn_reset);
  btn_reset.style->text_align = asw::TextJustify::Center;
  btn_reset.on_click = [this] { showResetConfirm(true); };

  btn_really_reset =
      &GameUi::addButton(ui, 700, 651, "Really reset?", 180, ROW_HEIGHT);
  btn_really_reset->on_click = [this] { resetSave(); };

  btn_cancel = &GameUi::addButton(ui, 700, 696, "Cancel", 180, ROW_HEIGHT);
  btn_cancel->on_click = [this] { showResetConfirm(false); };

  showResetConfirm(false);
}

void LevelSelect::showResetConfirm(bool show) {
  btn_really_reset->visible = show;
  btn_cancel->visible = show;
}

void LevelSelect::resetSave() {
  for (int level = 0; level <= LEVEL_COUNT; level++) {
    Config::setBool(std::format("level_{}_completed", level), false);
  }
  Config::save();

  // Reload to show the cleared progress
  manager.set_next_scene(ProgramState::LevelSelect);
}

void LevelSelect::update(float /*dt*/) {
  ui.update();
}

void LevelSelect::draw() {
  asw::draw::clear_color(asw::Color(75, 75, 100));
  ui.draw();
}
