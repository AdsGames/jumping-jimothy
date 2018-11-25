/**
 * Static Box
 * Danny Vanstemp
 * Physics box not effected by gravity
 * 05/05/2017
**/

#ifndef STATICBOX_H
#define STATICBOX_H

#include "Box.h"

class StaticBox : public Box {
  public:
    StaticBox() {};
    virtual ~StaticBox() {};

    void init(float,float,ALLEGRO_BITMAP*, ALLEGRO_BITMAP*, ALLEGRO_BITMAP*, ALLEGRO_BITMAP*);
    void draw();
    void update();

    int getType();;
};

#endif // STATICBOX_H
