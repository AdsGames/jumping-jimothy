#include "State.h"

#include "game/Game.h"
#include "editor/Editor.h"
#include "menu/Menu.h"
#include "menu/LevelSelect.h"
#include "menu/Options.h"

#include "util/Tools.h"

/*****************
 * STATE ENGINE
 *****************/

// Draw
void StateEngine::draw() {
  if (currentState) {
    currentState -> draw();
  }
}

// Update
void StateEngine::update() {
  if (currentState) {
    currentState -> update(this);
  }
  changeState();
}

// Set next state
void StateEngine::setNextState(int newState) {
  nextState = newState;
}

// Get state id
int StateEngine::getStateId() {
  return stateId;
}

// Change game screen
void StateEngine::changeState() {
  // If the state needs to be changed
  if (nextState == STATE_NULL) {
    return;
  }

  // Delete the current state
  if (currentState != nullptr) {
    delete currentState;
  }

  // Change the state
  switch(nextState) {
    case STATE_GAME:
      currentState = new Game();
      tools::log_message("Switched state to game.");
      break;
    case STATE_EDIT:
      currentState = new Editor();
      tools::log_message("Switched state to editor.");
      break;
    case STATE_MENU:
      currentState = new Menu();
      tools::log_message("Switched state to main menu.");
      break;
    case STATE_LEVELSELECT:
      currentState = new LevelSelect();
      tools::log_message("Switched state to level select.");
      break;
    case STATE_OPTIONS:
      currentState = new Options();
      tools::log_message("Switched state to options.");
      break;
    default:
      currentState = nullptr;
      tools::log_message("Exiting program.");
  }

  // Change the current state ID
  stateId = nextState;

  // NULL the next state ID
  nextState = STATE_NULL;
}


/*********
 * STATE
 *********/

// Change state
void State::setNextState(StateEngine* engine, int state) {
  engine -> setNextState(state);
}
