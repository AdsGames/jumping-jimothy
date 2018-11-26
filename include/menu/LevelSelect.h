/**
 * Level Select
 * Danny Van Stemp
 * Level select state
 * 04/01/2018
**/

#ifndef LEVELSELECT_H
#define LEVELSELECT_H

#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>

#include "State.h"
#include "ui/UIElement.h"
#include "ui/UIHandler.h"

class LevelSelect : public State {
  public:
    LevelSelect();
    virtual ~LevelSelect() {};

    void draw();
    void update();

  private:
    ALLEGRO_BITMAP *cursor;
    ALLEGRO_BITMAP *highlight_levelselect;
    ALLEGRO_BITMAP *highlight;

    void createLevelButton(int,int,int);

    ALLEGRO_FONT *levelselect_font;
    ALLEGRO_FONT *levelselect_font_large;
    UIHandler levelSelectUI;

    int highlight_y;
    int highlight_y_destination;
    int highlight_game_reset_y;
    int highlight_game_reset_y_destination;

    bool joystick_direction_hit;
    bool reset_game_menu;
};

#endif // LEVELSELECT_H
