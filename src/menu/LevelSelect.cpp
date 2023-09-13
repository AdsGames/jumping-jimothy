#include "LevelSelect.h"

#include <allegro5/allegro_ttf.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wzero-as-null-pointer-constant"
#pragma GCC diagnostic ignored "-Wswitch-default"
#include "../rapidxml/rapidxml.hpp"
#include "../rapidxml/rapidxml_print.hpp"
#pragma GCC diagnostic pop

#include "../ui/Button.h"
#include "../ui/Label.h"

#include "../util/ActionBinder.h"
#include "../util/Config.h"
#include "../util/Tools.h"

LevelSelect::LevelSelect() {
  levelselect_font = al_load_ttf_font("assets/fonts/munro.ttf", 24, 0);

  levelselect_font_large = al_load_ttf_font("assets/fonts/munro.ttf", 48, 0);

  levelSelectUI.addElement(std::make_shared<Label>(
      375, 5, "Select a level", "lblSelectLevel", levelselect_font_large));
  levelSelectUI.getElementById("lblSelectLevel")
      ->setTextColour(al_map_rgb(255, 255, 255));

  int x_loc = 340;
  int y_spacing = 45;
  int y_init = 66;

  for (int i = 0; i < 13; i++) {
    createLevelButton(x_loc, y_init + y_spacing * (i + 1), i + 1);
  }

  levelSelectUI.addElement(std::make_shared<Button>(
      x_loc, y_spacing * 14 + y_init, "Reset Save Game", "btnResetSave",
      levelselect_font));
  levelSelectUI.getElementById("btnResetSave")->setSize(300, 18);
  levelSelectUI.getElementById("btnResetSave")->setCellFillTransparent(true);
  levelSelectUI.getElementById("btnResetSave")->setTextJustification(1);
  levelSelectUI.getElementById("btnResetSave")
      ->setTextColour(al_map_rgb(255, 255, 255));

  levelSelectUI.addElement(std::make_shared<Button>(
      700, 651, "Really reset?", "btnReallyReset", levelselect_font));
  levelSelectUI.getElementById("btnReallyReset")->setSize(180, 18);
  levelSelectUI.getElementById("btnReallyReset")->disable();
  levelSelectUI.getElementById("btnReallyReset")->hide();

  levelSelectUI.addElement(std::make_shared<Button>(
      700, 696, "Cancel", "btnCancel", levelselect_font));
  levelSelectUI.getElementById("btnCancel")->setSize(180, 18);
  levelSelectUI.getElementById("btnCancel")->disable();
  levelSelectUI.getElementById("btnCancel")->hide();

  levelSelectUI.addElement(std::make_shared<Button>(
      x_loc, y_init, "Back to main menu", "btnBack", levelselect_font));
  levelSelectUI.getElementById("btnBack")->setSize(300, 18);
  levelSelectUI.getElementById("btnBack")->setCellFillTransparent(true);
  levelSelectUI.getElementById("btnBack")->setTextJustification(1);
  levelSelectUI.getElementById("btnBack")->setTextColour(
      al_map_rgb(255, 255, 255));
}

LevelSelect::~LevelSelect() {
  al_destroy_font(levelselect_font);
  al_destroy_font(levelselect_font_large);
}

void LevelSelect::createLevelButton(int newX, int newY, int newLevelNumber) {
  levelSelectUI.addElement(std::make_shared<Button>(
      newX, newY, "Level " + tools::toString(newLevelNumber),
      "btnLevel" + tools::toString(newLevelNumber), levelselect_font));
  levelSelectUI.getElementById("btnLevel" + tools::toString(newLevelNumber))
      ->setHeight(18);

  levelSelectUI.getElementById("btnLevel" + tools::toString(newLevelNumber))
      ->setWidth(300);
  levelSelectUI.getElementById("btnLevel" + tools::toString(newLevelNumber))
      ->setTextJustification(1);

  if (Config::getBooleanValue("level_" + tools::toString(newLevelNumber) +
                              "_completed")) {
    levelSelectUI.getElementById("btnLevel" + tools::toString(newLevelNumber))
        ->setBackgroundColour(al_map_rgb(0, 200, 0));
    levelSelectUI.getElementById("btnLevel" + tools::toString(newLevelNumber))
        ->setTextColour(al_map_rgb(255, 255, 255));
  }
}

void LevelSelect::draw() {
  al_clear_to_color(al_map_rgb(75, 75, 100));
  levelSelectUI.draw();
}

void LevelSelect::update(StateEngine* engine) {
  levelSelectUI.update();

  if (ActionBinder::actionBegun(Action::B) ||
      levelSelectUI.getElementById("btnBack")->clicked()) {
    setNextState(engine, ProgramState::MENU);
  }

  if (levelSelectUI.getElementById("btnResetSave")->clicked()) {
    reset_game_menu = true;
    levelSelectUI.getElementById("btnCancel")->show();
    levelSelectUI.getElementById("btnCancel")->enable();
    levelSelectUI.getElementById("btnReallyReset")->show();
    levelSelectUI.getElementById("btnReallyReset")->enable();
  }

  if (levelSelectUI.getElementById("btnReallyReset")->clicked()) {
    for (int i = 0; i < 16; i++)
      Config::setValue("level_" + tools::toString(i) + "_completed", false);
    setNextState(engine, ProgramState::LEVELSELECT);
  }

  if (levelSelectUI.getElementById("btnCancel")->clicked()) {
    reset_game_menu = false;
    levelSelectUI.getElementById("btnCancel")->hide();
    levelSelectUI.getElementById("btnCancel")->disable();
    levelSelectUI.getElementById("btnReallyReset")->hide();
    levelSelectUI.getElementById("btnReallyReset")->disable();
  }

  for (int i = 1; i < 14; i++) {
    if (levelSelectUI.getUIElements().at(i)->clicked()) {
      Config::setValue("level_to_start", i);
      setNextState(engine, ProgramState::GAME);
    }
  }
}
