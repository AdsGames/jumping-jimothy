#include "menu/LevelSelect.h"

#include <fstream>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>

#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"

#include "ui/Button.h"
#include "ui/Label.h"

#include "util/KeyListener.h"
#include "util/MouseListener.h"
#include "util/JoystickListener.h"
#include "util/Config.h"

#include "util/Tools.h"

bool LevelSelect::completed_level_list[16]= {};

LevelSelect::LevelSelect() {

  //Doc
  rapidxml::xml_document<> doc;

//   Make an xml object
  std::ifstream theFile("data/level_data.xml");
  std::vector<char> xml_buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
  xml_buffer.push_back('\0');

  // Parse the buffer using the xml file parsing library into doc
  doc.parse<0>(&xml_buffer[0]);

//   Find our root node
  rapidxml::xml_node<> * root_node;
  root_node = doc.first_node("data");

   //Iterate over the nodes
  for (rapidxml::xml_node<> * object_node = root_node -> first_node("level"); object_node; object_node = object_node -> next_sibling()){

    int levelNumber = atoi(object_node -> first_attribute("number") -> value());

    std::string newStatus ="";
   //  Load data
    if( object_node -> first_node("status") != 0)
      newStatus = object_node -> first_node("status") -> value();

    completed_level_list[levelNumber] = (newStatus=="complete");

  }

  levelselect_font = al_load_ttf_font( "fonts/munro.ttf", 24, 0);

  levelselect_font_large = al_load_ttf_font( "fonts/munro.ttf", 48, 0);

  cursor = tools::load_bitmap_ex("images/cursor.png");
  highlight = tools::load_bitmap_ex("images/highlight.png");
  highlight_levelselect = tools::load_bitmap_ex("images/highlight_levelselect.png");


  levelSelectUI = UIHandler();

  levelSelectUI.addElement(new Label(375, 5, "Select a level","lblSelectLevel", levelselect_font_large));
  levelSelectUI.getElementById("lblSelectLevel") -> setTextColour(al_map_rgb(255,255,255));

  int x_loc=340;
  int y_spacing=45;
  int y_init=66;

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
  levelSelectUI.getElementById("btnResetSave") -> disableHoverEffect();
  levelSelectUI.getElementById("btnResetSave") -> setCellFillTransparent(true);
  levelSelectUI.getElementById("btnResetSave") -> setTextJustification(1);
  levelSelectUI.getElementById("btnResetSave") -> setTextColour(al_map_rgb(255,255,255));

  levelSelectUI.addElement(new Button(x_loc, y_init, "Back to main menu", "btnBack", levelselect_font));
  levelSelectUI.getElementById("btnBack") -> setSize(300,18);
  levelSelectUI.getElementById("btnBack") -> disableHoverEffect();
  levelSelectUI.getElementById("btnBack") -> setCellFillTransparent(true);
  levelSelectUI.getElementById("btnBack") -> setTextJustification(1);
  levelSelectUI.getElementById("btnBack") -> setTextColour(al_map_rgb(255,255,255));




  levelSelectUI.addElement(new Button(700, 651, "Really reset?", "btnReallyReset", levelselect_font));
  levelSelectUI.getElementById("btnReallyReset") -> setSize(180,18);
  levelSelectUI.getElementById("btnReallyReset") -> disable();
  levelSelectUI.getElementById("btnReallyReset") -> hide();
  levelSelectUI.getElementById("btnReallyReset") -> disableHoverEffect();


  levelSelectUI.addElement(new Button(700, 696, "Cancel", "btnCancel", levelselect_font));
  levelSelectUI.getElementById("btnCancel") -> setSize(180,18);
  levelSelectUI.getElementById("btnCancel") -> disable();
  levelSelectUI.getElementById("btnCancel") -> hide();
  levelSelectUI.getElementById("btnCancel") -> disableHoverEffect();
}

LevelSelect::~LevelSelect()
{
  //dtor
}

