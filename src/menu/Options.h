/**
 * Options
 * Danny Van Stemp and Allan Legemaate
 * The options menu state. Works with config
 * 22/11/2018
 **/

#pragma once

#include "../FixedScene.h"
#include "../ui/Button.h"
#include "../ui/CheckBox.h"
#include "../ui/Label.h"
#include "../ui/UIHandler.h"

class Options : public FixedScene {
 public:
  using FixedScene::FixedScene;

  void init() override;
  void draw() override;

 protected:
  void tick() override;

 private:
  asw::Font options_font;
  asw::Font title_font;

  UIHandler ui;
  Label* lbl_gamepad{nullptr};
  CheckBox* chk_sfx{nullptr};
  CheckBox* chk_music{nullptr};
  CheckBox* chk_fullscreen{nullptr};
  Button* btn_back{nullptr};
};
