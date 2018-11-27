/**
 * Menu
 * Menu state
 * Danny Van Stemp
 * 06/05/2017
**/

#ifndef MENU_H
#define MENU_H

#include <allegro5/allegro_image.h>

#include "State.h"
#include "ui/Button.h"
#include "util/Sound.h"

enum{
  menu_button_play,
  menu_button_exit,
  menu_button_edit,
  menu_button_help,
  menu_button_options,
};

// Menu
class Menu : public State {
  public:
    Menu();
    virtual ~Menu();

    void update();
    void draw();

  private:

    static const int BUTTON_COUNT = 5;
    Button menu_buttons[BUTTON_COUNT];

    // Images
    ALLEGRO_BITMAP *title;
    ALLEGRO_BITMAP *title_overlay;
    ALLEGRO_BITMAP *title_shine;
    ALLEGRO_BITMAP *prompt_image;
    ALLEGRO_BITMAP *highlight;

    ALLEGRO_BITMAP *cursor;

    ALLEGRO_BITMAP *logo;

    ALLEGRO_FONT *menu_font;
    ALLEGRO_FONT *credits_font;
    ALLEGRO_FONT *button_font;

    int highlight_y_destination = 500;
    int highlight_y = 500;

    bool joystick_direction_hit = false;
    bool credits_menu;
    int counter_title;
    int counter_play;
    bool counter_prompt;

};

#endif // MENU_H
