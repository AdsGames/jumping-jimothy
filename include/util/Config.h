/**
 * Config
 * Allan Legemaate
 * Global configuration. Responsible for
 *   reading config data and updating on close.
 * 22/11/2018
 **/

#ifndef CONFIG_H
#define CONFIG_H

#include <string>

class Config {
  public:
    Config() {};
    virtual ~Config() {};

    static void read_data(std::string path);
    static void write_data(std::string path);

    static bool sfx_enabled;
    static bool music_enabled;
    static int graphics_mode;
    static bool draw_cursor;

    static std::string joystick_data;
    static bool joystick_mode;

    static int level_to_start;

  protected:

  private:
};

#endif // CONFIG_H
