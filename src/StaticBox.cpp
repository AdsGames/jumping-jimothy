#include "StaticBox.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "Globals.h"

// I'm trying to make static boxes really just for show,
// and have the collision handled elsewhere. So, this class
// will inherit for Box.cpp but will (should) only hold
// the location and the image

// Update box
void StaticBox::update(){

}

void StaticBox::init(float newX, float newY,  ALLEGRO_BITMAP *sp_1,ALLEGRO_BITMAP *sp_2,ALLEGRO_BITMAP *sp_3,ALLEGRO_BITMAP *sp_4) {

  x=newX;
  y=newY;
  type = STATIC;
  angle=0;
  width=1.5f;
  height=1.5f;

  new_tiles[0] = sp_1;
  new_tiles[1] = sp_2;
  new_tiles[2] = sp_3;
  new_tiles[3] = sp_4;
}

// Draw box to screen
void StaticBox::draw(){

  ALLEGRO_TRANSFORM trans, prevTrans;

  // back up the current transform
  al_copy_transform(&prevTrans, al_get_current_transform());

  // scale using the new transform
  al_identity_transform(&trans);

  //al_rotate_transform(&trans, -angle);
  al_translate_transform(&trans, x * 20, y * -20);

  al_use_transform(&trans);

  for( int t = 0; t < 4; t++){
        // Offsets from subtile
    int off_x = (t == 1 || t == 3) ? 16: 0;
    int off_y = (t >= 2) ? 16: 0;

    al_draw_bitmap( new_tiles[t],-(width/2)*20+off_x, -(height/2)*20+off_y, 0);
  }

  al_use_transform(&prevTrans);

  // I suck at programming
  // - Danny Van Stemp, July 30, 2017

}

// Get type
int StaticBox::getType() {
  return STATIC;
}
