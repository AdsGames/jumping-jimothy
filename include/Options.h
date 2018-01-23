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
    void updateUI();

    static void read_data();
    void write_data();

    static bool sfx_enabled;
    static bool music_enabled;


  protected:

  private:

    ALLEGRO_FONT *options_font;
    UIHandler OptionsUI;



};

#endif // OPTIONS_H
