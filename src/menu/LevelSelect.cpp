#include "menu/LevelSelect.h"

#include <allegro5/allegro_ttf.h>

#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"

#include "ui/Button.h"
#include "ui/Label.h"

#include "util/KeyListener.h"
#include "util/Config.h"

#include "util/Tools.h"

LevelSelect::LevelSelect() {

  levelselect_font = al_load_ttf_font("fonts/munro.ttf", 24, 0);

  levelselect_font_large = al_load_ttf_font("fonts/munro.ttf", 48, 0);

  levelSelectUI = UIHandler();

  levelSelectUI.addElement(new Label(375, 5, "Select a level","lblSelectLevel", levelselect_font_large));
  levelSelectUI.getElementById("lblSelectLevel") -> setTextColour(al_map_rgb(255,255,255));

  int x_loc = 340;
  int y_spacing = 45;
  int y_init = 66;

  reset_game_menu = false;

  createLevelButton(x_loc,y_init+y_spacing,1);
  createLevelButton(x_loc,y_init+y_spacing*2,2);
  createLevelButton(x_loc,y_init+y_spacing*3,3);
  createLevelButton(x_loc,y_init+y_spacing*4,4);
  createLevelButton(x_loc,y_init+y_spacing*5,5);
  createLevelButton(x_loc,y_init+y_spacing*6,6);
  createLevelButton(x_loc,y_init+y_spacing*7,7);
  createLevelButton(x_loc,y_init+y_spacing*8,8);
  createLevelButton(x_loc,y_init+y_spacing*9,9);
  createLevelButton(x_loc,y_init+y_spacing*10,10);
  createLevelButton(x_loc,y_init+y_spacing*11,11);
  createLevelButton(x_loc,y_init+y_spacing*12,12);
  createLevelButton(x_loc,y_init+y_spacing*13,13);

  levelSelectUI.addElement(new Button(x_loc, y_spacing*14 + y_init, "Reset Save Game", "btnResetSave", levelselect_font));
  levelSelectUI.getElementById("btnResetSave") -> setSize(300,18);
  //levelSelectUI.getElementById("btnResetSave") -> disableHoverEffect();
  //levelSelectUI.getElementById("btnResetSave") -> setCellFillTransparent(true);
  levelSelectUI.getElementById("btnResetSave") -> setTextJustification(1);
  //levelSelectUI.getElementById("btnResetSave") -> setTextColour(al_map_rgb(255,255,255));

  levelSelectUI.addElement(new Button(x_loc, y_init, "Back to main menu", "btnBack", levelselect_font));
  levelSelectUI.getElementById("btnBack") -> setSize(300,18);
  //levelSelectUI.getElementById("btnBack") -> disableHoverEffect();
  //levelSelectUI.getElementById("btnBack") -> setCellFillTransparent(true);
  levelSelectUI.getElementById("btnBack") -> setTextJustification(1);
  //levelSelectUI.getElementById("btnBack") -> setTextColour(al_map_rgb(255,255,255));

  levelSelectUI.addElement(new Button(700, 651, "Really reset?", "btnReallyReset", levelselect_font));
  levelSelectUI.getElementById("btnReallyReset") -> setSize(180,18);
  levelSelectUI.getElementById("btnReallyReset") -> disable();
  levelSelectUI.getElementById("btnReallyReset") -> hide();
  //levelSelectUI.getElementById("btnReallyReset") -> disableHoverEffect();


  levelSelectUI.addElement(new Button(700, 696, "Cancel", "btnCancel", levelselect_font));
  levelSelectUI.getElementById("btnCancel") -> setSize(180,18);
  levelSelectUI.getElementById("btnCancel") -> disable();
  levelSelectUI.getElementById("btnCancel") -> hide();
  //levelSelectUI.getElementById("btnCancel") -> disableHoverEffect();
}

void LevelSelect::createLevelButton(int newX, int newY, int newLevelNumber){

  levelSelectUI.addElement(new Button(newX, newY, "Level " + tools::toString(newLevelNumber), "btnLevel" + tools::toString(newLevelNumber) ,levelselect_font));
  levelSelectUI.getElementById("btnLevel"+ tools::toString(newLevelNumber)) -> setHeight(18);
  //levelSelectUI.getElementById("btnLevel"+ tools::toString(newLevelNumber)) -> setCellFillTransparent(true);
  //levelSelectUI.getElementById("btnLevel"+ tools::toString(newLevelNumber)) -> setTextColour(al_map_rgb(255,255,255));

  levelSelectUI.getElementById("btnLevel"+ tools::toString(newLevelNumber)) -> setWidth(300);
  levelSelectUI.getElementById("btnLevel"+ tools::toString(newLevelNumber)) -> setTextJustification(1);

  if(Config::getBooleanValue("level_" + tools::toString(newLevelNumber) + "_completed")) {
    levelSelectUI.getElementById("btnLevel"+ tools::toString(newLevelNumber)) -> setBackgroundColour(al_map_rgb(0,200,0));
    levelSelectUI.getElementById("btnLevel"+ tools::toString(newLevelNumber)) -> setTextColour(al_map_rgb(255,255,255));
    //levelSelectUI.getElementById("btnLevel"+ tools::toString(newLevelNumber)) -> setCellFillTransparent(false);
    //levelSelectUI.getElementById("btnLevel"+ tools::toString(newLevelNumber)) -> disableHoverEffect();
  }
}

void LevelSelect::draw(){
  al_clear_to_color( al_map_rgb(75,75,100));
  levelSelectUI.draw();
}

void LevelSelect::update(){
  levelSelectUI.update();

  if (KeyListener::key[ALLEGRO_KEY_ESCAPE] || levelSelectUI.getElementById("btnBack") -> clicked()) {
    set_next_state(STATE_MENU);
    //tools::log_message("Changing State to MENUU");
  }

  if (levelSelectUI.getElementById("btnResetSave") -> clicked()) {
    reset_game_menu=true;
    levelSelectUI.getElementById("btnCancel") -> show();
    levelSelectUI.getElementById("btnCancel") -> enable();
    levelSelectUI.getElementById("btnReallyReset") -> show();
    levelSelectUI.getElementById("btnReallyReset") -> enable();
  }

  if (levelSelectUI.getElementById("btnReallyReset") -> clicked()) {
    for(int i = 0; i < 16; i++)
      Config::setValue("level_" + tools::toString(i) + "_completed", false);
    set_next_state(STATE_LEVELSELECT);
  }

  if (levelSelectUI.getElementById("btnCancel") -> clicked()) {
    reset_game_menu = false;
    levelSelectUI.getElementById("btnCancel") -> hide();
    levelSelectUI.getElementById("btnCancel") -> disable();
    levelSelectUI.getElementById("btnReallyReset") -> hide();
    levelSelectUI.getElementById("btnReallyReset") -> disable();
  }

  for(int i=1; i<14; i++){
    if(levelSelectUI.getUIElements().at(i) -> clicked()){
      Config::setValue("level_to_start", i);
      set_next_state(STATE_GAME);
    }
  }
}
