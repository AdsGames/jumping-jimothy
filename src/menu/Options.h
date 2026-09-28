/**
 * Options
 * Danny Van Stemp and Allan Legemaate
 * The options menu state. Works with config
 * 22/11/2018
 **/

#pragma once

#include <asw/asw.h>

#include "../State.h"

class Options : public asw::scene::Scene<ProgramState> {
 public:
  using asw::scene::Scene<ProgramState>::Scene;

  void init() override;
  void update(float dt) override;
  void draw() override;

 private:
  void back();

  asw::Font options_font;
  asw::Font title_font;

  asw::ui::Root ui;
  asw::ui::Label* lbl_gamepad{nullptr};
};
