/**
 * Level Select
 * Danny Van Stemp
 * Level select state
 * 04/01/2018
**/

#ifndef LEVELSELECT_H
#define LEVELSELECT_H

#include <allegro5/allegro_font.h>

#include "State.h"
#include "ui/UIHandler.h"

class LevelSelect : public State {
  public:
    LevelSelect();
    virtual ~LevelSelect() {};

    void draw();
    void update();

  private:
    void createLevelButton(int,int,int);

    ALLEGRO_FONT *levelselect_font;
    ALLEGRO_FONT *levelselect_font_large;

    UIHandler levelSelectUI;

    bool reset_game_menu;
};

#endif // LEVELSELECT_H
