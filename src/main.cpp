/**
 * Main
 * Allan Legemaate and Danny Van Stemp
 * This is the main for Jumping Jimothy
 * 09/05/2017
 **/

#include <asw/asw.h>
#include <chrono>

#include "Globals.h"
#include "State.h"
#include "editor/Editor.h"
#include "game/Game.h"
#include "menu/LevelSelect.h"
#include "menu/Menu.h"
#include "menu/Options.h"
#include "util/Audio.h"
#include "util/Config.h"

int main() {
  asw::core::init(SCREEN_WIDTH, SCREEN_HEIGHT);
  asw::display::set_title("Jumping Jimothy");

  // Let see through fills blend
  asw::display::set_blend_mode(asw::BlendMode::Blend);

  Config::load();
  asw::display::set_fullscreen(Config::getBool("fullscreen"));
  Audio::init();

  auto app = asw::scene::SceneManager<ProgramState>();
  app.register_scene<Menu>(ProgramState::Menu, app);
  app.register_scene<Game>(ProgramState::Game, app);
  app.register_scene<Editor>(ProgramState::Editor, app);
  app.register_scene<LevelSelect>(ProgramState::LevelSelect, app);
  app.register_scene<Options>(ProgramState::Options, app);

  // Game logic is tuned for 60 ticks per second
  app.set_timestep(std::chrono::duration_cast<std::chrono::nanoseconds>(
      std::chrono::duration<float>(TICK_SECONDS)));

  app.set_next_scene(ProgramState::Menu);
  app.start();

  Config::save();
  asw::core::shutdown();

  return 0;
}
