#ifndef OPTIONS_H
#define OPTIONS_H

#include <string>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "rapidxml.hpp"
#include "rapidxml_print.hpp"

#include "UIHandler.h"
#include "UIElement.h"
#include "Button.h"
#include "State.h"
#include "KeyListener.h"
#include "JoystickListener.h"


class Options : public State
{
  public:
    Options();
    virtual ~Options();

    void draw();
    void update();
    void updateUI();

    static void read_data();
    void write_data();

    static bool sfx_enabled;
    static bool music_enabled;
    static bool draw_cursor;
    static std::string joystick_data;
    static void updateJoysticks(std::string newJoyData);
    static bool joystick_mode;

  protected:

  private:
    ALLEGRO_FONT *options_font;
    ALLEGRO_FONT *title_font;

    UIHandler OptionsUI;

    ALLEGRO_BITMAP *cursor;
    ALLEGRO_BITMAP *highlight;

    int highlight_y=100;;
    int highlight_y_destination=100;
    bool joystick_direction_hit=false;
    int temp_graphics_mode=0;



};

#endif // OPTIONS_H
