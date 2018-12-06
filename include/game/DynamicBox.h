/**
 * Dyanmic Box
 * Danny Van Stemp
 * Physics based box that is influenced by
 *   gravity.
 * 30/07/2017
**/

#ifndef DYNAMICBOX_H
#define DYNAMICBOX_H

#include "Box.h"

class DynamicBox : public Box {
  public:
    DynamicBox(float x, float y);
    virtual ~DynamicBox() {};

    void init(float vel_x, float vel_y, ALLEGRO_BITMAP *image, b2World *world);

    // Updates
    virtual void draw() override;
    virtual void update() override {};

    // Setters
    virtual void setStatic(bool stat) override;
    void setOrientation(int);

    // Get type
    virtual int getType() override;

    // Is dynamic
    virtual bool isPausable() override;

  private:

};

#endif // DYNAMICBOX_H
