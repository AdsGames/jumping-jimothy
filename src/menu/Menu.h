/**
 * Menu
 * Menu state
 * Danny Van Stemp
 * 06/05/2017
 **/

#pragma once

#include <asw/asw.h>

#include "../State.h"

class Menu : public asw::scene::Scene<ProgramState> {
 public:
  using asw::scene::Scene<ProgramState>::Scene;

  void init() override;
  void update(float dt) override;
  void draw() override;

 private:
  void drawTitle() const;
  void drawCredits() const;

  asw::Texture title;
  asw::Texture title_overlay;
  asw::Texture title_shine;
  asw::Texture logo;

  asw::Font menu_font;
  asw::Font button_font;
  asw::Font credits_font;

  asw::ui::Root ui;

  bool credits_menu{false};

  // Title shine animation
  int counter_title{0};
};
