#include "CollisionBox.h"



CollisionBox::CollisionBox()
{
  //ctor
}
void CollisionBox::init(float newX, float newY, float newWidth, float newHeight, b2World *newGameWorld){

  width = newWidth;
  height = newHeight;
  gameWorld = newGameWorld;
  b2BodyDef bodyDef;
  type = 4;

  bodyDef.type = b2_kinematicBody;

	bodyDef.position.Set(newX, newY);
	body = gameWorld -> CreateBody(&bodyDef);
	//body ->SetLinearDamping(1);
	//body ->SetAngularDamping(1);

	// Define another box shape for our dynamic body.
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(newWidth/2, newHeight/2);

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

void CollisionBox::draw(){

  if( keyListener::key[ALLEGRO_KEY_G]){

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

void CollisionBox::update(){

}

CollisionBox::~CollisionBox()
{
  //dtor
}

