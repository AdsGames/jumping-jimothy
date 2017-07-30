#ifndef COLLISIONBOX_H
#define COLLISIONBOX_H

#include <Box.h>
#include <allegro5/allegro_primitives.h>


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
