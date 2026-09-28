#include "Audio.h"

#include <algorithm>

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

void Audio::playAt(const asw::Sample& sample,
                   float screen_x,
                   asw::sound::PlayOptions options) {
  // Full left at the left edge to full right at the right edge, softened so
  // nothing plays in one ear only
  constexpr float PAN_STRENGTH = 0.7F;
  const float centre =
      static_cast<float>(asw::display::get_logical_size().x) / 2.0F;

  if (centre > 0.0F) {
    options.pan =
        std::clamp((screen_x - centre) / centre, -1.0F, 1.0F) * PAN_STRENGTH;
  }

  asw::sound::play(sample, options);
}

void Audio::setSfxEnabled(bool enabled) {
  Config::setBool("sfx_enabled", enabled);
  asw::sound::set_sfx_volume(enabled ? 1.0F : 0.0F);
}

void Audio::setMusicEnabled(bool enabled) {
  Config::setBool("music_enabled", enabled);
  asw::sound::set_music_volume(enabled ? 1.0F : 0.0F);
}
