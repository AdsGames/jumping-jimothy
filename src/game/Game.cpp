#include "Game.h"

#include <cmath>
#include <format>

#include "../Globals.h"
#include "../util/ActionBinder.h"
#include "../util/Audio.h"
#include "../util/Config.h"
#include "../util/Dialog.h"
#include "../util/Input.h"
#include "CollisionBox.h"
#include "DynamicBox.h"
#include "Explosive.h"
#include "Level.h"
#include "StaticBox.h"

namespace {
using asw::input::Key;

const b2Vec2 GRAVITY(0.0F, -10.0F);
constexpr int VELOCITY_ITERATIONS = 6;
constexpr int POSITION_ITERATIONS = 2;

// Leaving this area kills the character, in metres
constexpr float BOUNDS_LEFT = -1.0F;
constexpr float BOUNDS_RIGHT = 51.5F;
constexpr float BOUNDS_TOP = 2.0F;
constexpr float BOUNDS_BOTTOM = -40.0F;

// Spawn offsets from the level file position, in metres
constexpr float CHARACTER_SPAWN_OFFSET = -1.6F;
constexpr float GOAT_SPAWN_OFFSET = -1.0F;
}  // namespace

void Game::init() {
  leaving = false;

  game_font = asw::assets::load_font("assets/fonts/munro.ttf", 30,
                                     asw::FontStyle::Pixel);
  help_font = asw::assets::load_font("assets/fonts/munro.ttf", 50,
                                     asw::FontStyle::Pixel);
  edit_font = asw::assets::load_font("assets/fonts/fantasque.ttf", 18);

  assets.load();

  ui.clear();
  back_button = nullptr;

  if (session.editing_level) {
    level = 0;
    back_button = &ui.add<Button>(966, 728, "Back", edit_font);
  } else {
    level = session.level_to_start;
  }

  reset();

  Audio::playMusic(Audio::Track::Game);
}

void Game::cleanup() {
  character = nullptr;
  goat = nullptr;
  boxes.clear();
  world.reset();
  ui.clear();
  back_button = nullptr;

  FixedScene::cleanup();
}

void Game::changeScene(ProgramState state) {
  leaving = true;
  manager.set_next_scene(state);
}

void Game::reset() {
  character = nullptr;
  goat = nullptr;
  boxes.clear();
  world = std::make_unique<b2World>(GRAVITY);

  if (session.editing_level) {
    loadLevel(session.editing_file);
  } else {
    loadLevel(asw::assets::get_path(levelPath(level)));
  }

  // Levels start with time frozen
  static_mode = true;
  first_play = true;
  for (const auto& box : boxes) {
    box->setPaused(true);
  }
}

void Game::loadLevel(const std::string& path) {
  asw::log::info("Loading level {}", path);

  const auto level_data = Level::load(path);
  if (!level_data) {
    return;
  }

  std::vector<Goat*> goats;
  std::vector<Explosive*> explosives;
  int character_count = 0;

  for (const auto& object : level_data->objects) {
    switch (object.type) {
      case ObjectType::Static:
        boxes.push_back(std::make_unique<StaticBox>(
            object.x, object.y, assets.static_tiles, object.orientation));
        break;

      case ObjectType::Dynamic:
        boxes.push_back(std::make_unique<DynamicBox>(object.x, object.y,
                                                     assets.box, *world));
        break;

      case ObjectType::Collision:
        boxes.push_back(std::make_unique<CollisionBox>(
            object.x, object.y, object.width, object.height, *world));
        break;

      case ObjectType::Character: {
        auto new_character = std::make_unique<Character>(
            object.x, object.y + CHARACTER_SPAWN_OFFSET, assets, *world);
        character = new_character.get();
        boxes.push_back(std::move(new_character));
        character_count++;
        break;
      }

      case ObjectType::Finish: {
        auto new_goat = std::make_unique<Goat>(
            object.x, object.y + GOAT_SPAWN_OFFSET, assets, *world);
        goats.push_back(new_goat.get());
        boxes.push_back(std::move(new_goat));
        break;
      }

      case ObjectType::Explosive: {
        auto explosive = std::make_unique<Explosive>(
            object.x, object.y, object.orientation[0], object.affect_character,
            assets, *world);
        explosives.push_back(explosive.get());
        boxes.push_back(std::move(explosive));
        break;
      }
    }
  }

  // Link the character once everything is loaded, the order in the file
  // does not matter
  for (auto* g : goats) {
    g->setCharacter(character);
  }

  for (auto* explosive : explosives) {
    explosive->setCharacter(character);
  }

  goat = goats.empty() ? nullptr : goats.front();
  help_text = level_data->helpLines();

  if (character_count != 1) {
    asw::log::warn("Level has {} characters, expected 1", character_count);
  }

  if (goats.size() != 1) {
    asw::log::warn("Level has {} goats, expected 1", goats.size());
  }
}

