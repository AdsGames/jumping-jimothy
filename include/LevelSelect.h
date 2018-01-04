#ifndef LEVELSELECT_H
#define LEVELSELECT_H

#include <state.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "UIHandler.h"
#include "UIElement.h"
#include "button.h"


class LevelSelect : public state
{
  public:
    LevelSelect();
    virtual ~LevelSelect();

    void draw();
    void update();

  protected:

  private:

  ALLEGRO_FONT *levelselect_font;
  UIHandler levelSelectUI;

};

#endif // LEVELSELECT_H
