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

  static_mode = true;

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
void Box::setImages(ALLEGRO_BITMAP* img_1, ALLEGRO_BITMAP* img_2, ALLEGRO_BITMAP* img_3, ALLEGRO_BITMAP* img_4) {
  new_tiles[0] = img_1;
  new_tiles[1] = img_2;
  new_tiles[2] = img_3;
  new_tiles[3] = img_4;
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
void Box::setStatic(bool stat) {
  static_mode = stat;
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
