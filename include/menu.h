#ifndef MENU_H
#define MENU_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#include "keyListener.h"
#include "joystickListener.h"
#include "tools.h"
#include "state.h"

// Menu
class menu : public state{
  public:
    menu();
    virtual ~menu();

    void update();
    void draw();

  private:
    // Images
    ALLEGRO_BITMAP *title;
    ALLEGRO_BITMAP *play;
    ALLEGRO_BITMAP *prompt_image;
    ALLEGRO_BITMAP *play_images[50];
    ALLEGRO_BITMAP *title_images[170];

    int counter_title;
    int counter_play;
    bool counter_prompt;
};

#endif // MENU_H
