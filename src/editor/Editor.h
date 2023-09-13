/**
 * Editor
 * Allan Legemaate and Danny Van Stemp
 * Level editor to create/edit levels
 * 05/05/2017
 **/

#ifndef EDITOR_H
#define EDITOR_H

#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>

#include <string>
#include <vector>

#include "../State.h"
#include "../ui/UIHandler.h"

// Nice editor boxes
struct editor_box {
  float x;
  float y;
  std::string x_str;
  std::string y_str;
  std::string type_str;
  int type;
  int orientation[4];
  int width;
  int height;
  bool affect_character;
  std::string width_str;
  std::string height_str;
};

// The editor state
class Editor : public State {
 public:
  Editor();
  virtual ~Editor();

  // Override parent
  void update(StateEngine* engine) override;
  void draw() override;

 private:
  // Functions
  void calculate_orientation_global();

  bool box_at(int x, int y);
  bool box_at_with_type(int newType, int x, int y);

  bool is_player();

  bool save_map(std::string mapName);
  bool load_map(std::string mapName);

  static const int BUTTON_COUNT = 17;

  void set_explosive_ui_status();

  UIHandler editorUI;

  // Images
  ALLEGRO_BITMAP* image_box[6]{nullptr};
  ALLEGRO_BITMAP* tiles[5][15]{nullptr};
  ALLEGRO_BITMAP* help_menu{nullptr};
  ALLEGRO_BITMAP* cursor{nullptr};

  // Tiles
  std::vector<editor_box> editorBoxes;

  // Map name
  std::string file_name;

  bool gui_mode{false};
  float box_1_x;
  float box_1_y;

  float box_2_x;
  float box_2_y;

  bool is_dragging_box{false};
  bool dialog_open{false};

  // Remember saves
  bool is_saved{false};
  bool display_help{false};

  // Vars
  bool grid_on{false};

  int explosive_orientation{1};

  int tile_type{0};
  bool modified{false};

  // The font
  ALLEGRO_FONT* edit_font{nullptr};
};

#endif  // EDITOR_H
