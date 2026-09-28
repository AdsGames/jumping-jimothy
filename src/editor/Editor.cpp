#include "Editor.h"

#include <algorithm>
#include <cmath>
#include <filesystem>
#include <initializer_list>
#include <numbers>
#include <utility>

#include "../Globals.h"
#include "../ui/GameUi.h"
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
const asw::Color GREY(200, 200, 200);
const asw::Color SELECTED(0, 150, 0);
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

constexpr float UI_PADDING = 10;

// See through toggles, for when their group is hidden
constexpr uint8_t FADED_ALPHA = 150;

float rightOf(const asw::ui::Widget& widget) {
  return widget.transform.position.x + widget.transform.size.x;
}

// Move a toggle, set its arrow and fade it when its group is hidden
void setToggle(asw::ui::Button& toggle, float x, const std::string& text,
               bool faded) {
  toggle.transform.position.x = x;
  toggle.text = text;

  if (faded) {
    toggle.style = GameUi::buttonStyle(GREY, BLACK, FADED_ALPHA);
  } else {
    toggle.style.reset();
  }
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
  ui.root.clear_children();
  type_buttons.clear();
  file_buttons.clear();
  boxes.clear();
  asw::scene::Scene<ProgramState>::cleanup();
}

void Editor::changeScene(ProgramState state) {
  leaving = true;
  manager.set_next_scene(state);
}

void Editor::createUI() {
  ui = asw::ui::Root();
  GameUi::setup(ui, edit_font);
  ui.on_back = [this] { back(); };

  // Left and right are shortcuts here, not navigation
  ui.ctx.navigation.left = GameUi::NO_ACTION;
  ui.ctx.navigation.right = GameUi::NO_ACTION;

  // Bottom left, object types
  type_buttons.clear();
  float x = 0;
  for (const auto& [text, type] : std::initializer_list<
           std::pair<const char*, ObjectType>>{
           {"Dynamic", ObjectType::Dynamic},
           {"Static", ObjectType::Static},
           {"Player", ObjectType::Character},
           {"Goat", ObjectType::Finish},
           {"Collision", ObjectType::Collision},
           {"Explosive", ObjectType::Explosive},
       }) {
    auto& button = GameUi::addButton(ui, x, BAR_Y, text);
    button.on_click = [this, type] { setTileType(type); };
    type_buttons.push_back(&button);
    x = rightOf(button);
  }

  left_bottom_toggle = &GameUi::addButton(ui, x, BAR_Y, "<");
  left_bottom_toggle->on_click = [this] { toggleLeftBottom(); };

  // Bottom right, file actions
  right_bottom_toggle = &GameUi::addButton(ui, 566, BAR_Y, ">");
  right_bottom_toggle->on_click = [this] { toggleRightBottom(); };

  file_buttons.clear();
  x = rightOf(*right_bottom_toggle);
  for (const auto& [text, action] : std::initializer_list<
           std::pair<const char*, void (Editor::*)()>>{
           {"Undo", &Editor::undo},
           {"Clear", &Editor::clear},
           {"Save", &Editor::save},
           {"Save as", &Editor::saveAs},
           {"Load", &Editor::load},
           {"Grid", nullptr},
           {"Play", &Editor::play},
       }) {
    auto& button = GameUi::addButton(ui, x, BAR_Y, text);
    if (action != nullptr) {
      button.on_click = [this, action] { (this->*action)(); };
    } else {
      button.on_click = [this] { grid_on = !grid_on; };
    }
    file_buttons.push_back(&button);
    x = rightOf(button);
  }

  // Top right
  right_top_toggle = &GameUi::addButton(ui, 882, 0, ">");
  right_top_toggle->on_click = [this] { toggleRightTop(); };

  btn_help = &GameUi::addButton(ui, 911, 0, "Help");
  btn_help->on_click = [this] { display_help = !display_help; };

  btn_back = &GameUi::addButton(ui, rightOf(*btn_help), 0, "Back");
  btn_back->on_click = [this] { back(); };

  // Top left, explosive settings. The box sits after the text.
  const std::string affects_text = "Block affects character";
  const auto text_size = GameUi::fitText(edit_font, affects_text, UI_PADDING);
  const float box_size = text_size.y - (UI_PADDING * 2);

  chk_affects_char = &ui.root.add_child<asw::ui::Checkbox>();
  chk_affects_char->text = affects_text;
  chk_affects_char->padding = UI_PADDING;
  chk_affects_char->transform = asw::Quad<float>(
      0, 60, text_size.x + ui.ctx.theme.gap + box_size, text_size.y);

  left_top_toggle =
      &GameUi::addButton(ui, rightOf(*chk_affects_char), 60, "<");
  left_top_toggle->on_click = [this] { toggleLeftTop(); };

  for (std::size_t i = 0; i < explosive_buttons.size(); i++) {
    auto& button = i == 0
                       ? ui.root.add_child<ExplosiveButton>(box_repel, 0.0F)
                       : ui.root.add_child<ExplosiveButton>(
                             box_repel_direction,
                             (std::numbers::pi_v<float> / 2.0F) *
                                 static_cast<float>(i - 1));

    button.transform.position = asw::Vec2<float>(EXPLOSIVE_BUTTON_X[i], 100);
    button.on_click = [this, i] {
      setExplosiveOrientation(static_cast<int>(i));
    };
    explosive_buttons[i] = &button;
  }

  setExplosiveOrientation(explosive_orientation);
  setExplosiveUIVisible(false);
}