void LevelSelect::createLevelButton(int newX, int newY, int newLevelNumber){

  levelSelectUI.addElement(new Button(newX, newY, "Level " + tools::toString(newLevelNumber), "btnLevel" + tools::toString(newLevelNumber) ,levelselect_font));
  levelSelectUI.getElementById("btnLevel"+ tools::toString(newLevelNumber)) -> setHeight(18);
  levelSelectUI.getElementById("btnLevel"+ tools::toString(newLevelNumber)) -> setCellFillTransparent(true);
  levelSelectUI.getElementById("btnLevel"+ tools::toString(newLevelNumber)) -> setTextColour(al_map_rgb(255,255,255));


  levelSelectUI.getElementById("btnLevel"+ tools::toString(newLevelNumber)) -> setWidth(300);
  levelSelectUI.getElementById("btnLevel"+ tools::toString(newLevelNumber)) -> setTextJustification(1);
  if(completed_level_list[newLevelNumber]){
    levelSelectUI.getElementById("btnLevel"+ tools::toString(newLevelNumber)) -> setBackgroundColour(al_map_rgb(0,200,0));
    levelSelectUI.getElementById("btnLevel"+ tools::toString(newLevelNumber)) -> setCellFillTransparent(false);
    levelSelectUI.getElementById("btnLevel"+ tools::toString(newLevelNumber)) -> disableHoverEffect();
  }


}

void LevelSelect::setLevelComplete(int newLevelNumber){
  completed_level_list[newLevelNumber] = true;
}

void LevelSelect::writeLevelData(){
  // Write xml file
  rapidxml::xml_document<> doc;
  rapidxml::xml_node<>* decl = doc.allocate_node(rapidxml::node_declaration);
  decl -> append_attribute( doc.allocate_attribute("version", "1.0"));
  decl -> append_attribute( doc.allocate_attribute("encoding", "utf-8"));
  doc.append_node(decl);

  rapidxml::xml_node<>* root_node = doc.allocate_node( rapidxml::node_element, "data");
  doc.append_node(root_node);

  // Tiles
  for( unsigned int i = 0; i < 15; i++){

    char *node_name = doc.allocate_string("level");
    rapidxml::xml_node<>* object_node = doc.allocate_node( rapidxml::node_element, node_name);



    object_node -> append_attribute( doc.allocate_attribute("number", doc.allocate_string(tools::toString(i).c_str())));
    root_node -> append_node( object_node);

    // Save data

    std::string newStatus="incomplete";


    if(completed_level_list[i])
      newStatus="complete";


    char *newStatusChar = doc.allocate_string(newStatus.c_str());


    object_node -> append_node( doc.allocate_node( rapidxml::node_element, "status", newStatusChar));


    }


  // Save to file
  std::ofstream file_stored("data/level_data.xml");
  file_stored << doc;
  file_stored.close();
  doc.clear();




}

