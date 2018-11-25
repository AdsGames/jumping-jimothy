/**
 * Sensor
 * Danny Vanstemp
 * Senses physics bodies
 * 05/05/2017
**/

#ifndef SENSOR_H
#define SENSOR_H

#include "Box.h"

class b2World;
class b2Body;
class b2BodyDef;
class b2Vec2;
class keyListener;
class b2JointDef;
class b2WeldJointDef;

class Sensor : public Box{
  public:
    void draw();
    Sensor();
    virtual ~Sensor();
    bool isColliding();
	bool isCollidingWithDynamicBody();
    bool isCollidingWithBody(b2Body*);
    void init(float, float,float,float,ALLEGRO_COLOR, b2World *, b2Body *);
};

#endif // SENSOR_H
