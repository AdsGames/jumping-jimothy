/**
 * Game
 * Allan Legemaate and Danny Van Stemp
 * Game state
 * 05/05/2017
**/

#ifndef GAME_H
#define GAME_H

#include <vector>
#include <Box2D/Box2D.h>

#include "State.h"

#include "Box.h"
#include "Character.h"
#include "Goat.h"
#include "util/Sound.h"
#include "ui/Button.h"

class Game : public State {
  public:
    // Construct / destruct
    Game();
    ~Game();

    // Override parent
    void update();
    void draw();
    bool level_complete();

    // Test mode
    static bool testing;
    static const char *testing_file_name;

  private:
    // Functions
    void load_world(int);
    void b2_setup();
    void load_sprites();
    void reset();

    // Creation code
    Box *create_dynamic_box(float newX, float newY, float newWidth, float newHeight,float newVelX,float newVelY,ALLEGRO_BITMAP*, bool newBodyType, bool newIsSensor);
    Box *create_explosive_box(float,float,int,bool);
    Box *create_static_box(float newX, float newY,ALLEGRO_BITMAP*,ALLEGRO_BITMAP*, ALLEGRO_BITMAP*, ALLEGRO_BITMAP*);
    Box *create_collision_box(float newX, float newY,float,float);

    Goat *create_goat(float,float);
    Character *create_character(float newX, float newY);

    // Our character and goat
    Goat *gameGoat = nullptr;
    Character *gameCharacter = nullptr;

    // Game variables
    std::vector<Box*> gameBoxes;
    Box *newBox;
    Box *rootBox;
    b2Body* groundBody;

    int level;
    bool first_play;
    bool static_mode;

    std::vector<std::string> help_text;

    ALLEGRO_FONT *game_font;
    ALLEGRO_FONT *help_font;
    ALLEGRO_FONT *edit_font;

    // Bitmaps
    ALLEGRO_BITMAP *box;
    ALLEGRO_BITMAP *box_repel;
    ALLEGRO_BITMAP *box_repel_direction;
    ALLEGRO_BITMAP *character;
    ALLEGRO_BITMAP *goat_map;
    ALLEGRO_BITMAP *play;
    ALLEGRO_BITMAP *pause;

    ALLEGRO_BITMAP *new_dynamic_tile[100];

    //TODO (Danny#1#): FIX WHEN NOT DEAD INSIDE

    // Samples
    Sound toggle_off;
    Sound toggle_on;
    Sound death;
    Button testing_back_button;

    // Box2D world parameters
    b2Vec2 gravity;
    float32 timeStep;
    int32 velocityIterations;
    int32 positionIterations;
    b2World *gameWorld;
    bool doSleep;
};

#endif // GAME_H
