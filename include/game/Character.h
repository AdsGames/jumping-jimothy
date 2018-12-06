/**
 * Character
 * Danny Van Stemp
 * The game character
 * 05/05/2017
**/

#ifndef CHARACTER_H
#define CHARACTER_H

#include "Sensor.h"
#include "util/Sound.h"

class b2World;
class b2Body;
class b2BodyDef;
class b2Vec2;
class keyListener;

// Number of character images
const int NUM_CHARACTER_IMAGES = 20;

class Character : public Box{
  public:
    Character(const float x, const float y, b2World *world);
    virtual ~Character() {};

    virtual void draw() override;
    virtual void update() override;
    b2Body *getSensorBody(){return sensor_box->getBody();}

    // Get type
    virtual int getType() override;

  private:
    // Animation tick
    int tick;

    // Animation frame
    int frame;

    // Sound delay for jump sound
    int timer_sound_delay;

    // Jump delay
    int timer_jump_delay;

    // Sensing counter
    int counter_sensor_contact;

    // Landed on ground
    bool landed;

    // Previous velocity
    float velocity_old;

    // Direciton
    bool direction;

    // Sensor for collisions
    Sensor *sensor_box;

    // Character images
    ALLEGRO_BITMAP *sprites[NUM_CHARACTER_IMAGES];

    // Sample for jump
    Sound jump;

    // Sample for landing
    Sound land;
};


#endif // CHARACTER_H
