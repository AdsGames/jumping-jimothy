/**
 * Character
 * Danny Van Stemp
 * The game character
 * 05/05/2017
 **/

#ifndef CHARACTER_H
#define CHARACTER_H

#include "../util/Sound.h"
#include "Sensor.h"

#include <array>
#include <memory>

class b2World;
class b2Body;
class b2BodyDef;
class b2Vec2;
class keyListener;

// Number of character images
const int NUM_CHARACTER_IMAGES = 20;

class Character : public Box {
 public:
  // Constructor
  Character(const float x,
            const float y,
            ALLEGRO_BITMAP* image,
            b2World* world);

  // Draw
  void draw() override;

  // Update
  void update(b2World* world) override;

  // Get sensor body
  b2Body* getSensorBody();

  // Get type
  int getType() override;

 private:
  // Animation tick
  int tick{0};

  // Animation frame
  int frame{0};

  // Sound delay for jump sound
  int timer_sound_delay{0};

  // Jump delay
  int timer_jump_delay{0};

  // Sensing counter
  int counter_sensor_contact{0};

  // Landed on ground
  bool landed{false};

  // Previous velocity
  float velocity_old{0};

  // Direciton
  bool direction{false};

  // Sensor for collisions
  std::unique_ptr<Sensor> sensor_box;

  // Character images
  std::array<ALLEGRO_BITMAP*, NUM_CHARACTER_IMAGES> sprites{nullptr};

  // Sample for jump
  Sound jump;

  // Sample for landing
  Sound land;
};

#endif  // CHARACTER_H
