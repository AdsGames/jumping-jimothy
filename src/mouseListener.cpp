#include "mouseListener.h"

int mouseListener::mouse_x = 0;
int mouseListener::mouse_y = 0;
unsigned char mouseListener::mouse_button = 0;
unsigned char mouseListener::mouse_pressed = 0;
unsigned char mouseListener::mouse_released = 0;
unsigned char mouseListener::mouse_old = 0;

mouseListener::mouseListener(){
}

mouseListener::~mouseListener(){
}


// Check those buttons!
void mouseListener::update(){
  // Get state
  ALLEGRO_MOUSE_STATE state;
  al_get_mouse_state(&state);

  // Position
  mouse_x = state.x;
  mouse_y = state.y;

  // Check button just pressed
  for( int i = 0; i < MAX_MOUSE_BUTTONS; i ++){
    // Clear old values
    mouse_pressed &= ~(1 << i);
    mouse_released &= ~(1 << i);

    // Down or not
    int check_state = ((state.buttons >> i) & 1);
    mouse_button = (mouse_button & ~(1 << i)) | (check_state << i);

    // Pressed since last tick?
    if( ((mouse_button >> i) & 1) != 0 && ((mouse_old >> i) & 1) == 0){
      mouse_pressed |= 1 << i;
      //std::cout << "Button: " << i << " pressed. \n";
    }

    // Released since last tick?
    if( ((mouse_button >> i) & 1) == 0 && ((mouse_old >> i) & 1) != 0){
      mouse_released |= 1 << i;
      //std::cout << "Button: " << i << " released. \n";
    }

    // Button changed
    if( ((mouse_button >> i) & 1) != ((mouse_old >> i) & 1)){
        //std::cout << "Button: " << i << " was " << ((mouse_old >> i) & 1) << " and became " << ((mouse_button >> i) & 1) << "\n";
        mouse_old ^= 1 << i;
    }
  }
}
