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
#include <allegro5/allegro.h>
#include <allegro5/bitmap.h>

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
    bool level_complete();
  //DANNNNNNYYYYYYYYYYYYYYYY
  //SULLLYYYYYYYYYYYYYYYYYYY

  protected:

  private:
    // Functions
    void b2_setup();
    void load_sprites();
    void load_world(int);
    void reset();

    Box *create_box(float newX, float newY, float newWidth, float newHeight,float newVelX,float newVelY,ALLEGRO_BITMAP* newSprite, bool newBodyType, bool newIsSensor);
    Character *create_character(float newX, float newY);
    Box *create_goat(float,float);

    Box *goat;
    Character *gameCharacter;

    int level=1;

    // Game variables
    std::vector<Box*> gameBoxes;
    Box *newBox = nullptr;
    Box *rootBox = nullptr;
    b2Body* groundBody;

    bool static_mode = false;

    ALLEGRO_BITMAP *box;
    ALLEGRO_BITMAP *character;
    ALLEGRO_BITMAP *goat_sprite;
    ALLEGRO_BITMAP *goat_map;
    ALLEGRO_BITMAP *static_tileset;
    ALLEGRO_BITMAP *tiles[4][48];
    ALLEGRO_BITMAP *play;
    ALLEGRO_BITMAP *pause;

    // Box2D world parameters
    b2Vec2 gravity;
    float32 timeStep;
    int32 velocityIterations;
    int32 positionIterations;
    bool doSleep;

    b2World *gameWorld;
};

#endif // GAME_H
