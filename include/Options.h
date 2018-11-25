/*
 * Options
 * The options menu state. Works with config
 * Danny Van Stemp and Allan Legemaate
 * 22/11/2018
 */

#ifndef OPTIONS_H
#define OPTIONS_H

#include <string>
#include <allegro5/allegro_font.h>

#include "UIHandler.h"
#include "Button.h"
#include "State.h"

// Options state
class Options : public State {
  public:
    Options();
    virtual ~Options();

    void draw();
    void update();
    void updateUI();

  protected:

  private:
    ALLEGRO_FONT *options_font = nullptr;
    ALLEGRO_FONT *title_font = nullptr;

    UIHandler OptionsUI;

    ALLEGRO_BITMAP *cursor;
    ALLEGRO_BITMAP *highlight = nullptr;

    int highlight_y = 100;
    int highlight_y_destination = 100;
    bool joystick_direction_hit = false;
    int temp_graphics_mode = 0;

};

#endif // OPTIONS_H
