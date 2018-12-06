/**
 * Collision Box
 * Danny Van Stemp
 * A collidable box. Implemented for more
 *   precise collision.
 * 09/07/2017
**/

#ifndef COLLISIONBOX_H
#define COLLISIONBOX_H

#include "Box.h"

class CollisionBox : public Box {
  public:
    CollisionBox(float x, float y, float width, float height);
    void init(b2World *);
    virtual ~CollisionBox() {};
    void draw();
    void update() {};

    // Get type
    virtual int getType() override;
};

#endif // COLLISIONBOX_H
