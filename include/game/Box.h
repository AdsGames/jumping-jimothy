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

    // Set images
    void setImage(ALLEGRO_BITMAP* image);

    // Get X
    float getX();

    // Get Y
    float getY();

    // Get physics body
    b2Body *getBody();

    // Set paused
    void setPaused(bool pause);

    // Is pausable
    virtual bool isPausable();

    // Set orientation
    void setOrientation(int orientation);

  protected:
    // Position
    float x, y;

    // Angle
    float angle;

    // Orientation
    int orientation;

    // Dimensions
    float width, height;

    // Colour
    ALLEGRO_COLOR color;

    ALLEGRO_BITMAP *sprite;
    ALLEGRO_BITMAP *new_tiles[NUM_TILES];

    bool isPaused;

    b2Vec2 static_velocity;
    float static_angular_velocity;

    b2World *gameWorld;
    b2Body *body;

  private:
    // Set position
    void setPosition(float x, float y);

    // Set size
    void setSize(float width, float height);
};


#endif // BOX_H
