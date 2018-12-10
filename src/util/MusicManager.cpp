#include "util/MusicManager.h"

Sound *MusicManager::game_music;
Sound *MusicManager::menu_music;

// Loads game music
void MusicManager::load() {
  game_music = new Sound;
  game_music -> load_ogg("music/tojam.ogg");
  menu_music = new Sound;
  menu_music -> load_ogg("music/menu.ogg");
}

// Destroys game music samples
void MusicManager::destroy() {
  delete game_music;
  delete menu_music;
}