void LevelSelect::draw(){

  al_clear_to_color( al_map_rgb(75,75,100));
  levelSelectUI.draw();
  if(Config::getBooleanValue("draw_cursor"))
    al_draw_bitmap(cursor,MouseListener::mouse_x,MouseListener::mouse_y,0);

  if(!reset_game_menu)
    al_draw_bitmap(highlight_levelselect,340,highlight_y,0);
  else
    al_draw_bitmap(highlight,700,highlight_game_reset_y,0);



}
void LevelSelect::update(){

  if(highlight_y>highlight_y_destination)highlight_y-=5;
  if(highlight_y<highlight_y_destination)highlight_y+=5;
  if(highlight_y>highlight_y_destination)highlight_y-=5;
  if(highlight_y<highlight_y_destination)highlight_y+=5;

  if(highlight_game_reset_y>highlight_game_reset_y_destination)highlight_game_reset_y-=5;
  if(highlight_game_reset_y<highlight_game_reset_y_destination)highlight_game_reset_y+=5;
  if(highlight_game_reset_y>highlight_game_reset_y_destination)highlight_game_reset_y-=5;
  if(highlight_game_reset_y<highlight_game_reset_y_destination)highlight_game_reset_y+=5;


  if((JoystickListener::stickDirections[LEFT_STICK_UP] || JoystickListener::stickDirections[DPAD_UP2]) && !joystick_direction_hit){
    if(highlight_y_destination<695)
      highlight_y_destination+=45;
    if(highlight_game_reset_y_destination==650)
      highlight_game_reset_y_destination=695;
  }

  if((JoystickListener::stickDirections[LEFT_STICK_DOWN] || JoystickListener::stickDirections[DPAD_DOWN])  && !joystick_direction_hit){
    if(highlight_y_destination>65)
      highlight_y_destination-=45;

    if(highlight_game_reset_y_destination==695)
      highlight_game_reset_y_destination=650;

  }

  if(JoystickListener::stickDirections[LEFT_STICK_DOWN] || JoystickListener::stickDirections[LEFT_STICK_UP] || JoystickListener::stickDirections[DPAD_DOWN] || JoystickListener::stickDirections[DPAD_UP2]
  || JoystickListener::stickDirections[LEFT_STICK_LEFT] || JoystickListener::stickDirections[LEFT_STICK_RIGHT] || JoystickListener::stickDirections[DPAD_LEFT] || JoystickListener::stickDirections[DPAD_RIGHT] ){
    joystick_direction_hit=true;
    Config::setValue("joystick_mode",true);
  }
  else{
    Config::setValue("joystick_mode",false);
  }
  if(MouseListener::mouse_moved){
    Config::setValue("joystick_mode",false);
  }

  levelSelectUI.update();

  if(KeyListener::key[ALLEGRO_KEY_ESCAPE] || levelSelectUI.getElementById("btnBack") -> clicked())
    set_next_state(STATE_MENU);

  if(levelSelectUI.getElementById("btnReallyReset") -> clicked()){

  }
  if(levelSelectUI.getElementById("btnResetSave") -> clicked()){
    reset_game_menu=true;
    levelSelectUI.getElementById("btnCancel") -> show();
    levelSelectUI.getElementById("btnCancel") -> enable();
    levelSelectUI.getElementById("btnReallyReset") -> show();
    levelSelectUI.getElementById("btnReallyReset") -> enable();
  }

  if(levelSelectUI.getElementById("btnResetSave") -> hover() && !Config::getIntValue("joystick_mode")){
    highlight_y_destination = 695;
  }

  if(levelSelectUI.getElementById("btnBack") -> hover() && !Config::getIntValue("joystick_mode")){
    highlight_y_destination = 65;
  }

   if(levelSelectUI.getElementById("btnCancel") -> hover() && !Config::getIntValue("joystick_mode") && reset_game_menu){
    highlight_game_reset_y_destination = 695;
  }

  if(levelSelectUI.getElementById("btnReallyReset") -> hover() && !Config::getIntValue("joystick_mode") && reset_game_menu){
    highlight_game_reset_y_destination = 650;
  }




  if((JoystickListener::buttonReleased[JOY_XBOX_A] && reset_game_menu && highlight_game_reset_y_destination==650)
   || levelSelectUI.getElementById("btnReallyReset") -> clicked()){

    for(int i=0; i<16; i++)
      completed_level_list[i]=false;
    writeLevelData();
    set_next_state(STATE_LEVELSELECT);

  }

  if((JoystickListener::buttonReleased[JOY_XBOX_A] && reset_game_menu && highlight_game_reset_y_destination==695)
   || levelSelectUI.getElementById("btnCancel") -> clicked()){
    JoystickListener::clearButtons();
    reset_game_menu=false;

    levelSelectUI.getElementById("btnCancel") -> hide();
    levelSelectUI.getElementById("btnCancel") -> disable();
    levelSelectUI.getElementById("btnReallyReset") -> hide();
    levelSelectUI.getElementById("btnReallyReset") -> disable();
  }

  if(JoystickListener::buttonReleased[JOY_XBOX_B]){
    if(reset_game_menu){
      reset_game_menu=false;
      levelSelectUI.getElementById("btnCancel") -> hide();
      levelSelectUI.getElementById("btnCancel") -> disable();
      levelSelectUI.getElementById("btnReallyReset") -> hide();
      levelSelectUI.getElementById("btnReallyReset") -> disable();
    }else
      set_next_state(STATE_MENU);

  }

  if(JoystickListener::buttonReleased[JOY_XBOX_A] && !reset_game_menu){

    if(highlight_y_destination==65){
      set_next_state(STATE_MENU);
    }else if(highlight_y_destination==695){

      reset_game_menu=true;
      levelSelectUI.getElementById("btnCancel") -> show();
      levelSelectUI.getElementById("btnCancel") -> enable();
      levelSelectUI.getElementById("btnReallyReset") -> show();
      levelSelectUI.getElementById("btnReallyReset") -> enable();

    }else{
      int level=(highlight_y_destination-65)/45;
      Config::setValue("level_to_start", level);
      set_next_state(STATE_GAME);
    }
  }


  if(!Config::getIntValue("joystick_mode")){
    for(int i=1; i<14; i++){
      if(levelSelectUI.getUIElements().at(i) -> hover()){
        highlight_y_destination=65+45*i;
      }
    }
  }


  for(int i=1; i<14; i++){
    if(levelSelectUI.getUIElements().at(i) -> clicked()){
      Config::setValue("level_to_start", i);
      set_next_state(STATE_GAME);
    }
  }
}