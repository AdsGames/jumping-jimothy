/**
 * Explosive
 * Danny Van Stemp
 * Box that applys gravity in a direction
 * 21/09/2017
**/

#ifndef EXPLOSIVE_H
#define EXPLOSIVE_H

#include "Character.h"
#include "Box.h"

class Explosive : public Box {
  public:
    Explosive(float x, float y, bool affectCharacter, Character *character, b2World *world);
    virtual ~Explosive() {};
    virtual void draw() override;
    virtual void update() override;

    void applyBlastImpulse(b2Body* body, b2Vec2 blastCenter, b2Vec2 applyPoint, float blastPower);

    // Get type
    virtual int getType() override;
  protected:

    Character *gameCharacter;
    int blastRadius;
    int blastPower;
    bool affect_character;
    bool is_exploding;

  private:
};

#endif // EXPLOSIVE_H
