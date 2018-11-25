/**
 * INIT
 * Allan Legemaate
 * 09/05/2017
**/
#ifndef INIT_H
#define INIT_H

#include "State.h"

class Init : public State {
  public:
    Init();
    ~Init(){};

    void update(){};
    void draw(){}

  protected:

  private:
};

#endif // INIT_H
