#include "MouseListener.h"
#include "DisplayMode.h"

#include <allegro5/allegro.h>

int MouseListener::mouse_x = 0;
int MouseListener::mouse_y = 0;
int MouseListener::old_mouse_x = 0;
int MouseListener::old_mouse_y = 0;
unsigned char MouseListener::mouse_button = 0;
unsigned char MouseListener::mouse_pressed = 0;
unsigned char MouseListener::mouse_released = 0;
unsigned char MouseListener::mouse_old = 0;
bool MouseListener::mouse_moved = false;

// Check those buttons!
void MouseListener::update() {
  // Get state
  ALLEGRO_MOUSE_STATE state;
  al_get_mouse_state(&state);

  // Position
  mouse_x =
      (state.x - DisplayMode::getTranslationX()) / DisplayMode::getScaleX();
  mouse_y =
      (state.y - DisplayMode::getTranslationY()) / DisplayMode::getScaleY();

  mouse_moved = false;

  if (old_mouse_x != mouse_x || old_mouse_y != mouse_y)
    mouse_moved = true;

  old_mouse_x = state.x;
  old_mouse_y = state.y;

  // Check button just pressed
  for (int i = 0; i < MAX_MOUSE_BUTTONS; i++) {
    // Clear old values
    mouse_pressed &= ~(1 << i);
    mouse_released &= ~(1 << i);

    // Down or not
    const int check_state = ((state.buttons >> i) & 1);
    mouse_button = (mouse_button & ~(1 << i)) | (check_state << i);

    // Pressed since last tick?
    if (((mouse_button >> i) & 1) != 0 && ((mouse_old >> i) & 1) == 0) {
      mouse_pressed |= 1 << i;
    }

    // Released since last tick?
    if (((mouse_button >> i) & 1) == 0 && ((mouse_old >> i) & 1) != 0) {
      mouse_released |= 1 << i;
    }

    // Button changed
    if (((mouse_button >> i) & 1) != ((mouse_old >> i) & 1)) {
      mouse_old ^= 1 << i;
    }
  }
}
