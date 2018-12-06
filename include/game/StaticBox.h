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
    // Constructor
    StaticBox(float x, float y);

    // Destructor
    virtual ~StaticBox() {};

    // Draw
    virtual void draw() override;

    // Update
    virtual void update() override {};

    // Get type
    virtual int getType() override;
};

#endif // STATICBOX_H
