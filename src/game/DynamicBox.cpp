#include "game/DynamicBox.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "util/Tools.h"
#include "util/Globals.h"

DynamicBox::DynamicBox(float x, float y) :
  Box(x, y, 1.55f, 1.55f) {

  color = al_map_rgb(255, 0, 0);
}

void DynamicBox::init(float vel_x, float vel_y, ALLEGRO_BITMAP *image, b2World *world){

  sprite = image;

  // Set world
  gameWorld = world;

  // Create body
  createBody(BODY_KINEMATIC, false);
}


// Set state
void DynamicBox::setStatic(bool stat) {
  Box::setStatic(stat);

  if (static_mode) {
    static_velocity = body -> GetLinearVelocity();
    static_angular_velocity = body -> GetAngularVelocity();
    body -> SetType( b2_staticBody);
  }
  else {
    static_mode = false;
    body -> SetType( b2_dynamicBody);
    if(isPausable() && (static_velocity.y<=0.01f && static_velocity.y>=-0.01f && static_velocity.x<=0.1f && static_velocity.x>=-0.1f && static_angular_velocity<=0.1f && static_angular_velocity>=-0.1f )){
      body -> SetAwake(false);
      body -> SetLinearVelocity(b2Vec2(0,0));
    }
    else{
      body -> SetLinearVelocity( static_velocity);
      body -> SetAngularVelocity( static_angular_velocity);
    }
  }
}

// Draw box to screen
void DynamicBox::draw() {
  // Dynamic boxes have no update loop, so we must get the location
  // from the Box2D world in the update loop
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

  b2Vec2 draw_velocity = b2Vec2(0,0);

  // Danny... why was this removed?
  // Lolwutnow
  if( static_mode)
    draw_velocity = b2Vec2( static_velocity.x, static_velocity.y);
  else
    draw_velocity = b2Vec2( body -> GetLinearVelocity().x, body -> GetLinearVelocity().y);



  al_draw_filled_rectangle( -(width/2) * 20 + 1, -(height/2)*20 + 1, (width/2) * 20 - 1, (height/2) * 20 - 1,
  al_map_rgb( tools::clamp( 0, 255, int(draw_velocity.y * -10)),
                  tools::clamp( 0, 255, 255 - int(draw_velocity.y * -10)),
                  0));
  al_draw_bitmap(sprite,-(width/2)*20,-(height/2)*20,0);

  // restore the old transform

  al_use_transform(&prevTrans);
}

// Get box type
int DynamicBox::getType(){
  return BOX;
}

// Is pausable
bool DynamicBox::isPausable() {
  return true;
}
