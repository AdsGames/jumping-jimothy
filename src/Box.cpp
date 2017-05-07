#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <Box2D/Box2D.h>
#include <iostream>

#include "Box.h"

Box::Box(){

}

Box::~Box(){

}

void Box::init(float newX, float newY, float newWidth, float newHeight,float newVelX, float newVelY, bool newBodyType,ALLEGRO_BITMAP *newSprite, b2World *newGameWorld){

  //std::cout << "Created Box\n";

  sprite = newSprite;

  type = BOX;
  width = newWidth;
  height = newHeight;
  color = al_map_rgb(255,0,0);
  static_mode = false;
  static_box = false;
  angle = 0;
  x = 0;
  y = 0;

  static_velocity = b2Vec2( 0, 0);
  static_angular_velocity = 0;

  gameWorld = newGameWorld;
  b2BodyDef bodyDef;

  static_box = !newBodyType;

  if(newBodyType)
    bodyDef.type = b2_dynamicBody;
	else
	 bodyDef.type = b2_kinematicBody;

	bodyDef.position.Set(newX, newY);
	body = gameWorld -> CreateBody(&bodyDef);
	body -> SetLinearVelocity(b2Vec2(newVelX,newVelY));
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
}

// Return type
int Box::getType(){
  return type;
}

// Return X
float Box::getX(){
  return x;
}

// Return Y
float Box::getY(){
  return y;
}

// Return body
b2Body* Box::getBody(){
  b2Body *newBody = body;
  return newBody;
}

// Set state
void Box::setStatic(){
  if(!static_box){
    static_mode = true;
    static_velocity = body -> GetLinearVelocity();
    static_angular_velocity = body -> GetAngularVelocity();
    body -> SetType( b2_staticBody);
  }
}

// Set whether dynamic
void Box::setDynamic(){
  if(!static_box){
    static_mode = false;
    body -> SetType( b2_dynamicBody);

    if(static_velocity.y<=0.01f && static_velocity.y>=-0.01f && static_velocity.x<=0.1f && static_velocity.x>=-0.1f && static_angular_velocity<=0.1f && static_angular_velocity>=-0.1f ){
      body -> SetAwake(false);
      body -> SetLinearVelocity(b2Vec2(0,0));
    }
    else{
      body -> SetLinearVelocity( static_velocity);
      body -> SetAngularVelocity( static_angular_velocity);
    }
  }
}

// Update box
void Box::update(){

}

// Draw box to screen
void Box::draw(){
  // If the object is a character, the position is updated in the
  // update loop rather than in draw
  if(type == BOX){
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
  al_translate_transform(&trans, x * 20, y * -20);

  al_use_transform(&trans);

  b2Vec2 draw_velocity = b2Vec2(0,0);

  if(!static_box){

    if( static_mode)
      draw_velocity = b2Vec2( static_velocity.x, static_velocity.y);
    else
      draw_velocity = b2Vec2( body -> GetLinearVelocity().x, body -> GetLinearVelocity().y);

    al_draw_filled_rectangle(-(width/2)*20 + 1, -(height/2)*20  + 1, (width/2)*20 - 1, (height/2)*20 - 1,
                al_map_rgb( tools::clamp( 0, 255, int(draw_velocity.y * -10)), tools::clamp( 0, 255, 255 - int(draw_velocity.y * -10)), 0));

   // al_draw_line( 0, 0, draw_velocity.x * 10, draw_velocity.y * 10,
        //       al_map_rgb( tools::clamp( 0, 255, int(draw_velocity.y * -10)), tools::clamp( 0, 255, 255 - int(draw_velocity.y * -10)), 0), 3);

  }else{
      al_draw_filled_rectangle(-(width/2)*20 + 1, -(height/2)*20  + 1, (width/2)*20 - 1, (height/2)*20 - 1,
                al_map_rgb(255,255,0));
  }

  al_draw_bitmap(sprite,-(width/2)*20,-(height/2)*20,0);

  // restore the old transform
  al_use_transform(&prevTrans);

}
