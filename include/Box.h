#ifndef BOX_H
#define BOX_H
#include <allegro5/allegro.h>
#define BOX 0

class b2World;
class b2Body;
class b2BodyDef;
class b2Vec2;

class Box{
  public:
    Box();
    ~Box();
    void init(float, float, float, float, bool, b2World *);
    void draw();
    virtual void update();
    int getType();
    float getX();
    float getY();
    b2Body *getBody();

  protected:

    int type;

    float x;
    float y;
    float angle;
    ALLEGRO_COLOR color = al_map_rgb(0,0,0);

    float width;
    float height;

    b2World *gameWorld;
    b2Body *body;






};


#endif // BOX_H
