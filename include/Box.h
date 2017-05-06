#ifndef BOX_H
#define BOX_H
#include <allegro5/allegro.h>
#include <tools.h>

#define BOX 0

class b2World;
class b2Body;
class b2BodyDef;
class b2Vec2;

class Box{
  public:
    Box();
    ~Box();
    void init(float, float, float, float,float,float,bool,ALLEGRO_BITMAP*, b2World *);
    virtual void draw();
    void setStatic();
    void setDynamic();
    virtual void update();
    int getType();
    float getX();
    float getY();
    b2Body *getBody();

  protected:

    bool static_box;

    ALLEGRO_BITMAP *sprite;

    bool static_mode;
    int type;

    b2Vec2 static_velocity;
    float static_angular_velocity;

    float x;
    float y;
    float angle;
    ALLEGRO_COLOR color;

    float width;
    float height;

    b2World *gameWorld;
    b2Body *body;






};


#endif // BOX_H
