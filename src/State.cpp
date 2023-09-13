#include "State.h"

#include "editor/Editor.h"
#include "game/Game.h"
#include "menu/LevelSelect.h"
#include "menu/Menu.h"
#include "menu/Options.h"

#include "util/Tools.h"

/*****************
 * STATE ENGINE
 *****************/

// Draw
void StateEngine::draw() {
  if (currentState) {
    currentState->draw();
  }
}

// Update
void StateEngine::update() {
  if (currentState) {
    currentState->update(this);
  }
  changeState();
}

// Set next state
void StateEngine::setNextState(ProgramState newState) {
  nextState = newState;
}

// Get state id
ProgramState StateEngine::getStateId() const {
  return stateId;
}

// Change game screen
void StateEngine::changeState() {
  // If the state needs to be changed
  if (nextState == ProgramState::EMPTY) {
    return;
  }

  // Change the state
  switch (nextState) {
    case ProgramState::GAME:
      currentState = std::make_unique<Game>();
      tools::log_message("Switched state to game.");
      break;
    case ProgramState::EDIT:
      currentState = std::make_unique<Editor>();
      tools::log_message("Switched state to editor.");
      break;
    case ProgramState::MENU:
      currentState = std::make_unique<Menu>();
      tools::log_message("Switched state to main menu.");
      break;
    case ProgramState::LEVELSELECT:
      currentState = std::make_unique<LevelSelect>();
      tools::log_message("Switched state to level select.");
      break;
    case ProgramState::OPTIONS:
      currentState = std::make_unique<Options>();
      tools::log_message("Switched state to options.");
      break;
    default:
      currentState = nullptr;
      tools::log_message("Exiting program.");
  }

  // Change the current state ID
  stateId = nextState;

  // NULL the next state ID
  nextState = ProgramState::EMPTY;
}

/*********
 * STATE
 *********/

// Change state
void State::setNextState(StateEngine* engine, ProgramState state) const {
  engine->setNextState(state);
}
