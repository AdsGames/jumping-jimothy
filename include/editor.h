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
};

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
    ALLEGRO_BITMAP *tiles[4][16];

    // Functions
    int calculate_orientation(int);
    void calculate_orientation_global();

    bool box_at(int x, int y);
    bool box_at_with_type(int newType,int x, int y);

    void save_map( std::string mapName);
    void load_map( std::string mapName);

    // Tiles
    std::vector<editor_box> editorBoxes;

    // Vars
    bool grid_on;

    int tile_type;
    int level_number;

    // The font
    ALLEGRO_FONT *edit_font;
};

#endif // EDITOR_H
