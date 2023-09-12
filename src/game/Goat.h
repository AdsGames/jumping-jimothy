/**
 * Goat
 * Danny Van Stemp
 * The game goal
 * 06/05/2017
 **/

#ifndef GOAT_H
#define GOAT_H

#include <allegro5/allegro_primitives.h>
#include <array>
#include <memory>

#include "Box.h"
#include "Character.h"
#include "Sensor.h"

class Goat : public Box {
 public:
  // Constructor
  Goat(const float x,
       const float y,
       Character* character,
       ALLEGRO_BITMAP* image,
       b2World* world);

  // Draw
  void draw() override;

  // Can win
  bool getWinCondition();

  // Update logic
  void update(b2World* world) override{
      // Unused
  };

  // Get type
  int getType() override;

 private:
  // Sensor box pointer
  std::unique_ptr<Sensor> sensor_box{nullptr};

  // Character pointer
  Character* gameCharacter{nullptr};

  // Frame of animation
  int goat_frame{0};

  // Counter for incrementing frame
  int goat_tick{0};

  // Images of goat
  std::array<ALLEGRO_BITMAP*, 17> goat_images{nullptr};
};

#endif  // GOAT_H
