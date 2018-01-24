#ifndef OPTIONS_H
#define OPTIONS_H

#include <state.h>
#include "UIHandler.h"
#include "UIElement.h"
#include "button.h"
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_ttf.h"

#include "rapidxml.hpp"
#include "rapidxml_print.hpp"
#include "keyListener.h"


class Options : public state
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
    static int graphics_mode;
    static bool draw_cursor;


  protected:

  private:

    ALLEGRO_FONT *options_font;
    UIHandler OptionsUI;

    ALLEGRO_BITMAP *cursor;



};

#endif // OPTIONS_H
