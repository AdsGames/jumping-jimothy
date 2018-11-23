#ifndef OPTIONS_H
#define OPTIONS_H

#include <string>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "UIHandler.h"
#include "UIElement.h"
#include "Button.h"
#include "State.h"
#include "KeyListener.h"
#include "JoystickListener.h"


class Options : public State {
  public:
    Options();
    virtual ~Options();

    void draw();
    void update();
    void updateUI();

  protected:

  private:
    ALLEGRO_FONT *options_font;
    ALLEGRO_FONT *title_font;

    UIHandler OptionsUI;

    ALLEGRO_BITMAP *cursor;
    ALLEGRO_BITMAP *highlight;

    int highlight_y = 100;
    int highlight_y_destination = 100;
    bool joystick_direction_hit = false;
    int temp_graphics_mode = 0;

};

#endif // OPTIONS_H
