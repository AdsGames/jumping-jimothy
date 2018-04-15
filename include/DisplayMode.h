#ifndef DISPLAYMODE_H
#define DISPLAYMODE_H


enum{
  fullscreen_window_stretch,
  fullscreen_window_letterbox,
  fullscreen_window_center,
  fullscreen_true,
  windowed,
};

class DisplayMode
{
  public:
    DisplayMode();
    virtual ~DisplayMode();

    static int mode;
    static int game_render_width;
    static int game_render_height;
    static int window_render_height;
    static int window_render_width;

  protected:

  private:


};

#endif // DISPLAYMODE_H
