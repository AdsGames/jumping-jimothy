#include "DisplayMode.h"
#include "allegro5/allegro.h"
#include "Options.h"

float DisplayMode::window_width = 1024;
float DisplayMode::window_height = 768;

float DisplayMode::draw_width = 1024;
float DisplayMode::draw_height = 768;

float DisplayMode::scale_h = 0;
float DisplayMode::scale_w = 0;
float DisplayMode::scale_x = 0;
float DisplayMode::scale_y = 0;

int DisplayMode::mode = 0;

std::string DisplayMode::mode_string = "";

ALLEGRO_DISPLAY** DisplayMode::display = nullptr;

// Set active display
void DisplayMode::setActiveDisplay(ALLEGRO_DISPLAY **display) {
  // Set new display
  DisplayMode::display = display;
}

// Change display mode
void DisplayMode::setMode(int mode) {
  // Display hasnt been set
  if (display == nullptr) {
    std::cout << "Display not set.\n";
    return;
  }

  // Destroy existing display
  if (*display != nullptr) {
    al_destroy_display(*display);
    *display = nullptr;
  }

  // Set mode
  DisplayMode::mode = mode;

  // Destroy existing display
  if (*display != nullptr) {
    al_destroy_display(*display);
    *display = nullptr;
  }

  // Get monitor width
  ALLEGRO_MONITOR_INFO info;
  al_get_monitor_info(0, &info);

  // Window mode
  switch (mode) {
    // Fullscreen windowed stretch
    case FULLSCREEN_WINDOW_STRETCH:
      std::cout << "Setting mode to fullscreen windowed.\n";
      mode_string = "Borderless Fullscreen (Stretched)";

      // Set flags
      al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
      al_toggle_display_flag(*display, ALLEGRO_FULLSCREEN_WINDOW, true);

      // Screen size
      window_width = info.x2 - info.x1;
      window_height = info.y2 - info.y1;

      // Set scaling
      scale_w = window_width / draw_width;
      scale_h = window_height / draw_height;
      scale_x = 0.0f;
      scale_y = 0.0f;

      // Create display
      *display = al_create_display(window_width, window_height);

      // Cursor hidden
      al_hide_mouse_cursor(*display);
      Options::draw_cursor = true;
      break;

    // Fullscreen window center
    case FULLSCREEN_WINDOW_CENTER:
      std::cout << "Setting mode to fullscreen centered.\n";
      mode_string = "Borderless Fullscreen (Centered)";

      // Set flags
      al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
      al_toggle_display_flag(*display, ALLEGRO_FULLSCREEN_WINDOW, true);

      // Screen size
      window_width = info.x2 - info.x1;
      window_height = info.y2 - info.y1;

      // Set scaling
      scale_w = 1.0f;
      scale_h = 1.0f;
      scale_x = (window_width  - scale_w * draw_width) / 2;
      scale_y = (window_height - scale_h * draw_height) / 2;

      // Create display
      *display = al_create_display(window_width, window_height);

      // Cursor hidden
      al_hide_mouse_cursor(*display);
      Options::draw_cursor = true;
      break;

    // Fullscreen window center
    case FULLSCREEN_WINDOW_LETTERBOX:
      std::cout << "Setting mode to fullscreen letterbox.\n";
      mode_string = "Borderless Fullscreen (Letterbox)";

      // Set flags
      al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
      al_toggle_display_flag(*display, ALLEGRO_FULLSCREEN_WINDOW, true);

      // Screen size
      window_width = info.x2 - info.x1;
      window_height = info.y2 - info.y1;

      // Calculate max scale value
      scale_w = window_width / draw_width;
      scale_h = window_height / draw_height;

      // Set scaling
      scale_w = std::min(scale_w, scale_h);
      scale_h = std::min(scale_w, scale_h);
      scale_x = (window_width  - scale_w * draw_width) / 2;
      scale_y = (window_height - scale_h * draw_height) / 2;

      // Create display
      *display = al_create_display(window_width, window_height);

      // Cursor hidden
      al_hide_mouse_cursor(*display);
      Options::draw_cursor = true;
      break;

    // Windowed
    case WINDOWED:
      std::cout << "Setting mode to windowed.\n";
      mode_string = "Windowed";

      // Set flags
      al_set_new_display_flags(ALLEGRO_WINDOWED);
      al_toggle_display_flag(*display, ALLEGRO_WINDOWED, false);

      // Screen size
      window_height = draw_height;
      window_width = draw_width;

      // Set scaling
      scale_w = 1.0f;
      scale_h = 1.0f;
      scale_x = 0.0f;
      scale_y = 0.0f;

      // Create display
      *display = al_create_display(window_width, window_height);

      // Show cursor
      al_show_mouse_cursor(*display);
      Options::draw_cursor = false;
      break;

    // Invalid mode
    default:
      std::cout << "WARNING: Invalid display mode passed.\n";
      break;
  }
}
