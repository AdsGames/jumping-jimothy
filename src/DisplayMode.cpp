#include "DisplayMode.h"
#include "allegro5/allegro.h"
#include "Options.h"

// Initialize
int DisplayMode::window_w = 1024;
int DisplayMode::window_h = 768;

int DisplayMode::draw_w = 1024;
int DisplayMode::draw_h = 768;

float DisplayMode::scale_x = 0;
float DisplayMode::scale_y = 0;

int DisplayMode::translation_x = 0;
int DisplayMode::translation_y = 0;

int DisplayMode::mode = 0;

ALLEGRO_DISPLAY** DisplayMode::display = nullptr;

// Returns current display mode
int DisplayMode::getDisplayMode() {
  return DisplayMode::mode;
}

// Returns string version of current display mode
std::string DisplayMode::getDisplayModeString() {
  return getDisplayModeString(mode);
}


std::string DisplayMode::getDisplayModeString(int mode) {
// Window mode
  switch (mode) {
    // Fullscreen windowed stretch
    case FULLSCREEN_WINDOW_STRETCH:
      return "Borderless Fullscreen (Stretched)";
      break;

    // Fullscreen window center
    case FULLSCREEN_WINDOW_CENTER:
      return "Borderless Fullscreen (Centered)";
      break;

    // Fullscreen window center
    case FULLSCREEN_WINDOW_LETTERBOX:
      return "Borderless Fullscreen (Letterbox)";
      break;

    // Windowed
    case WINDOWED:
      return "Windowed";
      break;

    // Invalid mode
    default:
      return "Invalid Display Mode";
      break;
  }
}

// Set active display
void DisplayMode::setActiveDisplay(ALLEGRO_DISPLAY **newdisplay) {
  display = newdisplay;
}

// Gets draw width
int DisplayMode::getDrawWidth() {
  return draw_w;
}

// Gets draw height
int DisplayMode::getDrawHeight() {
  return draw_h;
}

// Gets translation x
int DisplayMode::getTranslationX() {
  return translation_x;
}

// Gets translation y
int DisplayMode::getTranslationY() {
  return translation_y;
}

// Gets scale width
int DisplayMode::getScaleWidth() {
  return scale_x * draw_w;
}

// Gets scale height
int DisplayMode::getScaleHeight() {
  return scale_y * draw_h;
}

// Gets scale x
float DisplayMode::getScaleX() {
  return scale_x;
}

// Gets scale y
float DisplayMode::getScaleY() {
  return scale_y;
}


// Set window size
void DisplayMode::setWindowSize(int width, int height) {
  window_w = width;
  window_h = height;
}

// Set scale
void DisplayMode::setScale(float width, float height) {
  scale_x = width;
  scale_y = height;
}

// Set translation
void DisplayMode::setTranslation(int x, int y) {
  translation_x = x;
  translation_y = y;
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

  // Get monitor width
  ALLEGRO_MONITOR_INFO info;
  al_get_monitor_info(0, &info);

  // Display cursor
  bool display_cursor = false;

  // Window mode
  switch (mode) {
    // Fullscreen windowed stretch
    case FULLSCREEN_WINDOW_STRETCH:
      // Set flags
      al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);

      // Set up screen size and positions
      setWindowSize(info.x2 - info.x1, info.y2 - info.y1);
      setScale((float)window_w / draw_w, (float)window_h / draw_h);
      setTranslation(0.0f, 0.0f);

      break;

    // Fullscreen window center
    case FULLSCREEN_WINDOW_CENTER:
      // Set flags
      al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);

      // Set up screen size and positions
      setWindowSize(info.x2 - info.x1, info.y2 - info.y1);
      setScale(1.0f, 1.0f);
      setTranslation((window_w  - scale_x * draw_w) / 2, (window_h - scale_y * draw_h) / 2);

      break;

    // Fullscreen window center
    case FULLSCREEN_WINDOW_LETTERBOX:
      // Set flags
      al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);

      // Set up screen size and positions
      setWindowSize(info.x2 - info.x1, info.y2 - info.y1);
      setScale(std::min((float)window_w / draw_w,
                        (float)window_h / draw_h),
               std::min((float)window_w / draw_w,
                        (float)window_h / draw_h));
      setTranslation((window_w  - scale_x * draw_w) / 2, (window_h - scale_y * draw_h) / 2);

      break;

    // Windowed
    case WINDOWED:
      // Set flags
      al_set_new_display_flags(ALLEGRO_WINDOWED);

      // Set up screen size and positions
      setWindowSize(draw_w, draw_h);
      setScale(1.0f, 1.0f);
      setTranslation(0.0f, 0.0f);

      // Windowed mode requires cursor draw
      display_cursor = true;

      break;

    // Invalid mode
    default:
      std::cout << "WARNING: Invalid display mode passed.\n";
      return;
      break;
  }

  // Create display
  *display = al_create_display(window_w, window_h);

  // Cursor config
  al_hide_mouse_cursor(*display);
  if (display_cursor)
    al_show_mouse_cursor(*display);
  Options::draw_cursor = !display_cursor;

  // Debug screen mode
  std::cout << "Scren mode set to " << getDisplayModeString() << ".\n";
}
