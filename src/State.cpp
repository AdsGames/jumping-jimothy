#include "State.h"

//State variables
int stateID = STATE_NULL;
int nextState = STATE_NULL;

// Set next state
void set_next_state (int newState) {
  if (nextState != STATE_EXIT) {
    nextState = newState;
  }
}
