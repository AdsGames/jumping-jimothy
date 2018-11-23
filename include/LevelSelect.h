#ifndef LEVELSELECT_H
#define LEVELSELECT_H

#include <allegro5/allegro_color.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "rapidxml.hpp"
#include "rapidxml_print.hpp"

#include "State.h"
#include "UIHandler.h"
#include "UIElement.h"
#include "Button.h"
#include "KeyListener.h"
#include "Game.h"
#include "Tools.h"


class LevelSelect : public State
{
  public:
    LevelSelect();
    virtual ~LevelSelect();

    void draw();
    void update();

    static void setLevelComplete(int urmom);


    static bool completed_level_list[16];
    static void writeLevelData();

  protected:

  private:

    ALLEGRO_BITMAP *cursor;
    ALLEGRO_BITMAP *highlight_levelselect;
    ALLEGRO_BITMAP *highlight;

    void createLevelButton(int,int,int);

    ALLEGRO_FONT *levelselect_font;
    ALLEGRO_FONT *levelselect_font_large;
    UIHandler levelSelectUI;

    int highlight_y=110;
    int highlight_y_destination=110;
    int highlight_game_reset_y=650;
    int highlight_game_reset_y_destination=650;


    bool joystick_direction_hit;
    bool reset_game_menu=false;
};

#endif // LEVELSELECT_H
