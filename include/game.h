#ifndef GAME_H
#define GAME_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <Box2D/Box2D.h>

#include <mouseListener.h>
#include <keyListener.h>
#include <joystickListener.h>

#include "state.h"
#include "rapidxml.hpp"
#include "rapidxml_print.hpp"

#include <Box.h>
#include <tools.h>
#include <Character.h>

class game : public state{
  public:
    // Construct/deconstruct
    game();
    ~game();

    // Override parent
    void update();
    void draw();

  protected:

  private:
    // Functions
    void b2_setup();
    void load_sprites();
    void load_world();

    void create_box(float newX, float newY, float newWidth, float newHeight,float newVelX,float newVelY, bool newBodyType, bool newIsSensor);
    void create_character(float newX, float newY);

    // Game variables
    std::vector<Box*> gameBoxes;
    bool static_mode = false;

    ALLEGRO_BITMAP *box;
    ALLEGRO_BITMAP *character;

    // Box2D world parameters
    b2Vec2 gravity;
    float32 timeStep;
    int32 velocityIterations;
    int32 positionIterations;
    bool doSleep;

    b2World *gameWorld;
};

#endif // GAME_H
