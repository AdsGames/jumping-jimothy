/**
 * Menu
 * Menu state
 * Danny Van Stemp
 * 06/05/2017
 **/

#pragma once

#include "../State.h"
#include "../ui/Button.h"
#include "../ui/UIHandler.h"

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

  UIHandler ui;
  Button* btn_play{nullptr};
  Button* btn_editor{nullptr};
  Button* btn_settings{nullptr};
  Button* btn_credits{nullptr};
  Button* btn_exit{nullptr};

  bool credits_menu{false};

  // Title shine animation
  int counter_title{0};
};
