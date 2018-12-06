/**
 * Goat
 * Danny Van Stemp
 * The game goal
 * 06/05/2017
**/

#ifndef GOAT_H
#define GOAT_H

#include <allegro5/allegro_primitives.h>

#include "Box.h"
#include "Character.h"
#include "Sensor.h"

class Goat : public Box {
  public:
    Goat(float x, float y);
    virtual ~Goat() {};

    void init(ALLEGRO_BITMAP *image, b2World *world, Character *character);
    virtual void draw() override;
    bool getWinCondition();

    // Update logic
    virtual void update() override {};

    // Get type
    virtual int getType() override;
  private:
    Sensor *sensor_box = nullptr;
    Character *gameCharacter = nullptr;

    int goat_frame;
    int goat_tick;
    ALLEGRO_BITMAP *goat_images[17];
};

#endif // GOAT_H
