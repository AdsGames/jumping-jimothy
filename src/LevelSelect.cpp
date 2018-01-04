#include "LevelSelect.h"

LevelSelect::LevelSelect()
{
  levelselect_font = al_load_ttf_font( "fonts/munro.ttf", 36, 0);

  levelSelectUI = UIHandler();

  levelSelectUI.addElement(new UIElement(20,20,"Select a level",levelselect_font));
  levelSelectUI.getElementByText("Select a level") -> setVisibleBackground(false);
  levelSelectUI.getElementByText("Select a level") -> setTextColour(al_map_rgb(255,255,255));

  levelSelectUI.addElement(new Button(100, 100, "Level 1", levelselect_font));


}

LevelSelect::~LevelSelect()
{
  //dtor
}

void LevelSelect::draw(){
    al_clear_to_color( al_map_rgb(50,50,50));
    levelSelectUI.draw();



}
void LevelSelect::update(){
  levelSelectUI.update();


}
