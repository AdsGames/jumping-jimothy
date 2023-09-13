#include "StaticBox.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "../util/Globals.h"

// I'm trying to make static boxes really just for show,
// and have the collision handled elsewhere. So, this class
// will inherit for Box.cpp but will (should) only hold
// the location and the image

// Static Box Constructors
StaticBox::StaticBox(const float x, const float y)
    : Box(x, y, 1.5f, 1.5f, nullptr) {}

// Destroy 'custom' image
StaticBox::~StaticBox() {
  // Destroy custom image
  if (sprite != nullptr)
    al_destroy_bitmap(sprite);
}

// Draw box to screen
void StaticBox::draw() {
  // Transform
  ALLEGRO_TRANSFORM trans;
  ALLEGRO_TRANSFORM prevTrans;

  // back up the current transform
  al_copy_transform(&prevTrans, al_get_current_transform());

  // scale using the new transform
  al_identity_transform(&trans);

  al_translate_transform(&trans, getX() * 20, getY() * -20);

  al_use_transform(&trans);

  al_draw_bitmap(sprite, -(getWidth() / 2) * 20, -(getHeight() / 2) * 20, 0);

  al_use_transform(&prevTrans);

  // I suck at programming
  // - Danny Van Stemp, July 30, 2017
}

// Get box type
int StaticBox::getType() {
  return STATIC;
}
