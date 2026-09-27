/**
 * Box
 * Danny Van Stemp
 * A base physics enabled box type
 * 05/05/2017
 **/

#pragma once

#include <asw/asw.h>
#include <box2d/box2d.h>

enum class BoxType { Dynamic, Character, Goat, Static, Collision, Explosive };

class Box {
 public:
  // Position is the centre, all values are in metres
  Box(float x, float y, float width, float height);

  // Bodies belong to the world and are freed with it
  virtual ~Box() = default;
  Box(const Box&) = delete;
  Box& operator=(const Box&) = delete;
  Box(Box&&) = delete;
  Box& operator=(Box&&) = delete;

  virtual void update(b2World& /*world*/) {}
  virtual void draw() const = 0;
  virtual BoxType getType() const = 0;

  // Pausable boxes freeze while time is stopped. When can_sleep is set, a box
  // that was at rest goes to sleep on unpause so it does not jitter.
  virtual bool isPausable() const { return false; }
  void setPaused(bool pause, bool can_sleep = true);

  float getX() const;
  float getY() const;
  float getWidth() const { return size.x; }
  float getHeight() const { return size.y; }
  float getAngle() const;
  b2Body* getBody() const { return body; }

 protected:
  void createBody(b2World& world, b2BodyType type);

  // Centre on screen in pixels
  asw::Vec2<float> screenPosition() const;

  // Screen quad of a w by h pixel image centred on the box, shifted by offset
  asw::Quad<float> screenQuad(float w,
                              float h,
                              asw::Vec2<float> offset = {0, 0}) const;

  // Screen angle, SDL rotates clockwise and Box2D counter clockwise
  float screenAngle() const { return -getAngle(); }

  b2Body* body{nullptr};

 private:
  bool is_paused{false};
  b2Vec2 paused_velocity{0, 0};
  float paused_angular_velocity{0};

  b2Vec2 initial_position;
  b2Vec2 size;
};
