/**
 * Level
 * Allan Legemaate
 * Level file data, shared by the game and the editor
 * 27/09/2026
 **/

#pragma once

#include <array>
#include <optional>
#include <string>
#include <vector>

enum class ObjectType { Dynamic, Static, Character, Finish, Collision, Explosive };

struct LevelObject {
  ObjectType type{ObjectType::Static};

  // Centre in metres, y is up
  float x{0};
  float y{0};

  // Size in metres, collision boxes only
  float width{0};
  float height{0};

  // Tiles of the four corners of a static box, or the direction of an
  // explosive in the first entry (0 is all directions, 1-4 is up, right,
  // down, left)
  std::array<int, 4> orientation{};

  // Explosives only
  bool affect_character{false};
};

struct Level {
  std::vector<LevelObject> objects;

  // Help text shown in the level, lines are split with '%'
  std::string help;

  // Help text split into lines
  std::vector<std::string> helpLines() const;

  static std::optional<Level> load(const std::string& path);
  bool save(const std::string& path) const;
};
