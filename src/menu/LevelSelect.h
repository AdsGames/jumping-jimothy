/**
 * Level Select
 * Danny Van Stemp
 * Level select state
 * 04/01/2018
 **/

#pragma once

#include <asw/asw.h>

#include "../State.h"

class LevelSelect : public asw::scene::Scene<ProgramState> {
 public:
  using asw::scene::Scene<ProgramState>::Scene;

  void init() override;
  void update(float dt) override;
  void draw() override;

 private:
  void showResetConfirm(bool show);
  void resetSave();

  asw::Font font;
  asw::Font font_large;

  asw::ui::Root ui;
  asw::ui::Button* btn_really_reset{nullptr};
  asw::ui::Button* btn_cancel{nullptr};
};
