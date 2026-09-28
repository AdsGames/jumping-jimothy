#include "Audio.h"

#include "Config.h"

namespace {
asw::Music menu_music;
asw::Music game_music;

// Track that should play, kept while music is off so it can resume
Audio::Track current = Audio::Track::None;
}  // namespace

void Audio::init() {
  menu_music = asw::assets::load_music("assets/music/menu.ogg");
  game_music = asw::assets::load_music("assets/music/tojam.ogg");

  asw::sound::set_sfx_volume(Config::getBool("sfx_enabled") ? 1.0F : 0.0F);
  asw::sound::set_music_volume(Config::getBool("music_enabled") ? 1.0F : 0.0F);
}

void Audio::playMusic(Track track) {
  if (track == current && asw::sound::is_music_playing()) {
    return;
  }

  current = track;

  switch (track) {
    case Track::Menu:
      asw::sound::play_music(menu_music);
      break;
    case Track::Game:
      asw::sound::play_music(game_music);
      break;
    case Track::None:
      asw::sound::stop_music();
      break;
  }
}

void Audio::stopMusic() {
  current = Track::None;
  asw::sound::stop_music();
}

void Audio::setSfxEnabled(bool enabled) {
  Config::setBool("sfx_enabled", enabled);
  asw::sound::set_sfx_volume(enabled ? 1.0F : 0.0F);
}

void Audio::setMusicEnabled(bool enabled) {
  Config::setBool("music_enabled", enabled);
  asw::sound::set_music_volume(enabled ? 1.0F : 0.0F);
}
