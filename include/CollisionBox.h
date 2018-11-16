#ifndef COLLISIONBOX_H
#define COLLISIONBOX_H

#include <allegro5/allegro_primitives.h>

#include "Box.h"
#include "KeyListener.h"

class CollisionBox : public Box
{
  public:
    CollisionBox();
    void init(float,float,float,float,b2World *);
    virtual ~CollisionBox();
    void draw();
    void update();
  protected:

  private:
};

#endif // COLLISIONBOX_H
