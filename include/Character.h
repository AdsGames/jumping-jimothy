/**
 * CHARACTER
 * Danny Vanstemp
 * 05/05/2017
**/
#ifndef CHARACTER_H
#define CHARACTER_H

#include "globals.h"
#include "Box.h"
#include "keyListener.h"
#include "joystickListener.h"

class b2World;
class b2Body;
class b2BodyDef;
class b2Vec2;
class keyListener;
class Sensor;

class Character : public Box{
  public:
    Character();
    ~Character();
    void init( float newX, float newY,ALLEGRO_BITMAP *newSprite, b2World *newGameWorld);
    void draw();
    void update();

  private:
    int time_move_jump_timer_thingy;
    int tick;
    int frame;

    bool direction;

    Sensor *sensor_box;

    ALLEGRO_BITMAP *sprite;
    ALLEGRO_BITMAP *sprites[20];
};


#endif // CHARACTER_H
