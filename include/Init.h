/**
 * INIT
 * Allan Legemaate
 * 09/05/2017
**/
#ifndef INIT_H
#define INIT_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include "State.h"

class Init : public State
{
  public:
    Init();
    ~Init(){};

    void update(){};
    void draw(){}

  protected:

  private:
};

#endif // INIT_H
