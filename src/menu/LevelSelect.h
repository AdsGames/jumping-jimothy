/**
 * Level Select
 * Danny Van Stemp
 * Level select state
 * 04/01/2018
 **/

#pragma once

#include <vector>

#include "../FixedScene.h"
#include "../ui/Button.h"
#include "../ui/UIHandler.h"

class LevelSelect : public FixedScene {
 public:
  using FixedScene::FixedScene;

  void init() override;
  void draw() override;

 protected:
  void tick() override;

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
