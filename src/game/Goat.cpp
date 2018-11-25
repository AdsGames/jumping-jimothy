#include "game/Goat.h"

#include "util/Tools.h"
#include "util/Globals.h"

// We'll use this for the goat
void Goat::init(float newX, float newY, ALLEGRO_BITMAP *newSprite, b2World *newGameWorld, Character *newCharacter){

  goat_frame = 0;
  goat_tick = 0;
  sprite = newSprite;
  gameCharacter = newCharacter;

  if( gameCharacter == nullptr)
    tools::log_message("WARNING: Box: gameCharacter is undeclared");

  // Image
  for( int i = 0; i < 16; i++)
    goat_images[i] = al_create_sub_bitmap( newSprite, i * 32,0, 32, 64);

  type = GOAT;
  width = 1.6;
  height = 3.2;
  orientation = 0;
  static_mode = false;
  static_box = false;
  angle = 0;
  x = 0;
  y = 0;

  gameWorld = newGameWorld;
  static_box = false;

  // Body
  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;

  // Position
  bodyDef.position.Set(newX, newY);
  body = gameWorld -> CreateBody(&bodyDef);

  // Velocity
  static_velocity = b2Vec2( 0, 0);
  static_angular_velocity = 0;

  //body -> SetLinearVelocity(b2Vec2(newVelX,newVelY));
  //body ->SetLinearDamping(1);
  //body ->SetAngularDamping(1);

  // Define another box shape for o0ur dynamic body.
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
  body -> CreateFixture(&fixtureDef);

  // Sensor
  sensor_box = new Sensor();
  sensor_box -> init(newX,newY,width,height,al_map_rgb(255,255,0),gameWorld,body);
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

  if(gameCharacter==nullptr)
    tools::log_message("WARNING: Box: gameCharacter is undeclared");


  if(sensor_box -> isCollidingWithBody(gameCharacter -> getBody())){
    return true;
  }
  return false;
}

Goat::~Goat()
{
  //dtor
}
