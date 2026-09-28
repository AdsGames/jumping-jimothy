#include "Editor.h"

#include <algorithm>
#include <cmath>
#include <filesystem>
#include <initializer_list>
#include <numbers>
#include <utility>

#include "../Globals.h"
#include "../util/Audio.h"
#include "../util/Config.h"

namespace {
using asw::input::Key;
using asw::input::MouseButton;

constexpr float CELL = 32;
constexpr float HALF_CELL = CELL / 2;

constexpr float BAR_Y = 728;

constexpr float TILE_SIZE = 16;

// Candidate tiles for a static box corner
constexpr int TILE_OPTIONS = 13;

const asw::Color BLACK(0, 0, 0);
const asw::Color SELECTED(0, 150, 0);
const asw::Color UNSELECTED(200, 200, 200);
const asw::Color COLLISION_FILL(0, 255, 0, 50);

const std::vector<asw::dialog::FileFilter> LEVEL_FILTERS{
    {"Levels", "xml"},
    {"All files", "*"},
};

// Explosive button order matches the orientation values
constexpr std::array<float, 5> EXPLOSIVE_BUTTON_X{152, 0, 38, 76, 114};

std::string defaultFile() {
  return Config::savePath() + "untitled.xml";
}

// Snap a pixel position to the top left of its grid cell
float snap(float value) {
  return std::floor(value / CELL) * CELL;
}

const char* typeName(ObjectType type) {
  switch (type) {
    case ObjectType::Dynamic:
      return "Dynamic";
    case ObjectType::Static:
      return "Static";
    case ObjectType::Character:
      return "Character spawn";
    case ObjectType::Finish:
      return "Endgame goat";
    case ObjectType::Collision:
      return "Collision Box";
    case ObjectType::Explosive:
      return "Explosive Box";
  }

  return "";
}

// Level files hold centres in metres with y up, the editor top left pixels
LevelObject toLevelObject(const EditorBox& box) {
  LevelObject object;
  object.type = box.type;
  object.x = (box.x + (box.width / 2)) / PIXELS_PER_METER;
  object.y = -(box.y + (box.height / 2)) / PIXELS_PER_METER;
  object.orientation = box.orientation;
  object.affect_character = box.affect_character;

  if (box.type == ObjectType::Collision) {
    object.width = box.width / PIXELS_PER_METER;
    object.height = box.height / PIXELS_PER_METER;
  }

  return object;
}

EditorBox fromLevelObject(const LevelObject& object) {
  EditorBox box;
  box.type = object.type;
  box.orientation = object.orientation;
  box.affect_character = object.affect_character;

  if (object.type == ObjectType::Collision) {
    box.width = object.width * PIXELS_PER_METER;
    box.height = object.height * PIXELS_PER_METER;
  }

  box.x = (object.x * PIXELS_PER_METER) - (box.width / 2);
  box.y = (-object.y * PIXELS_PER_METER) - (box.height / 2);

  return box;
}
}  // namespace

void Editor::init() {
  leaving = false;

  box_green = asw::assets::load_texture("assets/images/box_green.png");
  tile_sheet = asw::SpriteSheet(
      asw::assets::load_texture("assets/images/StaticBlock.png"),
      asw::Vec2<float>(TILE_SIZE, TILE_SIZE));
  character_sheet = asw::SpriteSheet(
      asw::assets::load_texture("assets/images/character.png"),
      asw::Vec2<float>(CELL, CELL * 2));
  goat_sheet =
      asw::SpriteSheet(asw::assets::load_texture("assets/images/goat.png"),
                       asw::Vec2<float>(CELL, CELL * 2));
  box_repel = asw::assets::load_texture("assets/images/box_repel.png");
  box_repel_direction =
      asw::assets::load_texture("assets/images/box_repel_direction.png");
  help_menu = asw::assets::load_texture("assets/images/help_menu.png");

  edit_font = asw::assets::load_font("assets/fonts/fantasque.ttf", 18);

  boxes.clear();
  help_text.clear();
  tile_type = ObjectType::Dynamic;
  explosive_orientation = 1;
  is_dragging_box = false;
  pending_file_action = FileAction::None;
  modified = false;
  display_help = false;
  grid_on = false;

  createUI();

  Audio::stopMusic();

  // Coming back from testing a level
  file_name = defaultFile();
  is_saved = false;

  if (session.editing_level && loadMap(session.editing_file)) {
    file_name = session.editing_file;
    is_saved = true;
  }

  session.editing_level = false;
  session.editing_file.clear();
}

