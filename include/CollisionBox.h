#ifndef COLLISIONBOX_H
#define COLLISIONBOX_H

#include "Box.h"

class CollisionBox : public Box {
  public:
    CollisionBox() {};
    void init(float,float,float,float,b2World *);
    virtual ~CollisionBox() {};
    void draw();
    void update();
};

#endif // COLLISIONBOX_H
