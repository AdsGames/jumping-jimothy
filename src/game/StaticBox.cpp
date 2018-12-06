#include "game/StaticBox.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "util/Globals.h"

// I'm trying to make static boxes really just for show,
// and have the collision handled elsewhere. So, this class
// will inherit for Box.cpp but will (should) only hold
// the location and the image

// Static Box Constructors
StaticBox::StaticBox(float x, float y) :
  Box(x, y, 1.5f, 1.5f) {

}

// Draw box to screen
void StaticBox::draw(){
  // Transform
  ALLEGRO_TRANSFORM trans, prevTrans;

  // back up the current transform
  al_copy_transform(&prevTrans, al_get_current_transform());

  // scale using the new transform
  al_identity_transform(&trans);

  //al_rotate_transform(&trans, -angle);
  al_translate_transform(&trans, x * 20, y * -20);

  al_use_transform(&trans);

  for(int t = 0; t < 4; t++){
    // Offsets from subtile
    int off_x = (t == 1 || t == 3) ? 16: 0;
    int off_y = (t >= 2) ? 16: 0;

    al_draw_bitmap( new_tiles[t],-(width/2)*20+off_x, -(height/2)*20+off_y, 0);
  }

  al_use_transform(&prevTrans);

  // I suck at programming
  // - Danny Van Stemp, July 30, 2017
}

// Get box type
int StaticBox::getType(){
  return STATIC;
}
