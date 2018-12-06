#include "game/Sensor.h"

#include <Box2D/Box2D.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_primitives.h>

#include "util/Globals.h"
#include "util/KeyListener.h"

Sensor::Sensor(float x, float y, float width, float height, b2Body *parentBody, b2World *world) :
  Box(x, y, width, height, world) {

  // Modify body
  body -> SetType(b2_dynamicBody);

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
  ALLEGRO_TRANSFORM trans, prevTrans;

  // back up the current transform
  al_copy_transform(&prevTrans, al_get_current_transform());

  // scale using the new transform
  al_identity_transform(&trans);

  al_rotate_transform(&trans, -getAngle());
  al_translate_transform(&trans, getX() * 20, getY() * -20);

  al_use_transform(&trans);


    al_draw_filled_rectangle(-(getWidth()/2)*20 , -(getHeight()/2)*20 , (getWidth()/2)*20 , (getHeight()/2)*20 ,
                al_map_rgb(255,255,0));

 // al_draw_bitmap(sprite,-(width/2)*20,-(height/2)*20,0);

  // restore the old transform
  al_use_transform(&prevTrans);

}

// Get box type
int Sensor::getType(){
  return BOX;
}
