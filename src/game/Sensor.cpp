#include "game/Sensor.h"

#include <Box2D/Box2D.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_primitives.h>

#include "util/Globals.h"
#include "util/KeyListener.h"

Sensor::Sensor(float x, float y,float width,float height) :
  Box(x, y, width, height) {

}

void Sensor::init(b2World *world, b2Body *parentBody){
  // Set world
  gameWorld = world;

  // Create body
  createBody(BODY_DYNAMIC, true);

  // Override defaults
  b2Fixture *fixPointer = body -> GetFixtureList();
  fixPointer -> SetDensity(0.0001f);
  fixPointer -> SetFriction(0.0f);
  fixPointer -> SetSensor(true);

  // Feet anchor
  b2Vec2 FeetAnchor(0,0);
	b2WeldJointDef *jointDef = new b2WeldJointDef();
  jointDef -> Initialize(getBody(), parentBody, FeetAnchor);
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

// Get box type
int Sensor::getType(){
  return BOX;
}
