/**
 * Box
 * Danny Van Stemp
 * A base physics enabled box type
 * 05/05/2017
 **/

#ifndef BOX_H
#define BOX_H

#include <allegro5/allegro.h>
#include <box2d/box2d.h>

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
  Box(const float x,
      const float y,
      const float width,
      const float height,
      b2World* world);

  // Destructor
  virtual ~Box();

  // Create body
  void createBody(b2World* world);

  // Draw
  virtual void draw() = 0;

  // Update logic
  virtual void update(b2World* world) = 0;

  // Get type
  virtual int getType() = 0;

  // Set images
  void setImage(ALLEGRO_BITMAP* image);

  // Get X
  float getX() const;

  // Get Y
  float getY() const;

  // Get width
  float getWidth() const;

  // Get height
  float getHeight() const;

  // Get angle
  float getAngle() const;

  // Get physics body
  b2Body* getBody();

  // Set paused
  void setPaused(const bool pause);

  // Is pausable
  virtual bool isPausable();

  // Set orientation
  void setOrientation(const int orientation);

 protected:
  // Orientation
  int orientation{0};

  // Colour
  ALLEGRO_COLOR color;

  // Sprite for box
  ALLEGRO_BITMAP* sprite{nullptr};

  // Pointer to physics body
  b2Body* body{nullptr};

  // Paused state
  bool isPaused{true};

  // Snapshot of velocity
  b2Vec2 paused_velocity{b2Vec2(0, 0)};

  // Snapshot of angular velocity
  float paused_angular_velocity{0.0f};

  // Position
  b2Vec2 initial_position{b2Vec2(0, 0)};

  // Size
  b2Vec2 initial_size{b2Vec2(0, 0)};

  // Angle
  float initial_angle{0.0f};
};

#endif  // BOX_H
