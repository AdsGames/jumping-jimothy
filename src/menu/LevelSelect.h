/**
 * Level Select
 * Danny Van Stemp
 * Level select state
 * 04/01/2018
 **/

#pragma once

#include <vector>

#include "../State.h"
#include "../ui/Button.h"
#include "../ui/UIHandler.h"

class LevelSelect : public asw::scene::Scene<ProgramState> {
 public:
  using asw::scene::Scene<ProgramState>::Scene;

  void init() override;
  void update(float dt) override;
  void draw() override;

 private:
  void showResetConfirm(bool show);

  asw::Font font;
  asw::Font font_large;

  UIHandler ui;
  std::vector<Button*> level_buttons;
  Button* btn_back{nullptr};
  Button* btn_reset{nullptr};
  Button* btn_really_reset{nullptr};
  Button* btn_cancel{nullptr};
};
