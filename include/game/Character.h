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

#define NUM_CHARACTER_IMAGES 20

class b2World;
class b2Body;
class b2BodyDef;
class b2Vec2;
class keyListener;

class Character : public Box{
  public:
    Character() {};
    virtual ~Character();

    void init( float newX, float newY,ALLEGRO_BITMAP *newSprite, b2World *newGameWorld);
    void draw();
    void update();
    b2Body *getSensorBody(){return sensor_box->getBody();}

  private:
    int tick;
    int frame;
    int timer_sound_delay;
    int timer_jump_delay;
    int counter_sensor_contact;

    bool landed;

    bool initial_key_release;

    float velocity_old;

    bool direction;

    Sensor *sensor_box;

    ALLEGRO_BITMAP *sprite;
    ALLEGRO_BITMAP *sprites[NUM_CHARACTER_IMAGES];

    Sound jump;
    Sound land;
};


#endif // CHARACTER_H
