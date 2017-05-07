#ifndef MENU_H
#define MENU_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include <vector>
#include <string>

#include "mouseListener.h"
#include "keyListener.h"
#include "tools.h"

#include "state.h"


// Menu
class menu : public state{
  public:
    menu();
    virtual ~menu();

    void update();
    void draw();

  protected:
    ALLEGRO_BITMAP *play_images[50];
    ALLEGRO_BITMAP *title_images[170];
    ALLEGRO_BITMAP *pressthing_images[1];

    int counter_title;
    int counter_play;

  private:
};

#endif // MENU_H
