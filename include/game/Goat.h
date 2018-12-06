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
    Goat(const float x, const float y, Character *character, b2World *world);
    virtual ~Goat() {};

    virtual void draw() override;
    bool getWinCondition();

    // Update logic
    virtual void update() override {};

    // Get type
    virtual int getType() override;
  private:
    // Sensor box pointer
    Sensor *sensor_box;

    // Character pointer
    Character *gameCharacter;

    // Frame of animation
    int goat_frame;

    // Counter for incrementing frame
    int goat_tick;

    // Images of goat
    ALLEGRO_BITMAP *goat_images[17];
};

#endif // GOAT_H
