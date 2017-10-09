#include <Box2D/Box2D.h>
#include "Sensor.h"
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_primitives.h>

Sensor::Sensor(){


}
void Sensor::init(float newX, float newY, float newWidth, float newHeight, ALLEGRO_COLOR newColor, b2World *newGameWorld, b2Body *parentBody){


  type = BOX;
  width = newWidth;
  height = newHeight;
  color = newColor;

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
	fixtureDef.density = 0.0001f;

	// Override the default friction.
	fixtureDef.friction = 0.0f;

  fixtureDef.isSensor = true;
	// Add the shape to the body.
	body -> CreateFixture(&fixtureDef);
	body -> SetFixedRotation(true);

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
bool Sensor::isCollidingWithDynamicBody(){
  for(b2ContactEdge *contact = body -> GetContactList(); contact; contact = contact ->next){
    if(contact ->other ->GetType() == b2_dynamicBody)
      return true;
  }
  return false;


}
bool Sensor::isCollidingWithBody(b2Body *newBody){
  for(b2ContactEdge *contact = body -> GetContactList(); contact; contact = contact ->next){
      if(contact -> other==newBody)
      return true;
  }
  return false;


}

// Draw box to screen
void Sensor::draw(){
  // If the object is a character, the position is updated in the
  // update loop rather than in draw

    b2Vec2 position = body -> GetPosition();
    x = position.x;
    y = position.y;
    angle = body -> GetAngle();


  ALLEGRO_TRANSFORM trans, prevTrans;

  // back up the current transform
  al_copy_transform(&prevTrans, al_get_current_transform());

  // scale using the new transform
  al_identity_transform(&trans);

  al_rotate_transform(&trans, -angle);
  al_translate_transform(&trans, x * 20, y * -20);

  al_use_transform(&trans);


    al_draw_filled_rectangle(-(width/2)*20 , -(height/2)*20 , (width/2)*20 , (height/2)*20 ,
                al_map_rgb(255,255,0));

 // al_draw_bitmap(sprite,-(width/2)*20,-(height/2)*20,0);

  // restore the old transform
  al_use_transform(&prevTrans);

}


Sensor::~Sensor(){


}