void Editor::cleanup() {
  ui.clear();
  boxes.clear();
  asw::scene::Scene<ProgramState>::cleanup();
}

void Editor::changeScene(ProgramState state) {
  leaving = true;
  manager.set_next_scene(state);
}

void Editor::createUI() {
  ui.clear();

  // Bottom left, object types
  btn_dynamic = &ui.add<Button>(0, BAR_Y, "Dynamic", edit_font);
  btn_static = &ui.addAfter<Button>(*btn_dynamic, "Static", edit_font);
  btn_player = &ui.addAfter<Button>(*btn_static, "Player", edit_font);
  btn_goat = &ui.addAfter<Button>(*btn_player, "Goat", edit_font);
  btn_collision = &ui.addAfter<Button>(*btn_goat, "Collision", edit_font);
  btn_explosive = &ui.addAfter<Button>(*btn_collision, "Explosive", edit_font);
  left_bottom_toggle = &ui.addAfter<Button>(*btn_explosive, "<", edit_font);

  // Bottom right, file actions
  right_bottom_toggle = &ui.add<Button>(566, BAR_Y, ">", edit_font);
  btn_undo = &ui.addAfter<Button>(*right_bottom_toggle, "Undo", edit_font);
  btn_clear = &ui.addAfter<Button>(*btn_undo, "Clear", edit_font);
  btn_save = &ui.addAfter<Button>(*btn_clear, "Save", edit_font);
  btn_save_as = &ui.addAfter<Button>(*btn_save, "Save as", edit_font);
  btn_load = &ui.addAfter<Button>(*btn_save_as, "Load", edit_font);
  btn_grid = &ui.addAfter<Button>(*btn_load, "Grid", edit_font);
  btn_play = &ui.addAfter<Button>(*btn_grid, "Play", edit_font);

  // Top right
  right_top_toggle = &ui.add<Button>(882, 0, ">", edit_font);
  btn_help = &ui.add<Button>(911, 0, "Help", edit_font);
  btn_back = &ui.addAfter<Button>(*btn_help, "Back", edit_font);

  // Top left, explosive settings
  chk_affects_char =
      &ui.add<CheckBox>(0, 60, "Block affects character", edit_font);
  left_top_toggle = &ui.addAfter<Button>(*chk_affects_char, "<", edit_font);

  for (std::size_t i = 0; i < explosive_buttons.size(); i++) {
    auto& button = ui.add<Button>(EXPLOSIVE_BUTTON_X[i], 100, "", nullptr);

    if (i == 0) {
      button.setImage(box_repel);
    } else {
      button.setImage(box_repel_direction);
      button.setImageRotation((std::numbers::pi_v<float> / 2.0F) *
                              static_cast<float>(i - 1));
    }

    button.setPadding(2, 2);
    explosive_buttons[i] = &button;
  }

  setExplosiveUIVisible(false);
}

void Editor::setExplosiveUIVisible(bool visible) {
  chk_affects_char->setVisible(visible);
  left_top_toggle->setVisible(visible);

  for (auto* button : explosive_buttons) {
    button->setVisible(visible);
  }
}

void Editor::setTileType(ObjectType type) {
  tile_type = type;
  setExplosiveUIVisible(type == ObjectType::Explosive);

  if (type == ObjectType::Explosive) {
    left_top_toggle->setText("<");
    left_top_toggle->setTransparency(255);
    left_top_toggle->setPosition(257, 60);
  }
}

void Editor::updateExplosiveButtons() {
  for (std::size_t i = 0; i < explosive_buttons.size(); i++) {
    if (explosive_buttons[i]->clicked()) {
      explosive_orientation = static_cast<int>(i);
    }
  }

  for (std::size_t i = 0; i < explosive_buttons.size(); i++) {
    explosive_buttons[i]->setBackgroundColour(
        static_cast<int>(i) == explosive_orientation ? SELECTED : UNSELECTED);
  }
}

