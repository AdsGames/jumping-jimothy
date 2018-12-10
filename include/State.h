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

// Class
class State;

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
    void setNextState(int newState);

    // Get state id
    int getStateId();

    // Game states
    enum programStates {
      STATE_NULL,
      STATE_EXIT,
      STATE_MENU,
      STATE_GAME,
      STATE_EDIT,
      STATE_LEVELSELECT,
      STATE_OPTIONS
    };
  private:
    // Change state
    void changeState();

    // Current state object
    State *currentState = nullptr;

    // Next state
    int nextState = STATE_NULL;

    // State id
    int stateId = STATE_NULL;

};

/*********
 * STATE
 *********/
class State {
  public:
    // Virtual destructor
    virtual ~State() {};

    // Draw to screen
    virtual void draw() = 0;

    // Update logic
    virtual void update(StateEngine* engine) = 0;

    // Change state
    void setNextState(StateEngine* engine, int state);
};

#endif // STATE_H
