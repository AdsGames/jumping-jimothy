#include "Sensor.h"

#include <Box2D/Box2D.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_primitives.h>

#include "../util/Globals.h"
#include "../util/KeyListener.h"

Sensor::Sensor(const float x,
               const float y,
               const float width,
               const float height,
               b2Body* parentBody,
               b2World* world)
    : Box(x, y, width, height, world) {
  // Modify body
  body->SetType(b2_dynamicBody);

  // Override defaults
  b2Fixture* fixPointer = body->GetFixtureList();
  fixPointer->SetDensity(0.0001f);
  fixPointer->SetFriction(0.0f);
  fixPointer->SetSensor(true);

  // Feet anchor
  b2Vec2 FeetAnchor(0, 0);

  // Create joint
  b2WeldJointDef* jointDef = new b2WeldJointDef();
  jointDef->Initialize(getBody(), parentBody, FeetAnchor);
  jointDef->collideConnected = false;
  jointDef->referenceAngle = 0;
  world->CreateJoint(jointDef);
}

bool Sensor::isColliding() {
  // Parse contacts
  for (b2ContactEdge* contact = body->GetContactList(); contact;
       contact = contact->next)
    return true;
  return false;
}

bool Sensor::isCollidingWithDynamicBody() {
  // Parse contacts
  for (b2ContactEdge* contact = body->GetContactList(); contact;
       contact = contact->next) {
    if (contact->other->GetType() == b2_dynamicBody)
      return true;
  }
  return false;
}

bool Sensor::isCollidingWithBody(b2Body* newBody) {
  // Parse contacts
  for (b2ContactEdge* contact = body->GetContactList(); contact;
       contact = contact->next) {
    if (contact->other == newBody)
      return true;
  }
  return false;
}

// Draw box to screen
void Sensor::draw() {
  // Draw transform
  ALLEGRO_TRANSFORM trans, prevTrans;

  // back up the current transform
  al_copy_transform(&prevTrans, al_get_current_transform());

  // scale using the new transform
  al_identity_transform(&trans);

  al_rotate_transform(&trans, -getAngle());
  al_translate_transform(&trans, getX() * 20, getY() * -20);

  al_use_transform(&trans);

  al_draw_filled_rectangle(-(getWidth() / 2) * 20, -(getHeight() / 2) * 20,
                           (getWidth() / 2) * 20, (getHeight() / 2) * 20,
                           al_map_rgb(255, 255, 0));

  // restore the old transform
  al_use_transform(&prevTrans);
}

// Get box type
int Sensor::getType() {
  return BOX;
}