void Editor::update(float /*dt*/) {
  if (leaving) {
    return;
  }

  // Wait for the file chooser
  if (pending_file_action != FileAction::None) {
    if (asw::dialog::is_file_pending()) {
      return;
    }

    const auto action = std::exchange(pending_file_action, FileAction::None);
    if (const auto path = asw::dialog::take_file()) {
      handleFileChosen(action, *path);
    }
    return;
  }

  ui.update();
  updateExplosiveButtons();

  handleShortcutsAndButtons();
  if (leaving || pending_file_action != FileAction::None) {
    return;
  }

  handleToggles();

  if (tile_type == ObjectType::Collision) {
    dragCollisionBox();
  } else {
    placeTiles();
  }

  removeTiles();
}

void Editor::handleShortcutsAndButtons() {
  // Object types
  if (asw::input::get_key_down(Key::Q) || btn_dynamic->clicked()) {
    setTileType(ObjectType::Dynamic);
  }

  if (asw::input::get_key_down(Key::W) || btn_static->clicked()) {
    setTileType(ObjectType::Static);
  }

  if (asw::input::get_key_down(Key::E) || btn_player->clicked()) {
    setTileType(ObjectType::Character);
  }

  if (asw::input::get_key_down(Key::R) || btn_goat->clicked()) {
    setTileType(ObjectType::Finish);
  }

  if (asw::input::get_key_down(Key::T) || btn_collision->clicked()) {
    setTileType(ObjectType::Collision);
  }

  if (asw::input::get_key_down(Key::Y) || btn_explosive->clicked()) {
    setTileType(ObjectType::Explosive);
  }

  if (asw::input::get_key_down(Key::H) || btn_help->clicked()) {
    display_help = !display_help;
  }

  if (asw::input::get_key_down(Key::G) || btn_grid->clicked()) {
    grid_on = !grid_on;
  }

  if ((asw::input::get_key_down(Key::Z) || btn_undo->clicked()) && !boxes.empty()) {
    boxes.pop_back();
    modified = true;
    calculateOrientations();
  }

  if (asw::input::get_key_down(Key::C) || btn_clear->clicked()) {
    if (asw::dialog::confirm("Clear?",
                        "Clear the map? There is no recovering this "
                        "masterpiece.")) {
      boxes.clear();
      modified = true;
    }
  }

  if (asw::input::get_key_down(Key::S) || btn_save->clicked()) {
    save();
  } else if (asw::input::get_key_down(Key::D) || btn_save_as->clicked()) {
    saveAs();
  } else if (asw::input::get_key_down(Key::A) || btn_load->clicked()) {
    load();
  } else if (asw::input::get_key_down(Key::F) || btn_play->clicked()) {
    play();
  } else if (asw::input::get_key_down(Key::V) || asw::input::get_key_down(Key::Escape) ||
             btn_back->clicked()) {
    back();
  }
}

// Buttons that hide and show groups of buttons to free up room
void Editor::handleToggles() {
  if (left_bottom_toggle->clicked() || asw::input::get_key_down(Key::Left)) {
    const bool show = left_bottom_toggle->getText() != "<";

    if (show) {
      left_bottom_toggle->setPosition(489, BAR_Y);
      left_bottom_toggle->setText("<");
      left_bottom_toggle->setTransparency(255);
    } else {
      left_bottom_toggle->setPosition(0, BAR_Y);
      left_bottom_toggle->setText(">");
      left_bottom_toggle->setTransparency(150);
    }

    for (auto* button : {btn_collision, btn_static, btn_dynamic, btn_player,
                         btn_goat, btn_explosive}) {
      button->setVisible(show);
    }
  }

  if (right_bottom_toggle->clicked() || asw::input::get_key_down(Key::Right)) {
    const bool show = right_bottom_toggle->getText() == "<";

    if (show) {
      right_bottom_toggle->setPosition(566, BAR_Y);
      right_bottom_toggle->setText(">");
      right_bottom_toggle->setTransparency(255);
    } else {
      right_bottom_toggle->setPosition(994, BAR_Y);
      right_bottom_toggle->setText("<");
      right_bottom_toggle->setTransparency(150);
    }

    for (auto* button : {btn_undo, btn_clear, btn_save, btn_save_as, btn_load,
                         btn_play, btn_grid}) {
      button->setVisible(show);
    }
  }

  if (right_top_toggle->clicked() || asw::input::get_key_down(Key::Up)) {
    const bool show = right_top_toggle->getText() == "<";

    if (show) {
      right_top_toggle->setPosition(882, 0);
      right_top_toggle->setText(">");
      right_top_toggle->setTransparency(255);
    } else {
      right_top_toggle->setPosition(994, 0);
      right_top_toggle->setText("<");
      right_top_toggle->setTransparency(150);
    }

    btn_back->setVisible(show);
    btn_help->setVisible(show);
  }

  if (left_top_toggle->clicked()) {
    const bool show = left_top_toggle->getText() != "<";

    if (show) {
      left_top_toggle->setPosition(257, 60);
      left_top_toggle->setText("<");
      left_top_toggle->setTransparency(255);
    } else {
      left_top_toggle->setPosition(0, 60);
      left_top_toggle->setText(">");
      left_top_toggle->setTransparency(150);
    }

    chk_affects_char->setVisible(show);
    for (auto* button : explosive_buttons) {
      button->setVisible(show);
    }
  }
}

