/**
 * State for machine and State Engine
 * Allan Legemaate
 * 30/12/2016
 * Compartmentalize program into states
 *   which can handle only their own logic,
 *   drawing and transitions
 */

#ifndef STATE_H
#define STATE_H

#include <memory>

// Class
class State;

// Game states
enum class ProgramState { EMPTY, EXIT, MENU, GAME, EDIT, LEVELSELECT, OPTIONS };

/*****************
 * STATE ENGINE
 *****************/
class StateEngine {
 public:
  // Update
  void update();

  // Draw
  void draw();

  // Set next state
  void setNextState(ProgramState newState);

  // Get state id
  ProgramState getStateId() const;

 private:
  // Change state
  void changeState();

  // Current state object
  std::unique_ptr<State> currentState{nullptr};

  // Next state
  ProgramState nextState{ProgramState::EMPTY};

  // State id
  ProgramState stateId{ProgramState::EMPTY};
};

/*********
 * STATE
 *********/
class State {
 public:
  // Virtual destructor
  virtual ~State() = default;

  // Draw to screen
  virtual void draw() = 0;

  // Update logic
  virtual void update(StateEngine* engine) = 0;

  // Change state
  void setNextState(StateEngine* engine, ProgramState state) const;
};

#endif  // STATE_H
