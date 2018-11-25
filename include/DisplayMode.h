/*
 * Display Mode
 * Sets up and changes passed display
 * Danny Van Stemp and Allan Legemaate
 * 20/11/2018
 */
#ifndef DISPLAYMODE_H
#define DISPLAYMODE_H

#include <allegro5/display.h>
#include <string>

#define NUM_GRAPHICS_MODES 4

// Possible screen modes
enum {
  FULLSCREEN_WINDOW_STRETCH,
  FULLSCREEN_WINDOW_LETTERBOX,
  FULLSCREEN_WINDOW_CENTER,
  WINDOWED
};

class DisplayMode {
  public:
    // Ctor and Dtor
    DisplayMode() {};
    virtual ~DisplayMode() {};

    // Setters
    static void setMode(int);
    static void setActiveDisplay(ALLEGRO_DISPLAY **newdisplay);

    // Getters
    static int getDisplayMode();
    static std::string getDisplayModeString();
    static std::string getDisplayModeString(int mode);

    static int getDrawWidth();
    static int getDrawHeight();

    static int getTranslationX();
    static int getTranslationY();

    static float getScaleX();
    static float getScaleY();

    static int getScaleWidth();
    static int getScaleHeight();

  private:
    // Size of drawing buffer
    static int draw_w;
    static int draw_h;

    // Size of window
    static int window_w;
    static int window_h;

    // Translation amount
    static int translation_x;
    static int translation_y;

    // Draw buffer scale percent (as decimal)
    static float scale_x;
    static float scale_y;

    // Active display
    static ALLEGRO_DISPLAY** display;

    // Private setters to reduce copy code
    static void setWindowSize(int width, int height);
    static void setScale(float x, float y);
    static void setTranslation(int x, int y);

};

#endif // DISPLAYMODE_H
