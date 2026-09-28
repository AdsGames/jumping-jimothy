#include "Game.h"

#include <cmath>
#include <format>
#include <numbers>

#include "../Globals.h"
#include "../ui/GameUi.h"
#include "../util/Controls.h"
#include "../util/Audio.h"
#include "../util/Config.h"
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

// Landing faster than this, in metres per second, kicks up dust
constexpr float DUST_LANDING_SPEED = 6.0F;

// Landing faster than this shakes the screen
constexpr float SHAKE_LANDING_SPEED = 12.0F;

// Screen shake strength in pixels
constexpr float LANDING_SHAKE = 4.0F;
constexpr float DEATH_SHAKE = 10.0F;

// Feet of the character below its centre, in pixels
constexpr float FEET_OFFSET = 25.0F;

asw::ParticleConfig dustConfig() {
  asw::ParticleConfig config;
  config.lifetime_min = 0.25F;
  config.lifetime_max = 0.5F;
  config.speed_min = 30.0F;
  config.speed_max = 90.0F;
  // Upwards fan, screen angles so up is -pi/2
  config.angle_min = -std::numbers::pi_v<float>;
  config.angle_max = 0.0F;
  config.size_start = 5.0F;
  config.size_end = 1.0F;
  config.color_start = asw::Color(200, 200, 210, 200);
  config.color_end = asw::Color(200, 200, 210, 0);
  config.gravity = asw::Vec2<float>(0, 200);
  return config;
}

asw::ParticleConfig burstConfig() {
  asw::ParticleConfig config;
  config.lifetime_min = 0.4F;
  config.lifetime_max = 0.9F;
  config.speed_min = 80.0F;
  config.speed_max = 260.0F;
  config.size_start = 6.0F;
  config.size_end = 1.0F;
  config.color_start = asw::Color(255, 255, 255);
  config.color_end = asw::Color(255, 60, 60, 0);
  config.gravity = asw::Vec2<float>(0, 400);
  return config;
}

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

  dust = asw::ParticleEmitter(dustConfig(), 128);
  burst = asw::ParticleEmitter(burstConfig(), 128);
  camera.snap_to(asw::Vec2<float>(SCREEN_WIDTH / 2.0F, SCREEN_HEIGHT / 2.0F));

  // The game's actions play the game, the back button takes the mouse only
  ui = asw::ui::Root();
  GameUi::setup(ui, edit_font);
  GameUi::pointerOnly(ui);

  if (session.editing_level) {
    level = 0;
    GameUi::addButton(ui, 966, 728, "Back").on_click = [this] {
      changeScene(ProgramState::Editor);
    };
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
  ui.root.clear_children();

  asw::scene::Scene<ProgramState>::cleanup();
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
            object.x, object.y, assets.tile_sheet, object.orientation));
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

void Game::update(float /*dt*/) {
  if (leaving) {
    return;
  }

  ui.update();
  if (leaving) {
    return;
  }

  if (session.editing_level && asw::input::get_key_down(Key::P)) {
    changeScene(ProgramState::Editor);
    return;
  }

  if (asw::input::get_key_down(Key::Escape)) {
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

  updateEffects();

  // Skip level
  if (asw::input::get_key_down(Key::C)) {
    if (session.editing_level) {
      asw::dialog::info("Level complete!", "Opening editor");
      changeScene(ProgramState::Editor);
      return;
    }

    asw::log::info("Level {} skipped", level);
    nextLevel();
    return;
  }

  // Previous level
  if (asw::input::get_key_down(Key::X) && !session.editing_level) {
    asw::log::info("Level {} skipped back", level);
    level = std::max(level - 1, 1);
    reset();
    return;
  }

  if (asw::input::get_action_down(Controls::FREEZE)) {
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

  if (asw::input::get_action_down(Controls::RESTART)) {
    die();
  }
}

void Game::updateEffects() {
  camera.update(TICK_SECONDS);
  dust.update(TICK_SECONDS);
  burst.update(TICK_SECONDS);

  if (character == nullptr) {
    return;
  }

  const float landing = character->takeLanding();

  if (landing > DUST_LANDING_SPEED) {
    dust.transform.position =
        character->pixelPosition() + asw::Vec2<float>(0, FEET_OFFSET);
    dust.emit(static_cast<uint32_t>(landing * 2.0F));
  }

  if (landing > SHAKE_LANDING_SPEED) {
    camera.shake(LANDING_SHAKE);
  }
}

void Game::completeLevel() {
  if (session.editing_level) {
    asw::dialog::info("Level complete!", "Opening editor");
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
  if (character != nullptr) {
    const auto position = character->pixelPosition();
    asw::sound::play_at(assets.death, position.x);
    burst.transform.position = position;
    burst.emit(60);
  } else {
    asw::sound::play(assets.death);
  }

  camera.shake(DEATH_SHAKE);
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

  // Help text, prompts match the device the player uses
  for (std::size_t i = 0; i < help_text.size(); i++) {
    asw::draw::text_shadow(
        help_font, Controls::describe(help_text[i]),
        asw::Vec2<float>(500, 75 + (static_cast<float>(i) * 50)),
        asw::Color(255, 255, 255), asw::Color(0, 0, 0, 160),
        asw::Vec2<float>(3, 3), asw::TextJustify::Center);
  }

  for (const auto& box : boxes) {
    box->draw(camera);
  }

  dust.draw(camera);
  burst.draw(camera);

  ui.draw();

  asw::draw::sprite(static_mode ? assets.pause : assets.play,
                    asw::Vec2<float>(10, 10));

  asw::draw::text_shadow(game_font, std::format("Level {}", level),
                         asw::Vec2<float>(1010, 15), asw::Color(255, 255, 255),
                         asw::Color(0, 0, 0, 160), asw::Vec2<float>(2, 2),
                         asw::TextJustify::Right);
}
