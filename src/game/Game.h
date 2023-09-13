/**
 * Game
 * Allan Legemaate and Danny Van Stemp
 * Game state
 * 05/05/2017
 **/

#ifndef GAME_H
#define GAME_H

#include <box2d/box2d.h>
#include <vector>

#include "../State.h"

#include "../ui/Button.h"
#include "../util/Sound.h"
#include "Box.h"
#include "Character.h"
#include "Goat.h"

#include "World.h"

class Game : public State {
 public:
  // Construct / destruct
  Game();
  ~Game();

  // Override parent
  void update(StateEngine* engine);
  void draw();
  bool level_complete();

 private:
  // Functions
  void load_world(std::string file);
  void load_sprites();
  void reset();

  // Creation code
  Box* create_dynamic_box(float x,
                          float y,
                          float velX,
                          float velY,
                          ALLEGRO_BITMAP*);
  Box* create_explosive_box(float, float, int, bool);
  Box* create_static_box(float x, float y, ALLEGRO_BITMAP*);
  Box* create_collision_box(float x, float y, float, float);

  Goat* create_goat(float, float);
  Character* create_character(float x, float y);

  // Our character and goat
  Goat* gameGoat{nullptr};
  Character* gameCharacter{nullptr};

  // Game variables
  std::vector<Box*> gameBoxes;

  int level;
  bool first_play;
  bool static_mode;

  std::vector<std::string> help_text;

  ALLEGRO_FONT* game_font{nullptr};
  ALLEGRO_FONT* help_font{nullptr};
  ALLEGRO_FONT* edit_font{nullptr};

  // Bitmaps
  ALLEGRO_BITMAP* box{nullptr};
  ALLEGRO_BITMAP* box_repel{nullptr};
  ALLEGRO_BITMAP* box_repel_direction{nullptr};
  ALLEGRO_BITMAP* character{nullptr};
  ALLEGRO_BITMAP* goat_map{nullptr};
  ALLEGRO_BITMAP* play{nullptr};
  ALLEGRO_BITMAP* pause{nullptr};

  ALLEGRO_BITMAP* new_dynamic_tile[100];

  // TODO (Danny#1#): FIX WHEN NOT DEAD INSIDE

  // Samples
  Sound toggle_off;
  Sound toggle_on;
  Sound death;
  Button* testing_back_button{nullptr};

  // Box 2d game world
  World* world{nullptr};
};

#endif  // GAME_H
