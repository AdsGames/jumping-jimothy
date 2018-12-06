#include "game/Box.h"

#include <allegro5/allegro_primitives.h>
#include <allegro5/color.h>

#include <Box2D/Box2D.h>

#include "game/Sensor.h"
#include "game/Character.h"

#include "util/Tools.h"
#include "util/Globals.h"

// Construct
Box::Box() {
  // Nullify sprite
  sprite = nullptr;

  orientation = 0;

  isPaused = true;

  static_velocity = b2Vec2(0, 0);
  static_angular_velocity = 0.0f;

  color = al_map_rgb(0, 0, 0);

  gameWorld = nullptr;
  body = nullptr;

  initial_angle = 0.0f;
  initial_position = b2Vec2(0, 0);
  initial_size = b2Vec2(0, 0);
}

// Detailed constructor
Box::Box(const float x, const float y, const float width, const float height, b2World *world) :
  Box() {
  initial_position = b2Vec2(x, y);
  initial_size = b2Vec2(width, height);
  gameWorld = world;
  createBody();
}

// Set images
void Box::setImage(ALLEGRO_BITMAP* image) {
  sprite = image;
}

// Create body
void Box::createBody() {
  // World must be set
  if (!gameWorld)
    return;

  // Body definition
  b2BodyDef bodyDef;

  // Body position
	bodyDef.position.Set(getX(), getY());

	// Shape definition
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(getWidth() / 2, getHeight() / 2);

	// Fixture definition
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;

	// Set the box density to be non-zero, so it will be dynamic.
	fixtureDef.density = 1.0f;

	// Override the default friction.
	fixtureDef.friction = 0.3f;

	// Create body and create fixture
	body = gameWorld -> CreateBody(&bodyDef);
	body -> CreateFixture(&fixtureDef);
}

// Set static mode
void Box::setPaused(const bool pause) {
  // Must be pausable
  if (!isPausable())
    return;

  // Set paused state
  isPaused = pause;

  // Body must be defined
  if (!body)
    return;

  if (isPaused) {
    static_velocity = body -> GetLinearVelocity();
    static_angular_velocity = body -> GetAngularVelocity();
    body -> SetType( b2_staticBody);
  }
  else {
    isPaused = false;
    body -> SetType( b2_dynamicBody);
    if(isPausable() && (static_velocity.y<=0.01f && static_velocity.y>=-0.01f && static_velocity.x<=0.1f && static_velocity.x>=-0.1f && static_angular_velocity<=0.1f && static_angular_velocity>=-0.1f )){
      body -> SetAwake(false);
      body -> SetLinearVelocity(b2Vec2(0,0));
    }
    else{
      body -> SetLinearVelocity( static_velocity);
      body -> SetAngularVelocity( static_angular_velocity);
    }
  }
}

// Get x position
float Box::getX() {
  if (body)
    return body -> GetPosition().x;
  return initial_position.x;
};

// Get y position
float Box::getY() {
  if (body)
    return body -> GetPosition().y;
  return initial_position.y;
};

// Get width
float Box::getWidth() {
  //if (body)
  //  return body -> GetPosition().y;
  return initial_size.x;
}

// Get height
float Box::getHeight() {
  //if (body)
  //  return body -> GetPosition().y;
  return initial_size.y;
}

// Get angle
float Box::getAngle() {
  if (body)
    return body -> GetAngle();
  return 0.0f;
}

// Get physics body
b2Body* Box::getBody() {
  return body;
};

// Get physics body
bool Box::isPausable() {
  return false;
};

// Set orientation
void Box::setOrientation(const int orientation) {
  this -> orientation = orientation;
}
