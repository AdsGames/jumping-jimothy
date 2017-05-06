#ifndef MENU_H
#define MENU_H

#include "state.h"

// Menu
class menu : public state{
  public:
    menu();
    virtual ~menu();

    void update();
    void draw();

  protected:

  private:
};

#endif // MENU_H