void Editor::placeTiles() {
  if (!asw::input::get_mouse_button(MouseButton::Left) || ui.isHovering()) {
    return;
  }

  const auto mouse = asw::input::get_mouse().position;
  if (mouse.x < 0 || mouse.y < 0 || mouse.x >= SCREEN_WIDTH ||
      mouse.y >= SCREEN_HEIGHT) {
    return;
  }

  // One object per cell
  for (auto type : {ObjectType::Dynamic, ObjectType::Static,
                    ObjectType::Character, ObjectType::Finish,
                    ObjectType::Explosive}) {
    if (boxAt(type, mouse.x, mouse.y)) {
      return;
    }
  }

  EditorBox box;
  box.type = tile_type;
  box.x = snap(mouse.x);
  box.y = snap(mouse.y);
  box.affect_character = chk_affects_char->getChecked();

  if (tile_type == ObjectType::Explosive) {
    box.orientation[0] = explosive_orientation;
  }

  boxes.push_back(box);
  modified = true;
  calculateOrientations();
}

void Editor::dragCollisionBox() {
  const auto mouse = asw::input::get_mouse().position;
  const asw::Vec2<float> cell(snap(mouse.x), snap(mouse.y));

  if (!is_dragging_box) {
    if (asw::input::get_mouse_button_down(MouseButton::Left) && !ui.isHovering()) {
      is_dragging_box = true;
      drag_start = cell;
      drag_end = cell;
    }
    return;
  }

  if (asw::input::get_mouse_button(MouseButton::Left)) {
    drag_end = cell;
    return;
  }

  // Released, covers every cell between the start and end
  is_dragging_box = false;

  EditorBox box;
  box.type = ObjectType::Collision;
  box.x = std::min(drag_start.x, drag_end.x);
  box.y = std::min(drag_start.y, drag_end.y);
  box.width = std::max(drag_start.x, drag_end.x) + CELL - box.x;
  box.height = std::max(drag_start.y, drag_end.y) + CELL - box.y;

  boxes.push_back(box);
  modified = true;
}

void Editor::removeTiles() {
  if (!asw::input::get_mouse_button(MouseButton::Right) || ui.isHovering()) {
    return;
  }

  const auto mouse = asw::input::get_mouse().position;
  const auto removed = std::erase_if(boxes, [&mouse](const EditorBox& box) {
    return mouse.x > box.x && mouse.x < box.x + box.width &&
           mouse.y > box.y && mouse.y < box.y + box.height;
  });

  if (removed > 0) {
    modified = true;
    calculateOrientations();
  }
}

void Editor::save() {
  if (boxes.empty()) {
    asw::dialog::error("Empty Map", "You can't save an empty map!");
    return;
  }

  if (!is_saved) {
    pending_file_action = FileAction::Save;
    asw::dialog::request_file(asw::dialog::FileMode::Save, Config::savePath(),
                             LEVEL_FILTERS);
    return;
  }

  if (saveMap(file_name)) {
    modified = false;
  } else {
    asw::dialog::error("Error!", "Error saving map to: " + file_name);
  }
}

