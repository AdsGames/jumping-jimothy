#ifndef BOX_H
#define BOX_H
#include <allegro5/allegro.h>
#include <tools.h>

#define BOX 0
#define GOAT 2

class b2World;
class b2Body;
class b2BodyDef;
class b2Vec2;
class Sensor;
class Character;

class Box{
  public:
    Box();
    ~Box();
    void init(float, float, float, float,float,float,bool,ALLEGRO_BITMAP*, b2World *);
    void init(float, float, ALLEGRO_BITMAP*, b2World *,Character*);
    bool getGoatWin();
    virtual void draw();
    void setStatic();
    void setDynamic();
    void setOrientation(int);
    virtual void update();
    int getType();
    float getX();
    float getY();
    b2Body *getBody();

  protected:

    Sensor *sensor_box = nullptr;
    bool static_box;

    Character *gameCharacter;

    ALLEGRO_BITMAP *sprite;
    ALLEGRO_BITMAP *goat_images[17];

    bool static_mode;
    int type;
    int orientation;
    int goat_frame;
    int goat_tick;

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
