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
    Sensor(float x, float y,float width,float height);
    virtual ~Sensor() {};

    bool isColliding();

    virtual void update() override {};
    virtual void draw() override;

    bool isCollidingWithDynamicBody();
    bool isCollidingWithBody(b2Body*);
    void init(b2World *, b2Body *);

    // Get type
    virtual int getType() override;
};

#endif // SENSOR_H
