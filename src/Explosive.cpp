#include "Explosive.h"
#include <allegro5/allegro_primitives.h>

Explosive::Explosive()
{
  //ctor
}

Explosive::~Explosive()
{
  //dtor
}

//subclass b2QueryCallback for proximity query callback
class MyQueryCallback : public b2QueryCallback {
public:
    std::vector<b2Body*> foundBodies;

    bool ReportFixture(b2Fixture* fixture) {
        foundBodies.push_back( fixture->GetBody() );
        return true;//keep going to find all fixtures in the query area
    }
};

void Explosive::init(float newX, float newY, int newOrientation,BITMAP *newSprite,bool newAffectCharacter, b2World *newGameWorld, Character *newGameCharacter){

  sprite = newSprite;
  gameCharacter = newGameCharacter;
  orientation = newOrientation;

  affect_character=newAffectCharacter;
  numRays=32;
  blastRadius = 10;
  blastPower = 1000;

  type = 5;
  width = 1.55f;
  height = 1.55f;
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

  bodyDef.type = b2_kinematicBody;

	bodyDef.position.Set(newX, newY);
	body = gameWorld -> CreateBody(&bodyDef);
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
}



void Explosive::update(){

  b2Vec2 center = body->GetPosition();



  // Totally not copypasta'd code
  // Should do something about it
  // Ehhhhhhh

  //find all fixtures within blast radius AABB
  MyQueryCallback queryCallback;
  b2AABB aabb;
  aabb.lowerBound = center - b2Vec2( blastRadius, blastRadius );
  aabb.upperBound = center + b2Vec2( blastRadius, blastRadius );
  gameWorld->QueryAABB( &queryCallback, aabb );

  //check which of these have their center of mass within the blast radius
  for (int i = 0; i < queryCallback.foundBodies.size(); i++) {
    b2Body* newBody = queryCallback.foundBodies[i];
    b2Vec2 bodyCom = newBody->GetWorldCenter();
    //ignore bodies outside the blast range
    if ( (bodyCom - center).Length() >= blastRadius )
      continue;
    applyBlastImpulse(newBody, center, bodyCom, blastPower * 0.05f );//scale blast power to roughly match results of other methods at 32 rays
  }
}

 //this is the same for proximity and raycast methods so we can put it in a common function
void Explosive::applyBlastImpulse(b2Body* newBody, b2Vec2 blastCenter, b2Vec2 applyPoint, float blastPower){

  //ignore the grenade itself, and any non-dynamic bodies

  // Fricking feet, how do they work?
  if ( newBody == body || newBody->GetType() != b2_dynamicBody || ((newBody == gameCharacter->getBody()
    || newBody == gameCharacter -> getSensorBody() ) && !affect_character))
    return;
  b2Vec2 blastDir = applyPoint - blastCenter;
  float distance = blastDir.Normalize();
        //ignore bodies exactly at the blast point - blast direction is undefined
  if ( distance == 0 )
    return;
  float invDistance = 1 / distance;
  float impulseMag = blastPower * invDistance * invDistance;
  impulseMag = b2Min( impulseMag, 500.0f );



  if(orientation == 0)
    newBody->ApplyLinearImpulse( impulseMag * blastDir, applyPoint,true );
  else{
    float magnitude=0.2;
    b2Vec2 new_direction;
    if(orientation==1)
      new_direction=b2Vec2(0,magnitude);
    if(orientation==3)
      new_direction=b2Vec2(0,-magnitude);

    if(orientation==2)
      new_direction=b2Vec2(magnitude,0);

    if(orientation==4)
      new_direction=b2Vec2(-magnitude,0);

    newBody->ApplyLinearImpulse( impulseMag*new_direction, applyPoint,true );

  }
}

void Explosive::draw(){

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

    draw_velocity = b2Vec2( 100,100);



  if(affect_character)
    al_draw_filled_rectangle( -(width/2) * 20 + 1, -(height/2)*20 + 1, (width/2) * 20 - 1, (height/2) * 20 - 1,al_map_rgb(255,0,0));
  else
    al_draw_filled_rectangle( -(width/2) * 20 + 1, -(height/2)*20 + 1, (width/2) * 20 - 1, (height/2) * 20 - 1,al_map_rgb(0,255,0));


  // PI/2 is a quarter turn. Editor boxes orientation is a range from 1-4.
  // So we have a quarter turn * 1-4, creating a quarter turn, half turn, ect.
  // - PI/2 is because we start rotated right a quarter turn.
  float new_angle=(PI/2)*orientation - PI/2;

  al_draw_rotated_bitmap(sprite,16,16,0,0,new_angle,0);

 // al_draw_bitmap(sprite,-(width/2)*20,-(height/2)*20,0);

  // restore the old transform

  al_use_transform(&prevTrans);
}