void Editor::saveAs() {
  if (boxes.empty()) {
    asw::dialog::error("Empty Map", "You can't save an empty map!");
    return;
  }

  pending_file_action = FileAction::SaveAs;
  asw::dialog::request_file(asw::dialog::FileMode::Save, Config::savePath(),
                             LEVEL_FILTERS);
}

void Editor::load() {
  pending_file_action = FileAction::Load;
  asw::dialog::request_file(asw::dialog::FileMode::Open,
                           asw::assets::get_path("assets/data/"), LEVEL_FILTERS);
}

void Editor::handleFileChosen(FileAction action, const std::string& path) {
  if (action == FileAction::Load) {
    if (loadMap(path)) {
      file_name = path;
      is_saved = true;
      modified = false;
    } else {
      asw::dialog::error("Error!", "Error loading map from: " + path);
    }
    return;
  }

  // Save and save as
  std::filesystem::path save_path(path);
  if (!save_path.has_extension()) {
    save_path.replace_extension(".xml");
  }

  if (!saveMap(save_path.string())) {
    asw::dialog::error("Error!", "Error saving map to: " + save_path.string());
    return;
  }

  file_name = save_path.string();
  is_saved = true;
  modified = false;

  if (action == FileAction::SaveAs) {
    asw::dialog::info("Saved map", "We've saved a map to: " + file_name);
  }
}

void Editor::play() {
  if (boxes.empty()) {
    asw::dialog::info("Attempting to play an empty level",
                 "That wouldn't be very fun would it?");
    return;
  }

  if (!hasPlayer()) {
    asw::dialog::info("Missing player",
                 "You must place a player spawn to test the level.");
    return;
  }

  if (!saveMap(file_name)) {
    asw::dialog::error("Error!", "Error saving map to: " + file_name);
    return;
  }

  session.editing_level = true;
  session.editing_file = file_name;
  changeScene(ProgramState::Game);
}

void Editor::back() {
  if (modified && !asw::dialog::confirm("Main menu?",
                                   "Return to main menu? All unsaved changes "
                                   "will be lost.")) {
    return;
  }

  changeScene(ProgramState::Menu);
}

void Editor::calculateOrientations() {
  for (auto& box : boxes) {
    if (box.type != ObjectType::Static) {
      continue;
    }

    for (std::size_t corner = 0; corner < box.orientation.size(); corner++) {
      const float x = box.x + ((corner % 2 == 1) ? HALF_CELL : 0);
      const float y = box.y + ((corner >= 2) ? HALF_CELL : 0);

      const auto neighbour = [this, x, y](float dx, float dy) {
        return boxAt(ObjectType::Static, x + (dx * HALF_CELL),
                     y + (dy * HALF_CELL));
      };

      std::array<bool, TILE_OPTIONS> options{};
      options.fill(true);

      const auto remove = [&options](std::initializer_list<int> tiles) {
        for (int tile : tiles) {
          options[tile] = false;
        }
      };

      // Edges rule out the tiles that border them, or need them
      neighbour(0, -1) ? remove({0, 1, 2}) : remove({3, 4, 5});
      neighbour(1, 0) ? remove({2, 5, 8}) : remove({1, 4, 7});
      neighbour(0, 1) ? remove({6, 7, 8}) : remove({3, 4, 5});
      neighbour(-1, 0) ? remove({0, 3, 6}) : remove({1, 4, 7});

      // Corners pick between the full tile and the inner corner tiles
      neighbour(1, -1) ? remove({11}) : remove({4});
      neighbour(1, 1) ? remove({9}) : remove({4});
      neighbour(-1, 1) ? remove({10}) : remove({4});
      neighbour(-1, -1) ? remove({12}) : remove({4});

      const auto first = std::ranges::find(options, true);
      box.orientation[corner] =
          first != options.end()
              ? static_cast<int>(std::distance(options.begin(), first))
              : 0;
    }
  }
}

bool Editor::boxAt(ObjectType type, float x, float y) const {
  return std::ranges::any_of(boxes, [type, x, y](const EditorBox& box) {
    return box.type == type && box.x < x + 1 && x < box.x + CELL &&
           box.y < y + 1 && y < box.y + CELL;
  });
}

bool Editor::hasPlayer() const {
  return std::ranges::any_of(boxes, [](const EditorBox& box) {
    return box.type == ObjectType::Character;
  });
}

