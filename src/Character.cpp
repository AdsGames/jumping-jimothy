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

  if(gameKeyListener -> key[ALLEGRO_KEY_W] && sensor_box -> isColliding() && body -> GetLinearVelocity().y<0.1f){
    body -> SetLinearVelocity(b2Vec2(body -> GetLinearVelocity().x,0));
    body -> ApplyForce(b2Vec2(0,2000),position);
  }


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
  //ALLEGRO_COLOR newColour =
  sensor_box -> init(newX,newY-0.5,width*0.6,1,al_map_rgb(255,255,0),gameWorld,body);



}

void Character::draw(){


  // If the object is a character, the position is updated in the
  // update loop rather than in draw
  if(type==BOX){
    b2Vec2 position = body -> GetPosition();
    x = position.x;
    y = position.y;
    angle = body -> GetAngle();
 }
  ALLEGRO_TRANSFORM trans, prevTrans;

  // back up the current transform
  al_copy_transform(&prevTrans, al_get_current_transform());

  // scale using the new transform
  al_identity_transform(&trans);

  al_rotate_transform(&trans, -angle);
  al_translate_transform(&trans, (x)*20, -(y)*20);

  al_use_transform(&trans);

  al_draw_rectangle(-(width/2)*20, -(height/2)*20, (width/2)*20 , (height/2)*20,al_map_rgb(0,0,0),3);

  al_draw_filled_rectangle(-(width/2)*20, -(height/2)*20, (width/2)*20 , (height/2)*20,color);

  // restore the old transform
  al_use_transform(&prevTrans);

  //sensor_box -> draw();

}

Character::~Character(){


}
