/*
 * Display Mode
 * Sets up and changes passed display
 * Danny Vanstemp and Allan Legemaate
 * 20/11/2018
 */
#ifndef DISPLAYMODE_H
#define DISPLAYMODE_H

#include <allegro5/display.h>
#include <string>

#define NUM_GRAPHICS_MODES 4

enum {
  FULLSCREEN_WINDOW_STRETCH,
  FULLSCREEN_WINDOW_LETTERBOX,
  FULLSCREEN_WINDOW_CENTER,
  WINDOWED
};

class DisplayMode {
  public:
    DisplayMode() {};
    virtual ~DisplayMode() {};

    static void setMode(int);
    static void setActiveDisplay(ALLEGRO_DISPLAY **display);

    static int getDisplayMode() { return mode; }
    static std::string getDisplayModeString() { return mode_string; }

    // Size of drawing buffer
    static float draw_width;
    static float draw_height;

    // Size of window
    static float window_height;
    static float window_width;

    // Translation amount
    static float scale_x;
    static float scale_y;

    // Draw buffer scale percent (as decimal)
    static float scale_w;
    static float scale_h;

    // Draw cursor enabled?
    static bool draw_cursor;
  protected:

  private:
    static int mode;
    static ALLEGRO_DISPLAY** display;

    static std::string mode_string;

};

#endif // DISPLAYMODE_H
