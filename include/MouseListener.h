/**
 * Mouse Listener 2 (allegro 5)
 * Allan Legemaate
 * Listens for mouse buttons JUST pressed or JUST released
 *   since the last tick
 * 18/01/2017
**/

#ifndef MOUSELISTENER_H
#define MOUSELISTENER_H

#define MAX_MOUSE_BUTTONS 3

#define MOUSE_LEFT 1
#define MOUSE_RIGHT 2
#define MOUSE_CENTER 4

class MouseListener {
  public:
    MouseListener() {};
    virtual ~MouseListener() {};

    void update();

    static int mouse_x;
    static int mouse_y;
    static int old_mouse_x;
    static int old_mouse_y;
    static unsigned char mouse_button;
    static unsigned char mouse_pressed;
    static unsigned char mouse_released;
    static bool mouse_moved;

  private:
    static unsigned char mouse_old;
};

#endif // MOUSELISTENER_H
