#ifndef EDITOR_H
#define EDITOR_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#include <vector>

#include "mouseListener.h"
#include "tools.h"

#include "state.h"

// Nice editor boxes
struct editor_box{
  int x;
  int y;
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

    std::vector<editor_box> editorBoxes;
  private:
};

#endif // EDITOR_H
