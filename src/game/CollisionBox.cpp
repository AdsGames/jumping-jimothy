#include "game/CollisionBox.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "util/KeyListener.h"
#include "util/Globals.h"

CollisionBox::CollisionBox(float x, float y, float width, float height) :
  Box(x, y, width, height) {
}

void CollisionBox::init(b2World *world){
  // Set world
  gameWorld = world;

  // Create body
  createBody(BODY_KINEMATIC, false);
}

void CollisionBox::draw(){

  if( KeyListener::key[ALLEGRO_KEY_G]){

    // Collision boxes don't have an update loop, we have to get
    // the location from the Box2D world ourselves in the draw loop
    b2Vec2 position = body -> GetPosition();
    x = position.x;
    y = position.y;

    ALLEGRO_TRANSFORM trans, prevTrans;

    // back up the current transform
    al_copy_transform(&prevTrans, al_get_current_transform());

    // scale using the new transform
    al_identity_transform(&trans);

    al_translate_transform(&trans, x * 20, y * -20);

    al_use_transform(&trans);

    al_draw_filled_rectangle( -(width/2) * 20 + 1, -(height/2)*20 + 1, (width/2) * 20 - 1, (height/2) * 20 - 1,al_map_rgb(255,0,0));

    al_use_transform(&prevTrans);
  }
}

// Get box type
int CollisionBox::getType(){
  return COLLISION;
}
