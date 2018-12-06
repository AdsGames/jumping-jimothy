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
    Sensor(const float x, const float y, const float width, const float height, b2Body *parentBody, b2World *world);
    virtual ~Sensor() {};

    bool isColliding();

    virtual void update(b2World *world) override {};
    virtual void draw() override;

    bool isCollidingWithDynamicBody();
    bool isCollidingWithBody(b2Body*);

    // Get type
    virtual int getType() override;
};

#endif // SENSOR_H
