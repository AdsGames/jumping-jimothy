#include "Menu.h"

#include <array>

#include "../util/Audio.h"
#include "../util/Controls.h"

namespace {
constexpr float BUTTON_X = 40;
constexpr float BUTTON_WIDTH = 179;
constexpr float BUTTON_HEIGHT = 20;

// Top left of the title
constexpr float TITLE_X = 300;
constexpr float TITLE_Y = 100;

constexpr int TITLE_CYCLE_TICKS = 300;
constexpr int SHINE_TICKS = 50;

const asw::Color WHITE(255, 255, 255);
}  // namespace

void Menu::init() {
  title = asw::assets::load_texture("assets/images/title_static.png");
  title_overlay = asw::assets::load_texture("assets/images/title_overlay.png");
  title_shine = asw::assets::load_texture("assets/images/title_shine.png");
  logo = asw::assets::load_texture("assets/images/logo.png");

  menu_font = asw::assets::load_font("assets/fonts/munro.ttf", 18,
                                     asw::FontStyle::Pixel);
  button_font = asw::assets::load_font("assets/fonts/munro.ttf", 24,
                                       asw::FontStyle::Pixel);
  credits_font = asw::assets::load_font("assets/fonts/munro.ttf", 32,
                                        asw::FontStyle::Pixel);

  ui.clear();

  auto add_button = [this](float y, const std::string& text) {
    auto& button = ui.add<Button>(BUTTON_X, y, text, button_font);
    button.setSize(BUTTON_WIDTH, BUTTON_HEIGHT);
    return &button;
  };

  btn_play = add_button(500, "Play");
  btn_editor = add_button(550, "Level Editor");
  btn_settings = add_button(600, "Settings");
  btn_credits = add_button(650, "Credits");
  btn_exit = add_button(700, "Exit");

  // The editor needs native file choosers, which the browser does not have
#ifdef __EMSCRIPTEN__
  btn_editor->hide();
#endif

  credits_menu = false;
  counter_title = 0;

  Audio::playMusic(Audio::Track::Menu);
}

void Menu::update(float /*dt*/) {
  counter_title = (counter_title + 1) % TITLE_CYCLE_TICKS;

  if (credits_menu) {
    if (asw::input::get_keyboard().any_pressed ||
        asw::input::get_mouse_button_down(asw::input::MouseButton::Left) ||
        asw::input::get_action_down(Controls::SELECT) ||
        asw::input::get_action_down(Controls::BACK)) {
      credits_menu = false;
    }
    return;
  }

  ui.update();

  if (btn_play->clicked()) {
    manager.set_next_scene(ProgramState::LevelSelect);
  } else if (btn_editor->clicked()) {
    manager.set_next_scene(ProgramState::Editor);
  } else if (btn_settings->clicked()) {
    manager.set_next_scene(ProgramState::Options);
  } else if (btn_credits->clicked()) {
    credits_menu = true;
  } else if (btn_exit->clicked()) {
    asw::core::exit();
  }
}

void Menu::draw() {
  asw::draw::clear_color(asw::Color(50, 50, 50));

  if (credits_menu) {
    drawCredits();
  } else {
    drawTitle();
    ui.draw();
  }
}

void Menu::drawTitle() const {
  asw::draw::stretch_sprite_blit(title, asw::Quad<float>(0, 0, 175, 160),
                                 asw::Quad<float>(TITLE_X, TITLE_Y, 612, 560));

  if (counter_title < SHINE_TICKS) {
    asw::draw::stretch_sprite_blit(
        title_shine, asw::Quad<float>(0, 0, 60, 150),
        asw::Quad<float>(TITLE_X + static_cast<float>(counter_title * 10),
                         TITLE_Y, 210, 525));
  }

  asw::draw::stretch_sprite_blit(title_overlay,
                                 asw::Quad<float>(0, 0, 200, 160),
                                 asw::Quad<float>(TITLE_X, TITLE_Y, 700, 560));

  constexpr std::array info{"TOJam 12, 2017", "Danny Van Stemp",
                            "Allan Legemaate", "Sullivan Stobo",
                            "Max Keleher"};

  for (std::size_t i = 0; i < info.size(); i++) {
    asw::draw::text(menu_font, info[i],
                    asw::Vec2<float>(1010, 15 + (static_cast<float>(i) * 20)),
                    WHITE, asw::TextJustify::Right);
  }
}

void Menu::drawCredits() const {
  constexpr float PADDING = 50;
  constexpr float X = 395;

  asw::draw::sprite(logo, asw::Vec2<float>(730, 50));

  const std::array<std::pair<float, const char*>, 10> lines{{
      {40, "Written in C++"},
      {80, "ASW (SDL3) for graphics and sound"},
      {120, "Box2D for physics"},
      {160, "pugixml for level loading/saving"},
      {240, "Music/code by Allan Legemaate"},
      {280, "Art/game design by Sullivan Stobo"},
      {320, "Level/game design by Max Keleher"},
      {360, "Lead code by Danny Van Stemp"},
      {400, "Art made in Paint.net and Aseprite"},
      {440, "Music made in FL Studio"},
  }};

  for (const auto& [y, text] : lines) {
    asw::draw::text(credits_font, text, asw::Vec2<float>(X, y + PADDING), WHITE,
                    asw::TextJustify::Center);
  }

  asw::draw::text(credits_font, "Made for TOJam 12", asw::Vec2<float>(835, 320),
                  WHITE, asw::TextJustify::Center);
  asw::draw::text(credits_font, "ADS Games, 2017", asw::Vec2<float>(835, 280),
                  WHITE, asw::TextJustify::Center);
  asw::draw::text(credits_font, "Press any key to return.",
                  asw::Vec2<float>(40, 720), asw::Color(255, 100, 100));
}
