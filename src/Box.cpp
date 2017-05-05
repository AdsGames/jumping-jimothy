#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <Box2D/Box2D.h>
#include <iostream>

#include "Box.h"

Box::Box(){


}

void Box::init(float newX, float newY, float newWidth, float newHeight, bool newBodyType, b2World *newGameWorld){

  std::cout<<"Created Box\n";

  type = BOX;
  width = newWidth;
  height = newHeight;
  color = al_map_rgb(255,0,0);

  gameWorld = newGameWorld;
  b2BodyDef bodyDef;
  if(newBodyType)
    bodyDef.type = b2_dynamicBody;
	else
	 bodyDef.type = b2_kinematicBody;

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
	fixtureDef.density = 1.0f;

	// Override the default friction.
	fixtureDef.friction = 0.3f;


	// Add the shape to the body.
	body->CreateFixture(&fixtureDef);





}
int Box::getType(){
  return type;
}
float Box::getX(){
  return x;
}

float Box::getY(){
  return y;
}
b2Body* Box::getBody(){
  b2Body *newBody = body;
  return newBody;
}


// polymorpism haxx until I figure out nice casting and identification
void Box::update(){

  // If you see this, you have a problem
  std::cout<<"Houston, we have a problem\n";


}

void Box::draw(){


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

}

Box::~Box(){


}
