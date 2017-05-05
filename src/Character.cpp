#include <Box2D/Box2D.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "Character.h"
#include "Sensor.h"

Character::Character(){


}

void Character::update(){

//std::cout<<"Character updated\n";


  b2Vec2 position = body -> GetPosition();
  x = position.x;
  y = position.y;
  angle = body -> GetAngle();

  if(gameKeyListener -> key[ALLEGRO_KEY_A])
    body -> ApplyForce(b2Vec2(-100,0),position);

  if(gameKeyListener -> key[ALLEGRO_KEY_D])
    body -> ApplyForce(b2Vec2(100,0),position);

  if(gameKeyListener -> key[ALLEGRO_KEY_W])
    body -> ApplyForce(b2Vec2(0,100),position);

  if(gameKeyListener -> key[ALLEGRO_KEY_S])
    body -> ApplyForce(b2Vec2(0,-100),position);

  if(sensor_box -> isColliding()){
    color = al_map_rgb(50,100,255);

  }else
        color = al_map_rgb(0,0,255);




}

void Character::init(float newX, float newY, b2World *newGameWorld, keyListener *newKeyListener){

  type = CHARACTER;
  width = 2;
  height = 2;
  color = al_map_rgb(0,0,255);

  gameWorld = newGameWorld;
  b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(newX, newY);
	body = gameWorld -> CreateBody(&bodyDef);
	body ->SetLinearDamping(1);

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
	body->CreateFixture(&fixtureDef);

  body ->SetFixedRotation(true);

  sensor_box = new Sensor();
  sensor_box -> init(newX,newY,5,5,gameWorld,body);



}


Character::~Character(){


}
