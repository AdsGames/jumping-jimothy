#include "CollisionBox.h"



CollisionBox::CollisionBox()
{
  //ctor
}
void CollisionBox::init(float newX, float newY, float newWidth, float newHeight, b2World *newGameWorld){

  gameWorld = newGameWorld;
  b2BodyDef bodyDef;

  bodyDef.type = b2_kinematicBody;

	bodyDef.position.Set(newX, newY);
	body = gameWorld -> CreateBody(&bodyDef);
	//body ->SetLinearDamping(1);
	//body ->SetAngularDamping(1);

	// Define another box shape for o0ur dynamic body.
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(newWidth/2, newHeight/2);

	// Define the dynamic body fixture.
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;

	// Set the box density to be non-zero, so it will be dynamic.
	fixtureDef.density = 1.0f;

	// Override the default friction.
	fixtureDef.friction = 0.3f;


	// Add the shape to the body.
	body -> CreateFixture(&fixtureDef);

  std::cout<<"!@#$!$!$@%$^&*&^#@$(*(&*&*(&*(^^&*(^&*^&%^&*^&$%^#^%$\n";

}

void CollisionBox::draw(){

}

void CollisionBox::update(){

}

CollisionBox::~CollisionBox()
{
  //dtor
}

