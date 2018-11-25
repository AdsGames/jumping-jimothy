#ifndef DYNAMICBOX_H
#define DYNAMICBOX_H

#include "Box.h"

class DynamicBox : public Box {
  public:
    DynamicBox() {};
    virtual ~DynamicBox() {};

    void init(float, float, float, float,float,float,bool,ALLEGRO_BITMAP*, b2World *);

    // Updates
    void draw();
    void update();

    // Setters
    void setStatic();
    void setDynamic(bool);
    void setOrientation(int);
};

#endif // DYNAMICBOX_H
