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


class Options : public state
{
  public:
    Options();
    virtual ~Options();

    void draw();
    void update();

    static void read_data();
    static void write_data();



  protected:

  private:

    ALLEGRO_FONT *options_font;
    UIHandler OptionsUI;

    static bool sfx_enabled;
    static bool music_enabled;

};

#endif // OPTIONS_H
