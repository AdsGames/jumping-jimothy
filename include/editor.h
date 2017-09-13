/**
 * BOX
 * Allan Legemaate
 * 05/05/2017
**/
#ifndef EDITOR_H
#define EDITOR_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_native_dialog.h>

#include <vector>
#include <string>
#include <math.h>
#include <algorithm>

#include "rapidxml.hpp"
#include "rapidxml_print.hpp"

#include "mouseListener.h"
#include "keyListener.h"
#include "tools.h"
#include "button.h"
#include "globals.h"
#include "game.h"

#include "state.h"

// Nice editor boxes
struct editor_box{
  float x;
  float y;
  std::string x_str;
  std::string y_str;
  std::string bodyType;
  int type;
  int orientation[4];
  int width;
  int height;
  std::string width_str;
  std::string height_str;
};

// Button names
enum button_names{
  button_type_dynamic,
  button_type_static,
  button_type_player,
  button_type_goat,
  button_type_collision,
  button_hide_left,
  button_hide_right,
  button_undo,
  button_clear,
  button_save,
  button_save_as,
  button_load,
  button_play,
  button_grid,
  button_help,
  button_back,
  button_hide_top
};

// The editor state
class editor : public state{
  public:
    editor();
    virtual ~editor();

    // Override parent
    void update();
    void draw();
  private:
    // Imagess
    ALLEGRO_BITMAP *image_box[4];
    ALLEGRO_BITMAP *tiles[4][15];

    // Functions
    void calculate_orientation_global();

    bool box_at(int x, int y);
    bool box_at_with_type(int newType,int x, int y);

    bool save_map( std::string mapName);
    bool load_map( std::string mapName);

    static const int BUTTON_COUNT = 17;

    // Type buttons
    button editor_buttons[BUTTON_COUNT];

    // Tiles
    std::vector<editor_box> editorBoxes;

    // Map name
    const char *file_name;

    bool gui_mode;

    float box_1_x;
    float box_1_y;

    float box_2_x;
    float box_2_y;

    bool is_dragging_box = false;
    bool dialog_open = false;

    // Remember saves
    bool is_saved = false;

    // Vars
    bool grid_on;

    int tile_type;
    int level_number;
    bool test_mode = false;

    // The font
    ALLEGRO_FONT *edit_font;
};

#endif // EDITOR_H
