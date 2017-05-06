#include <Box2D/Box2D.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "Character.h"
#include "Sensor.h"

Character::Character(){


}

void Character::update(){

//std::cout<<"Character updated\n";

  b2Vec2 position = body -> GetPosition();
  x = position.x;
  y = position.y;
  angle = body -> GetAngle();

  int x_movement=6;
  float x_air_movement=0.4;
  time_move_jump_timer_thingy++;


    if(keyListener::key[ALLEGRO_KEY_A]){
        if(sensor_box -> isColliding() && time_move_jump_timer_thingy>30){
            body -> SetLinearVelocity(b2Vec2(-x_movement,body ->GetLinearVelocity().y));


         }else{
          body -> ApplyLinearImpulse(b2Vec2(-x_air_movement,0),position);
        }
    }

    else if(keyListener::key[ALLEGRO_KEY_D]){
      if(sensor_box -> isColliding() && time_move_jump_timer_thingy>30){
            body -> SetLinearVelocity(b2Vec2(x_movement,body ->GetLinearVelocity().y));

        }else{
          body -> ApplyLinearImpulse(b2Vec2(x_air_movement,0),position);

        }
    }else if(sensor_box -> isColliding())
       body -> SetLinearVelocity(b2Vec2(0,body ->GetLinearVelocity().y));
    // Homemade friction




  if(keyListener::key[ALLEGRO_KEY_W] && sensor_box -> isColliding() && body -> GetLinearVelocity().y<0.1f){
    body -> SetLinearVelocity(b2Vec2(0,6.5));
    time_move_jump_timer_thingy=0;

  }


  if(sensor_box -> isColliding()){
    color = al_map_rgb(50,100,255);

  }else
        color = al_map_rgb(0,0,255);






}

void Character::init(float newX, float newY,ALLEGRO_BITMAP *newSprite, b2World *newGameWorld){

  direction = false;
  sprite = newSprite;
  type = CHARACTER;
  width = 2;
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
	body->CreateFixture(&fixtureDef);

  body ->SetFixedRotation(true);

  sensor_box = new Sensor();
  //ALLEGRO_COLOR newColour =
  sensor_box -> init(newX,newY-0.55,width*0.7,0.6,al_map_rgb(255,255,0),gameWorld,body);



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

  al_draw_bitmap(sprite,-(width/2)*20,(-(height/2)*20)-24,0);

 // al_draw_rectangle(-(width/2)*20, -(height/2)*20, (width/2)*20 , (height/2)*20,al_map_rgb(0,0,0),3);

  //al_draw_filled_rectangle(-(width/2)*20, -(height/2)*20, (width/2)*20 , (height/2)*20,color);

  // restore the old transform
  al_use_transform(&prevTrans);

  //if(sensor_box -> isColliding())
 // sensor_box -> draw();

}

Character::~Character(){


}
