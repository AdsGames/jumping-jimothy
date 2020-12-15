#include "KeyListener.h"

bool KeyListener::key[ALLEGRO_KEY_MAX] = {false};
bool KeyListener::keyPressed[ALLEGRO_KEY_MAX] = {false};
bool KeyListener::keyReleased[ALLEGRO_KEY_MAX] = {false};
bool KeyListener::lastTicksKey[ALLEGRO_KEY_MAX] = {false};
int KeyListener::lastKeyPressed = -1;
int KeyListener::lastKeyReleased = -1;
bool KeyListener::anyKeyPressed = false;

// For allegro 5, we use events
void KeyListener::on_event(ALLEGRO_EVENT_TYPE event_type, const int keycode) {
  // Key down
  if (event_type == ALLEGRO_EVENT_KEY_DOWN) {
    key[keycode] = true;
  } else if (event_type == ALLEGRO_EVENT_KEY_UP) {
    key[keycode] = false;
  }
}

// Check those keys!
void KeyListener::update() {
  // Reset last key
  lastKeyPressed = -1;
  lastKeyReleased = -1;

  anyKeyPressed = false;

  // Check key just pressed
  for (int i = 0; i < ALLEGRO_KEY_MAX; i++) {
    // Clear old values
    keyPressed[i] = false;
    keyReleased[i] = false;

    if (key[i])
      anyKeyPressed = true;

    // Pressed since last tick?
    if (key[i] == true && lastTicksKey[i] == false) {
      keyPressed[i] = true;
      lastKeyPressed = i;
    }

    // Released since last tick?
    if (key[i] == false && lastTicksKey[i] == true) {
      keyReleased[i] = true;
      lastKeyReleased = i;
    }
  }

  // Get new values
  for (int i = 0; i < ALLEGRO_KEY_MAX; i++) {
    // Key changed
    if (lastTicksKey[i] != key[i]) {
      lastTicksKey[i] = key[i];
    }
  }
}
