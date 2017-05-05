#ifndef SENSOR_H
#define SENSOR_H

#include "globals.h"
#include "Box.h"
#include "keyListener.h"

class b2World;
class b2Body;
class b2BodyDef;
class b2Vec2;
class keyListener;
class b2JointDef;
class b2WeldJointDef;

class Sensor : public Box{
  public:
    Sensor();
    ~Sensor();
    bool isColliding();
    void init(float, float,float,float, b2World *, b2Body *);




};


#endif // SENSOR_H