void Editor::setExplosiveUIVisible(bool visible) {
  chk_affects_char->visible = visible;
  left_top_toggle->visible = visible;

  for (auto* button : explosive_buttons) {
    button->visible = visible;
  }
}

void Editor::setTileType(ObjectType type) {
  tile_type = type;
  setExplosiveUIVisible(type == ObjectType::Explosive);

  if (type == ObjectType::Explosive) {
    setToggle(*left_top_toggle, rightOf(*chk_affects_char), "<", false);
  }
}

void Editor::setExplosiveOrientation(int orientation) {
  explosive_orientation = orientation;

  for (std::size_t i = 0; i < explosive_buttons.size(); i++) {
    auto& style = explosive_buttons[i]->style;
    if (static_cast<int>(i) == orientation) {
      style = GameUi::buttonStyle(SELECTED, BLACK);
    } else {
      style.reset();
    }
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

  ui_used = ui.update();
  if (leaving || pending_file_action != FileAction::None) {
    return;
  }

  handleShortcuts();
  if (leaving || pending_file_action != FileAction::None) {
    return;
  }

  if (tile_type == ObjectType::Collision) {
    dragCollisionBox();
  } else {
    placeTiles();
  }

  removeTiles();
}

void Editor::handleShortcuts() {
  using asw::input::get_key_down;

  // Object types
  if (get_key_down(Key::Q)) {
    setTileType(ObjectType::Dynamic);
  }

  if (get_key_down(Key::W)) {
    setTileType(ObjectType::Static);
  }

  if (get_key_down(Key::E)) {
    setTileType(ObjectType::Character);
  }

  if (get_key_down(Key::R)) {
    setTileType(ObjectType::Finish);
  }

  if (get_key_down(Key::T)) {
    setTileType(ObjectType::Collision);
  }

  if (get_key_down(Key::Y)) {
    setTileType(ObjectType::Explosive);
  }

  if (get_key_down(Key::H)) {
    display_help = !display_help;
  }

  if (get_key_down(Key::G)) {
    grid_on = !grid_on;
  }

  if (get_key_down(Key::Z)) {
    undo();
  }

  if (get_key_down(Key::C)) {
    clear();
  }

  if (get_key_down(Key::Left)) {
    toggleLeftBottom();
  }

  if (get_key_down(Key::Right)) {
    toggleRightBottom();
  }

  if (get_key_down(Key::Up)) {
    toggleRightTop();
  }

  // Escape is the UI back, see createUI
  if (get_key_down(Key::S)) {
    save();
  } else if (get_key_down(Key::D)) {
    saveAs();
  } else if (get_key_down(Key::A)) {
    load();
  } else if (get_key_down(Key::F)) {
    play();
  } else if (get_key_down(Key::V)) {
    back();
  }
}

void Editor::undo() {
  if (boxes.empty()) {
    return;
  }

  boxes.pop_back();
  modified = true;
  calculateOrientations();
}

void Editor::clear() {
  if (asw::dialog::confirm("Clear?",
                           "Clear the map? There is no recovering this "
                           "masterpiece.")) {
    boxes.clear();
    modified = true;
  }
}

void Editor::toggleLeftBottom() {
  const bool show = left_bottom_toggle->text != "<";
  setToggle(*left_bottom_toggle, show ? 489 : 0, show ? "<" : ">", !show);

  for (auto* button : type_buttons) {
    button->visible = show;
  }
}

void Editor::toggleRightBottom() {
  const bool show = right_bottom_toggle->text == "<";
  setToggle(*right_bottom_toggle, show ? 566 : 994, show ? ">" : "<", !show);

  for (auto* button : file_buttons) {
    button->visible = show;
  }
}

void Editor::toggleRightTop() {
  const bool show = right_top_toggle->text == "<";
  setToggle(*right_top_toggle, show ? 882 : 994, show ? ">" : "<", !show);

  btn_back->visible = show;
  btn_help->visible = show;
}

void Editor::toggleLeftTop() {
  const bool show = left_top_toggle->text != "<";
  setToggle(*left_top_toggle, show ? rightOf(*chk_affects_char) : 0,
            show ? "<" : ">", !show);

  chk_affects_char->visible = show;
  for (auto* button : explosive_buttons) {
    button->visible = show;
  }
}

void Editor::placeTiles() {
  if (!asw::input::get_mouse_button(MouseButton::Left) || ui_used) {
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
  box.affect_character = chk_affects_char->checked;

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
    if (asw::input::get_mouse_button_down(MouseButton::Left) && !ui_used) {
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
  if (!asw::input::get_mouse_button(MouseButton::Right) || ui_used) {
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
