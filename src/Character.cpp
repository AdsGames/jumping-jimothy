#include <Box2D/Box2D.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "Character.h"
#include "Sensor.h"

Character::Character(){

}

void Character::init( float newX, float newY,ALLEGRO_BITMAP *newSprite, b2World *newGameWorld){

  initial_key_release=false;

  timer_sound_delay=0;
  tick = 0;
  frame = 0;
  landed=true;

  static_box = false;
  static_mode = false;
  orientation = 0;
  static_velocity = b2Vec2( 0, 0);
  static_angular_velocity = 0;
  angle = 0;

  direction = false;
  sprite = newSprite;
  type = CHARACTER;
  width = 0.8;
  height = 2.5;
  color = al_map_rgb(0,0,255);

  gameWorld = newGameWorld;
  b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(newX, newY+0.25f);
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
  sensor_box -> init(newX,newY-0.55,width*0.4,0.6,al_map_rgb(255,255,0),gameWorld,body);

  sprite = tools::load_bitmap_ex("images/anim.png");

  jump = tools::load_sample_ex("jump.wav");
  land = tools::load_sample_ex("land.wav");

  for( int i = 0; i < 15; i++)
    sprites[i] = al_create_sub_bitmap( sprite, i * 32, 0, 32, 64);
}

void Character::update(){

  if(!keyListener::anyKeyPressed && !joystickListener::anyButtonPressed)
    initial_key_release=true;

  if(sensor_box -> isColliding())
    counter_sensor_contact++;
  else
    counter_sensor_contact=0;
 // if(counter_sensor_contact>25)
    //landed=true;

  //std::cout<<body ->GetLinearVelocity().y<<"\n";
  if((body ->GetLinearVelocity().y>=-0.01f && body ->GetLinearVelocity().y<=0.01f) && sensor_box -> isColliding() && velocity_old<-0.01f){
    float land_volume = velocity_old/20;
    if(land_volume>1)
      land_volume=1;
    al_play_sample( land, land_volume, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    landed=true;

  }
  velocity_old = body ->GetLinearVelocity().y;
  if(sensor_box -> isCollidingWithDynamicBody())
    landed=true;

  std::cout<<initial_key_release<<"\n";


  int ticks_per_frame = 5;
  if( !sensor_box -> isColliding()){
    ticks_per_frame = 2;
  }else
    // One liner to fix Andy's exploits
    // This makes the character have no friction when he is airborne.
    // No more wall climbing!
    body ->GetFixtureList() ->SetFriction(0.0f);

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

  float x_velocity_ground = 4;
  float x_velocity_air = 0.8;
  float x_velocity_air_max=4;


  if((keyListener::key[ALLEGRO_KEY_A] || joystickListener::button[JOY_XBOX_PAD_LEFT] ) && initial_key_release){
    direction=false;
    if(sensor_box -> isColliding())
      body -> SetLinearVelocity(b2Vec2(-x_velocity_ground, yVel));
    else
      if(getBody() -> GetLinearVelocity().x > -x_velocity_air_max)
        body -> ApplyLinearImpulse(b2Vec2(-x_velocity_air, 0),position,true);
  }
  else if((keyListener::key[ALLEGRO_KEY_D] || joystickListener::button[JOY_XBOX_PAD_RIGHT] )&& initial_key_release){
    direction=true;
    if(sensor_box -> isColliding())
          body -> SetLinearVelocity(b2Vec2(x_velocity_ground, yVel));
    else
      if(getBody() -> GetLinearVelocity().x < x_velocity_air_max)
        body -> ApplyLinearImpulse(b2Vec2(x_velocity_air, 0),position,true);
  }
  else if(sensor_box -> isColliding()){
     body -> SetLinearVelocity(b2Vec2(0,body ->GetLinearVelocity().y));
  }

  // Jumping Jimothy
  //std::cout<<landed<<"\n";
  timer_jump_delay++;
  if((keyListener::key[ALLEGRO_KEY_W] || joystickListener::button[JOY_XBOX_A]) && sensor_box -> isColliding() && body -> GetLinearVelocity().y<0.1f && landed && initial_key_release){

    if(timer_jump_delay>20){
        timer_jump_delay=0;
      body -> ApplyLinearImpulse(b2Vec2(0,17),position,true);
      landed=false;
      if(timer_sound_delay>10){
        al_play_sample( jump, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        timer_sound_delay=0;
      }
    }
  }
  timer_sound_delay++;

  // Fixed Danny's poop code by getting rid if it =)

  if(sensor_box -> isColliding())
    color = al_map_rgb(50,100,255);
  else
    color = al_map_rgb(0,0,255);
}


void Character::draw(){
  // If the object is a character, the position is updated in the
  // update loop rather than in draw

  ALLEGRO_TRANSFORM trans, prevTrans;

  // back up the current transform
  al_copy_transform(&prevTrans, al_get_current_transform());

  // scale using the new transform
  al_identity_transform(&trans);

  al_rotate_transform(&trans, -angle);
  al_translate_transform(&trans, (x)*20, -(y)*20);

  al_use_transform(&trans);

    // Nice debug draw for player hitbox
 // al_draw_filled_rectangle( -(width/2) * 20 + 1, -(height/2)*20 + 1, (width/2) * 20 - 1, (height/2) * 20 - 1,al_map_rgb(255,25,64));

  int x_offset=-8;
  int y_offset=-13;

  if(direction){
    if(body -> GetLinearVelocity().Length()>0.1f)
      al_draw_bitmap(sprites[frame],-(width/2)*20+x_offset,(-(height/2)*20)+y_offset,0);
    else
      al_draw_bitmap(sprites[14],-(width/2)*20+x_offset,(-(height/2)*20)+y_offset,0);
  }
  else{
    if(body -> GetLinearVelocity().Length()>0.1f)
      al_draw_bitmap(sprites[frame],-(width/2)*20+x_offset,(-(height/2)*20)+y_offset,ALLEGRO_FLIP_HORIZONTAL);
    else
      al_draw_bitmap(sprites[14],-(width/2)*20+x_offset,(-(height/2)*20)+y_offset,ALLEGRO_FLIP_HORIZONTAL);

  }



  // restore the old transform
  al_use_transform(&prevTrans);



  // Nice debug draw for sensor box
  //sensor_box -> draw();
}

Character::~Character(){

}
