/**
 * Editor
 * Allan Legemaate and Danny Van Stemp
 * Level editor to create/edit levels
 * 05/05/2017
 **/

#pragma once

#include <array>
#include <string>
#include <vector>

#include <asw/asw.h>

#include "../State.h"
#include "../game/Level.h"
#include "ExplosiveButton.h"

// A placed object, positions are in screen pixels
struct EditorBox {
  ObjectType type{ObjectType::Dynamic};

  // Top left
  float x{0};
  float y{0};

  // Collision boxes can be any size, everything else fills one cell
  float width{32};
  float height{32};

  std::array<int, 4> orientation{};
  bool affect_character{false};
};

class Editor : public asw::scene::Scene<ProgramState> {
 public:
  using asw::scene::Scene<ProgramState>::Scene;

  void init() override;
  void update(float dt) override;
  void draw() override;
  void cleanup() override;

 private:
  // What to do with the file the chooser returns
  enum class FileAction { None, Save, SaveAs, Load };

  void createUI();
  void setExplosiveOrientation(int orientation);
  void setTileType(ObjectType type);
  void setExplosiveUIVisible(bool visible);

  void handleShortcuts();
  void undo();
  void clear();

  // Buttons that hide and show groups of buttons to free up room
  void toggleLeftBottom();
  void toggleRightBottom();
  void toggleRightTop();
  void toggleLeftTop();
  void handleFileChosen(FileAction action, const std::string& path);
  void placeTiles();
  void dragCollisionBox();
  void removeTiles();

  void save();
  void saveAs();
  void load();
  void play();
  void back();

  // Pick the tile of each corner of the static boxes from their neighbours
  void calculateOrientations();

  bool boxAt(ObjectType type, float x, float y) const;
  bool hasPlayer() const;

  bool saveMap(const std::string& path) const;
  bool loadMap(const std::string& path);

  void drawBox(const EditorBox& box) const;

  // Leave for another scene, no more ticks run here
  void changeScene(ProgramState state);

  // Images
  asw::Texture box_green;
  asw::SpriteSheet tile_sheet;
  asw::SpriteSheet character_sheet;
  asw::SpriteSheet goat_sheet;
  asw::Texture box_repel;
  asw::Texture box_repel_direction;
  asw::Texture help_menu;

  asw::Font edit_font;

  // UI
  asw::ui::Root ui;
  std::vector<asw::ui::Button*> type_buttons;
  std::vector<asw::ui::Button*> file_buttons;
  asw::ui::Button* left_bottom_toggle{nullptr};
  asw::ui::Button* right_bottom_toggle{nullptr};
  asw::ui::Button* right_top_toggle{nullptr};
  asw::ui::Button* btn_help{nullptr};
  asw::ui::Button* btn_back{nullptr};
  asw::ui::Checkbox* chk_affects_char{nullptr};
  asw::ui::Button* left_top_toggle{nullptr};

  // Explosive direction buttons, indexed by orientation
  std::array<ExplosiveButton*, 5> explosive_buttons{};

  // The mouse is on the UI this tick, so it does not edit the level
  bool ui_used{false};

  // Level
  std::vector<EditorBox> boxes;
  std::string help_text;
  std::string file_name;

  // Collision box drag, in grid cells
  bool is_dragging_box{false};
  asw::Vec2<float> drag_start;
  asw::Vec2<float> drag_end;

  FileAction pending_file_action{FileAction::None};

  bool is_saved{false};
  bool modified{false};
  bool display_help{false};
  bool grid_on{false};
  bool leaving{false};

  int explosive_orientation{1};
  ObjectType tile_type{ObjectType::Dynamic};
};
