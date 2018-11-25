#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <Box2D/Box2D.h>

#include "game/Box.h"
#include "game/Sensor.h"
#include "game/Character.h"

#include "util/Tools.h"
#include "util/Globals.h"

Box::Box(){

}

Box::~Box(){

}


void Box::init(){}


// Halp how do I polymorph?
void Box::update(){}
void Box::setStatic(){}
void Box::setDynamic(bool newBool){
  static_mode = newBool;
}
void Box::draw(){}
