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
  sprite = nullptr;

  orientation = 0;
  angle = 0.0f;

  isPaused = true;

  static_velocity = b2Vec2(0, 0);
  static_angular_velocity = 0.0f;

  color = al_map_rgb(0, 0, 0);

  gameWorld = nullptr;
  body = nullptr;

  setPosition(0, 0);
  setSize(1, 1);
}

// Detailed constructor
Box::Box(float x, float y, float width, float height) :
  Box() {
  setPosition(x, y);
  setSize(width, height);
}

// Set images
void Box::setImage(ALLEGRO_BITMAP* image) {
  sprite = image;
}

// Create body
void Box::createBody(int bodyType, bool fixedRotation) {
  // World must be set
  if (!gameWorld)
    return;

  // Body
  b2BodyDef bodyDef;

  // Body types
  switch (bodyType) {
    case BODY_DYNAMIC:
      bodyDef.type = b2_dynamicBody;
      break;
    case BODY_KINEMATIC:
      bodyDef.type = b2_kinematicBody;
      break;
    default:
      bodyDef.type = b2_dynamicBody;
      break;
  }

	bodyDef.position.Set(x, y);
	body = gameWorld -> CreateBody(&bodyDef);

	if (fixedRotation)
    body -> SetFixedRotation(true);

	// Define another box shape for our dynamic body.
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(width/2, height/2);

	// Define the dynamic body fixture.
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;

	// Set the box density to be non-zero, so it will be dynamic.
	fixtureDef.density = 1.0f;

	// Override the default friction.
	fixtureDef.friction = 0.3f;

	// Add the shape to the body.
	body -> CreateFixture(&fixtureDef);
}

// Set static mode
void Box::setPaused(bool pause) {
  // Must be pausable
  if (!isPausable())
    return;

  // Set paused state
  isPaused = pause;

  // Body must be defined
  if (body) {
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
}

// Get x position
float Box::getX(){
  return x;
};

// Get y position
float Box::getY(){
  return y;
};

// Get physics body
b2Body* Box::getBody(){
  return body;
};

// Get physics body
bool Box::isPausable(){
  return false;
};

// Set orientation
void Box::setOrientation(int orientation) {
  this -> orientation = orientation;
}

// Set position
void Box::setPosition(float x, float y) {
  this -> x = x;
  this -> y = y;
}

// Set size
void Box::setSize(float width, float height) {
  this -> width = width;
  this -> height = height;
}