void Game::tick() {
  if (leaving) {
    return;
  }

  ui.update();

  // Back to the editor when testing a level
  if (back_button != nullptr && back_button->clicked()) {
    changeScene(ProgramState::Editor);
    return;
  }

  if (session.editing_level && input::keyPressed(Key::P)) {
    changeScene(ProgramState::Editor);
    return;
  }

  if (input::keyPressed(Key::Escape)) {
    Audio::stopMusic();
    changeScene(ProgramState::Menu);
    return;
  }

  if (goat != nullptr && goat->getWinCondition()) {
    completeLevel();
    return;
  }

  world->Step(TICK_SECONDS, VELOCITY_ITERATIONS, POSITION_ITERATIONS);

  for (const auto& box : boxes) {
    box->update(*world);
  }

  // Skip level
  if (input::keyPressed(Key::C)) {
    if (session.editing_level) {
      Dialog::info("Level complete!", "Opening editor");
      changeScene(ProgramState::Editor);
      return;
    }

    asw::log::info("Level {} skipped", level);
    nextLevel();
    return;
  }

  // Previous level
  if (input::keyPressed(Key::X) && !session.editing_level) {
    asw::log::info("Level {} skipped back", level);
    level = std::max(level - 1, 1);
    reset();
    return;
  }

  if (ActionBinder::actionBegun(Action::B)) {
    togglePause();
  }

  // Fell out of the level
  if (character != nullptr) {
    const float x = character->getX();
    const float y = character->getY();

    if (!std::isfinite(x) || !std::isfinite(y) || x < BOUNDS_LEFT ||
        x > BOUNDS_RIGHT || y > BOUNDS_TOP || y < BOUNDS_BOTTOM) {
      die();
      return;
    }
  }

  if (input::keyPressed(Key::R)) {
    die();
  }
}

void Game::completeLevel() {
  if (session.editing_level) {
    Dialog::info("Level complete!", "Opening editor");
    changeScene(ProgramState::Editor);
    return;
  }

  asw::log::info("Level {} completed", level);
  Config::setBool(std::format("level_{}_completed", level), true);
  Config::save();

  nextLevel();
}

void Game::nextLevel() {
  if (level >= LEVEL_COUNT) {
    Audio::stopMusic();
    changeScene(ProgramState::Menu);
    return;
  }

  level++;
  reset();
}

void Game::die() {
  asw::sound::play(assets.death);
  reset();
}

void Game::togglePause() {
  static_mode = !static_mode;

  for (const auto& box : boxes) {
    box->setPaused(static_mode, !first_play);
  }

  if (!static_mode) {
    first_play = false;
  }

  asw::sound::play(static_mode ? assets.toggle_off : assets.toggle_on);
}

void Game::draw() {
  asw::draw::clear_color(asw::Color(40, 40, 60));

  // Help text
  for (std::size_t i = 0; i < help_text.size(); i++) {
    asw::draw::text(help_font, help_text[i],
                    asw::Vec2<float>(500, 75 + (static_cast<float>(i) * 50)),
                    asw::Color(255, 255, 255), asw::TextJustify::Center);
  }

  for (const auto& box : boxes) {
    box->draw();
  }

  ui.draw();

  asw::draw::sprite(static_mode ? assets.pause : assets.play,
                    asw::Vec2<float>(10, 10));

  asw::draw::text(game_font, std::format("Level {}", level),
                  asw::Vec2<float>(1010, 15), asw::Color(255, 255, 255),
                  asw::TextJustify::Right);
}
