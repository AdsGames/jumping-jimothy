#ifndef EDITOR_H
#define EDITOR_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#include <vector>
#include <string>

#include "rapidxml.hpp"
#include "rapidxml_print.hpp"

#include "mouseListener.h"
#include "keyListener.h"
#include "tools.h"

#include "state.h"

// Nice editor boxes
struct editor_box{
  int x;
  int y;
  std::string x_str;
  std::string y_str;
  std::string bodyType;
};

class editor : public state{
  public:
    editor();
    virtual ~editor();

    // Override parent
    void update();
    void draw();
  protected:
    ALLEGRO_BITMAP *image_box;
    ALLEGRO_BITMAP *edges[16];

    std::vector<editor_box> editorBoxes;
    bool box_at( int x, int y);

    void save_map( std::string mapName);


  private:
};

#endif // EDITOR_H
