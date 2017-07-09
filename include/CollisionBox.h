#ifndef COLLISIONBOX_H
#define COLLISIONBOX_H

#include <Box.h>


class CollisionBox : public Box
{
  public:
    CollisionBox();
    virtual ~CollisionBox();
    void draw();
    void update();
  protected:

  private:
};

#endif // COLLISIONBOX_H
