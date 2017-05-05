#include <Box2D/Box2D.h>
#include "Sensor.h"

Sensor::Sensor(){


}
void Sensor::init(float newX, float newY, float newWidth, float newHeight, b2World *newGameWorld, b2Body *parentBody){

  std::cout<<"Created Box\n";

  type = BOX;
  width = newWidth;
  height = newHeight;
  color = al_map_rgb(255,0,0);

  gameWorld = newGameWorld;
  b2BodyDef bodyDef;

  bodyDef.type = b2_dynamicBody;


	bodyDef.position.Set(newX, newY);
	body = gameWorld -> CreateBody(&bodyDef);
	body ->SetLinearDamping(1);
	body ->SetAngularDamping(1);

	// Define another box shape for our dynamic body.
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(width/2, height/2);

	// Define the dynamic body fixture.
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;


	// Set the box density to be non-zero, so it will be dynamic.
	fixtureDef.density = 0.001f;

	// Override the default friction.
	fixtureDef.friction = 0.0f;

  fixtureDef.isSensor = true;
	// Add the shape to the body.
	body->CreateFixture(&fixtureDef);

  b2Vec2 FeetAnchor(0,0);


	b2WeldJointDef *jointDef = new b2WeldJointDef();
  jointDef -> Initialize(body, parentBody, FeetAnchor);
  jointDef -> collideConnected = false;
  jointDef  -> referenceAngle = 0;
  gameWorld -> CreateJoint(jointDef);






}
bool Sensor::isColliding(){
  for(b2ContactEdge *contact = body -> GetContactList(); contact; contact = contact ->next)
    return true;
  return false;


}

Sensor::~Sensor(){


}