bool Editor::saveMap(const std::string& path) const {
  Level level;
  level.help = help_text;

  for (const auto& box : boxes) {
    level.objects.push_back(toLevelObject(box));
  }

  return level.save(path);
}

bool Editor::loadMap(const std::string& path) {
  const auto level = Level::load(path);
  if (!level) {
    return false;
  }

  boxes.clear();
  for (const auto& object : level->objects) {
    boxes.push_back(fromLevelObject(object));
  }

  help_text = level->help;
  return true;
}

void Editor::draw() {
  asw::draw::clear_color(asw::Color(200, 200, 255));

  if (grid_on) {
    for (float x = 0; x < SCREEN_WIDTH; x += CELL) {
      asw::draw::line(asw::Vec2<float>(x, 0), asw::Vec2<float>(x, SCREEN_HEIGHT),
                      BLACK);
    }

    for (float y = 0; y < SCREEN_HEIGHT; y += CELL) {
      asw::draw::line(asw::Vec2<float>(0, y), asw::Vec2<float>(SCREEN_WIDTH, y),
                      BLACK);
    }
  }

  for (const auto& box : boxes) {
    drawBox(box);
  }

  // Collision boxes see through, on top
  for (const auto& box : boxes) {
    if (box.type == ObjectType::Collision) {
      asw::draw::rect_fill(asw::Quad<float>(box.x, box.y, box.width, box.height),
                           COLLISION_FILL);
    }
  }

  if (is_dragging_box) {
    const float x = std::min(drag_start.x, drag_end.x);
    const float y = std::min(drag_start.y, drag_end.y);
    asw::draw::rect_fill(
        asw::Quad<float>(x, y, std::max(drag_start.x, drag_end.x) + CELL - x,
                         std::max(drag_start.y, drag_end.y) + CELL - y),
        COLLISION_FILL);
  }

  asw::draw::text(edit_font, std::string("Type: ") + typeName(tile_type),
                  asw::Vec2<float>(10, 30), BLACK);

  asw::draw::text(edit_font,
                  "File: " +
                      std::filesystem::path(file_name).filename().string() +
                      (modified ? " *" : ""),
                  asw::Vec2<float>(10, 10), BLACK);

  if (display_help) {
    asw::draw::sprite(help_menu, asw::Vec2<float>(110, 75));
  }

  ui.draw();
}

void Editor::drawBox(const EditorBox& box) const {
  switch (box.type) {
    case ObjectType::Static:
      for (std::size_t corner = 0; corner < box.orientation.size(); corner++) {
        const int tile = box.orientation[corner];
        if (tile < 0 || tile >= tile_sheet.get_frame_count()) {
          continue;
        }

        tile_sheet.draw_frame(
            tile, asw::Quad<float>(box.x + ((corner % 2 == 1) ? TILE_SIZE : 0),
                                   box.y + ((corner >= 2) ? TILE_SIZE : 0),
                                   TILE_SIZE, TILE_SIZE));
      }
      break;

    case ObjectType::Dynamic:
      asw::draw::sprite(box_green, asw::Vec2<float>(box.x, box.y));
      break;

    case ObjectType::Character:
      character_sheet.draw_frame(0,
                                 asw::Quad<float>(box.x, box.y, CELL, CELL * 2));
      break;

    case ObjectType::Finish:
      goat_sheet.draw_frame(0, asw::Quad<float>(box.x, box.y, CELL, CELL * 2));
      break;

    case ObjectType::Explosive: {
      asw::draw::rect_fill(
          asw::Quad<float>(box.x + 4, box.y + 4, CELL - 8, CELL - 8),
          box.affect_character ? asw::Color(255, 0, 0)
                               : asw::Color(0, 255, 0));

      const int orientation = box.orientation[0];
      if (orientation == 0) {
        asw::draw::sprite(box_repel, asw::Vec2<float>(box.x, box.y));
      } else {
        asw::draw::stretch_sprite_rotate_blit(box_repel_direction, asw::Quad<float>(0, 0, CELL, CELL),
                    asw::Quad<float>(box.x, box.y, CELL, CELL),
                    (std::numbers::pi_v<float> / 2.0F) *
                        static_cast<float>(orientation - 1));
      }
      break;
    }

    case ObjectType::Collision:
      break;
  }
}
