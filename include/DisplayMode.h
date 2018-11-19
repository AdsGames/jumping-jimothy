#ifndef DISPLAYMODE_H
#define DISPLAYMODE_H

#include <allegro5/display.h>

enum {
  fullscreen_window_stretch,
  fullscreen_window_letterbox,
  fullscreen_window_center,
  fullscreen_true,
  windowed
};

class DisplayMode {
  public:
    DisplayMode() {};
    virtual ~DisplayMode() {};

    static int mode;
    static int game_render_width;
    static int game_render_height;
    static int window_render_height;
    static int window_render_width;
    static ALLEGRO_DISPLAY *init(int);
    static ALLEGRO_DISPLAY *gameDisplay;
    static float scale_x;
    static float scale_y;
    static float scale_w;
    static float scale_h;
    static void setMode(ALLEGRO_DISPLAY *newDisplay,int);
  protected:

  private:

};

#endif // DISPLAYMODE_H
