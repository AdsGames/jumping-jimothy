#include "game/Goat.h"

#include "util/Tools.h"
#include "util/Globals.h"

// We'll use this for the goat
Goat::Goat(float x, float y) :
  Box(x, y, 1.6, 3.2) {

  goat_frame = 0;
  goat_tick = 0;

  for (int i = 0; i < 16; i++)
    goat_images[i] =  nullptr;
}

void Goat::init(ALLEGRO_BITMAP *image, b2World *world, Character *character) {

  sprite = image;
  gameCharacter = character;

  // Image
  for (int i = 0; i < 16; i++)
    goat_images[i] = al_create_sub_bitmap( image, i * 32,0, 32, 64);

  // Set world
  gameWorld = world;

  // Create body
  createBody(BODY_DYNAMIC, false);

  // Sensor
  sensor_box = new Sensor(x,y,width,height);
  sensor_box -> init(gameWorld, getBody());
}

// Draw box to screen
void Goat::draw(){

  b2Vec2 position = body -> GetPosition();
  x = position.x;
  y = position.y;
  angle = body -> GetAngle();
  goat_tick++;
  if(goat_tick>10){
    goat_frame++;
    goat_tick=0;
  }
  if(goat_frame>14)
    goat_frame=0;


  ALLEGRO_TRANSFORM trans, prevTrans;

  // back up the current transform
  al_copy_transform(&prevTrans, al_get_current_transform());

  // scale using the new transform
  al_identity_transform(&trans);

  al_rotate_transform(&trans, -angle);
  al_translate_transform(&trans, x * 20, y * -20);

  al_use_transform(&trans);

  al_draw_bitmap(goat_images[goat_frame],-(width/2)*20,-(height/2)*20,0);

  al_use_transform(&prevTrans);
}

bool Goat::getWinCondition(){
  if(gameCharacter && sensor_box -> isCollidingWithBody(gameCharacter -> getBody())){
    return true;
  }
  return false;
}


// Get box type
int Goat::getType(){
  return GOAT;
}
