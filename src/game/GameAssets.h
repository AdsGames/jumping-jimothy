/**
 * Game Assets
 * Allan Legemaate
 * Images and sounds loaded once and shared by game objects
 * 27/09/2026
 **/

#pragma once

#include <asw/asw.h>

struct GameAssets {
  void load() {
    box = asw::assets::load_texture("assets/images/box.png");
    box_repel = asw::assets::load_texture("assets/images/box_repel.png");
    box_repel_direction =
        asw::assets::load_texture("assets/images/box_repel_direction.png");
    goat = asw::assets::load_texture("assets/images/goat.png");
    character = asw::assets::load_texture("assets/images/character.png");
    static_tiles = asw::assets::load_texture("assets/images/StaticBlock.png");
    play = asw::assets::load_texture("assets/images/play.png");
    pause = asw::assets::load_texture("assets/images/pause.png");

    jump = asw::assets::load_sample("assets/sfx/jump.wav");
    land = asw::assets::load_sample("assets/sfx/land.wav");
    toggle_on = asw::assets::load_sample("assets/sfx/toggle_on.wav");
    toggle_off = asw::assets::load_sample("assets/sfx/toggle_off.wav");
    death = asw::assets::load_sample("assets/sfx/death.wav");
  }

  asw::Texture box;
  asw::Texture box_repel;
  asw::Texture box_repel_direction;
  asw::Texture goat;
  asw::Texture character;
  asw::Texture static_tiles;
  asw::Texture play;
  asw::Texture pause;

  asw::Sample jump;
  asw::Sample land;
  asw::Sample toggle_on;
  asw::Sample toggle_off;
  asw::Sample death;
};
