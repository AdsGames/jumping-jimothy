#include "keyListener.h"

bool keyListener::key[ALLEGRO_KEY_MAX] = { false };
bool keyListener::keyPressed[ALLEGRO_KEY_MAX] = { false};
bool keyListener::keyReleased[ALLEGRO_KEY_MAX] = { false};
bool keyListener::lastTicksKey[ALLEGRO_KEY_MAX] = { false};
int keyListener::lastKeyPressed = -1;
int keyListener::lastKeyReleased = -1;
bool keyListener::anyKeyPressed=false;

// Constructor
keyListener::keyListener(){

}

// Destructor
keyListener::~keyListener(){

}

// For allegro 5, we use events
void keyListener::on_event( ALLEGRO_EVENT_TYPE event_type, int keycode){
  // Key down
  if( event_type == ALLEGRO_EVENT_KEY_DOWN){
    key[keycode] = true;
  }
  else if( event_type == ALLEGRO_EVENT_KEY_UP){
    key[keycode] = false;
  }
}

// Check those keys!
void keyListener::update(){
  // Reset last key
  lastKeyPressed = -1;
  lastKeyReleased = -1;

  anyKeyPressed=false;
  // Check key just pressed
  for( int i = 0; i < ALLEGRO_KEY_MAX; i++){
    // Clear old values
    keyPressed[i] = false;
    keyReleased[i] = false;


    if(key[i])
      anyKeyPressed=true;




    // Pressed since last tick?
    if( key[i] == true && lastTicksKey[i] == false){
      keyPressed[i] = true;
      lastKeyPressed = i;
      //std::cout << "Key: " << i << " pressed. \n";
    }

    // Released since last tick?
    if( key[i] == false && lastTicksKey[i] == true){
      keyReleased[i] = true;
      lastKeyReleased = i;
      //std::cout << "Key: " << i << " released. \n";
    }
  }


  // Get new values
  for( int i = 0; i < ALLEGRO_KEY_MAX; i++){
    // Key changed
    if( lastTicksKey[i] != key[i]){
        //std::cout << "Key: " << i << " was " << lastTicksKey[i] << " and became " << (bool)key[i] << "\n";
        lastTicksKey[i] = key[i];
    }
  }
}
