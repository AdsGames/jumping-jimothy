#ifndef GOAT_H
#define GOAT_H

#include <allegro5/allegro_primitives.h>

#include "Box.h"
#include "Character.h"
#include "Sensor.h"

class Goat : public Box {
  public:
    void init(float newX, float newY, ALLEGRO_BITMAP *newSprite, b2World *newGameWorld, Character *newCharacter);
    virtual ~Goat();
    void draw();
    bool getWinCondition();

  private:
    Sensor *sensor_box = nullptr;
    Character *gameCharacter = nullptr;

    int goat_frame;
    int goat_tick;
    ALLEGRO_BITMAP *goat_images[17];
};

#endif // GOAT_H
