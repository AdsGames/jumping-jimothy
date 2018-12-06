/**
 * Box
 * Danny Van Stemp
 * A base physics enabled box type
 * 05/05/2017
**/

#ifndef BOX_H
#define BOX_H

#include <allegro5/allegro.h>
#include <Box2D/Box2D.h>

#define NUM_TILES 4

class b2World;
class b2Body;
class b2BodyDef;
class b2Vec2;
class Sensor;
class Character;

class Box {
  public:
    // Constructor
    Box();
    Box(float x, float y, float width, float height);

    // Destructor
    virtual ~Box() {};

    // Create body
    void createBody(int bodyType, bool fixedRotation);

    // Draw
    virtual void draw() = 0;

    // Update logic
    virtual void update() = 0;

    // Get type
    virtual int getType() = 0;

    // Setters
    virtual void setStatic(bool stat);
    void setImages(ALLEGRO_BITMAP* img_1, ALLEGRO_BITMAP* img_2, ALLEGRO_BITMAP* img_3, ALLEGRO_BITMAP* img_4);

    // Getters
    float getX();
    float getY();
    b2Body *getBody();

    // Is pausable
    virtual bool isPausable();
  protected:
    float x;
    float y;
    float angle;
    int orientation;

    ALLEGRO_COLOR color;

    float width;
    float height;


    ALLEGRO_BITMAP *sprite;
    ALLEGRO_BITMAP *new_tiles[NUM_TILES];

    bool static_mode;

    b2Vec2 static_velocity;
    float static_angular_velocity;

    b2World *gameWorld;
    b2Body *body;

  private:
    void setPosition(float x, float y);
    void setSize(float width, float height);
};


#endif // BOX_H
