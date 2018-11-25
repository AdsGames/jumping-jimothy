#ifndef EXPLOSIVE_H
#define EXPLOSIVE_H

#include "Character.h"
#include "Box.h"

class Explosive : public Box {
  public:
    Explosive() {};
    virtual ~Explosive() {};
    void draw();
    void update();

    void init(float, float,int,ALLEGRO_BITMAP*, bool,b2World *, Character *);
    void applyBlastImpulse(b2Body* body, b2Vec2 blastCenter, b2Vec2 applyPoint, float blastPower);

  protected:

    Character *gameCharacter;
    int numRays;
    int blastRadius;
    int blastPower;
    bool affect_character;
    bool is_exploding;

  private:
};

#endif // EXPLOSIVE_H
