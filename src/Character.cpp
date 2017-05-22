#include <Box2D/Box2D.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "Character.h"
#include "Sensor.h"

Character::Character(){

}

void Character::init( float newX, float newY,ALLEGRO_BITMAP *newSprite, b2World *newGameWorld){
  tick = 0;
  frame = 0;

  static_box = false;
  static_mode = false;
  orientation = 0;
  static_velocity = b2Vec2( 0, 0);
  static_angular_velocity = 0;
  angle = 0;

  direction = false;
  sprite = newSprite;
  type = CHARACTER;
  width = 1;
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
	//body -> SetLinearDamping(1);

	// Set the box density to be non-zero, so it will be dynamic.
	fixtureDef.density = 1.0f;

	// Override the default friction.
	fixtureDef.friction = 0.3f;

	// Add the shape to the body.
	body -> CreateFixture(&fixtureDef);

  body ->SetFixedRotation(true);

  sensor_box = new Sensor();
  sensor_box -> init(newX,newY-0.55,width*0.5,0.6,al_map_rgb(255,255,0),gameWorld,body);

  sprite = tools::load_bitmap_ex("images/anim.png");

  for( int i = 0; i < 15; i++)
    sprites[i] = al_create_sub_bitmap( sprite, i * 32, 0, 32, 64);
}

void Character::update(){
  int ticks_per_frame = 5;
  if( !sensor_box -> isColliding())
    ticks_per_frame = 2;

  tick++;
  if( tick >= ticks_per_frame){
    frame++;
    tick = 0;
  }
  if(frame==14)
    frame = 0;

  b2Vec2 position = body -> GetPosition();
  x = position.x;
  y = position.y;
  angle = body -> GetAngle();
  double yVel = getBody() -> GetLinearVelocity().y;

  if(keyListener::key[ALLEGRO_KEY_A] || joystickListener::button[JOY_XBOX_PAD_LEFT]){
    direction=false;
    if(sensor_box -> isColliding())
      body -> SetLinearVelocity(b2Vec2(-5, yVel));
    else
      if(getBody() -> GetLinearVelocity().x > -5)
        body -> ApplyLinearImpulse(b2Vec2(-2, 0),position,true);
  }
  else if(keyListener::key[ALLEGRO_KEY_D] || joystickListener::button[JOY_XBOX_PAD_RIGHT]){
    direction=true;
    if(sensor_box -> isColliding())
          body -> SetLinearVelocity(b2Vec2(5, yVel));
    else
      if(getBody() -> GetLinearVelocity().x < 5)
        body -> ApplyLinearImpulse(b2Vec2(2, 0),position,true);
  }
  else if(sensor_box -> isColliding()){
     body -> SetLinearVelocity(b2Vec2(0,body ->GetLinearVelocity().y));
  }

  // Homemade friction
  if((keyListener::key[ALLEGRO_KEY_W] || joystickListener::button[JOY_XBOX_A]) && sensor_box -> isColliding() && body -> GetLinearVelocity().y<0.1f)
    body -> ApplyLinearImpulse(b2Vec2(0, 15),position,true);

  // Fixed Danny's poop code by getting rid if it =)
  if(sensor_box -> isColliding())
    color = al_map_rgb(50,100,255);
  else
    color = al_map_rgb(0,0,255);
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

  if(direction){
    if(body -> GetLinearVelocity().Length()>0.1f)
      al_draw_bitmap(sprites[frame],-(width/2)*20,(-(height/2)*20)-24,0);
    else
      al_draw_bitmap(sprites[14],-(width/2)*20,(-(height/2)*20)-24,0);
  }
  else{
    if(body -> GetLinearVelocity().Length()>0.1f)
      al_draw_bitmap(sprites[frame],-(width/2)*20,(-(height/2)*20)-24,ALLEGRO_FLIP_HORIZONTAL);
    else
      al_draw_bitmap(sprites[14],-(width/2)*20,(-(height/2)*20)-24,ALLEGRO_FLIP_HORIZONTAL);

  }
  // restore the old transform
  al_use_transform(&prevTrans);

  /*if(sensor_box -> isColliding())
    sensor_box -> draw();*/
}

Character::~Character(){

}
