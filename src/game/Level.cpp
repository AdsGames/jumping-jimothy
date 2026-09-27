#include "Level.h"

#include <asw/asw.h>
#include <format>
#include <pugixml.hpp>
#include <sstream>
#include <utility>

namespace {
constexpr std::array<std::pair<ObjectType, const char*>, 6> TYPE_NAMES{{
    {ObjectType::Dynamic, "Dynamic"},
    {ObjectType::Static, "Static"},
    {ObjectType::Character, "Character"},
    {ObjectType::Finish, "Finish"},
    {ObjectType::Collision, "Collision"},
    {ObjectType::Explosive, "Explosive"},
}};

std::optional<ObjectType> typeFromName(const std::string& name) {
  for (const auto& [type, type_name] : TYPE_NAMES) {
    if (name == type_name) {
      return type;
    }
  }

  return std::nullopt;
}

const char* typeName(ObjectType type) {
  for (const auto& [t, name] : TYPE_NAMES) {
    if (t == type) {
      return name;
    }
  }

  return "Static";
}

// Orientation is "a b c d" for static boxes and "a" for explosives. A single
// value is used for all four corners.
std::array<int, 4> parseOrientation(const char* text) {
  std::array<int, 4> result{};
  std::istringstream stream(text);
  std::vector<int> values;

  int value = 0;
  while (values.size() < result.size() && stream >> value) {
    values.push_back(value);
  }

  for (std::size_t i = 0; i < result.size() && !values.empty(); i++) {
    result[i] = values.size() == result.size() ? values[i] : values[0];
  }

  return result;
}

// Shortest text that reads back as the same float
std::string formatFloat(float value) {
  return std::format("{}", value);
}

void addChild(pugi::xml_node& node, const char* name, const std::string& text) {
  node.append_child(name).text().set(text.c_str());
}
}  // namespace

std::vector<std::string> Level::helpLines() const {
  std::vector<std::string> lines;
  std::istringstream stream(help);

  std::string line;
  while (std::getline(stream, line, '%')) {
    lines.push_back(line);
  }

  return lines;
}

std::optional<Level> Level::load(const std::string& path) {
  pugi::xml_document doc;
  if (const auto result = doc.load_file(path.c_str()); !result) {
    asw::log::warn("Could not load level {}: {}", path, result.description());
    return std::nullopt;
  }

  const auto root = doc.child("data");
  if (!root) {
    asw::log::warn("Level {} has no data node", path);
    return std::nullopt;
  }

  Level level;

  for (const auto& node : root.children("Object")) {
    const auto type = typeFromName(node.attribute("type").as_string("Static"));
    if (!type) {
      asw::log::warn("Skipping object of unknown type {}",
                     node.attribute("type").as_string());
      continue;
    }

    LevelObject object;
    object.type = *type;
    object.x = node.child("x").text().as_float();
    object.y = node.child("y").text().as_float();
    object.width = node.child("width").text().as_float();
    object.height = node.child("height").text().as_float();
    object.orientation =
        parseOrientation(node.child("orientation").text().as_string());
    object.affect_character = node.child("affect_character").text().as_bool();
    level.objects.push_back(object);
  }

  level.help = root.child("Help").text().as_string();

  return level;
}

bool Level::save(const std::string& path) const {
  pugi::xml_document doc;
  auto decl = doc.append_child(pugi::node_declaration);
  decl.append_attribute("version") = "1.0";
  decl.append_attribute("encoding") = "utf-8";

  auto root = doc.append_child("data");

  for (const auto& object : objects) {
    auto node = root.append_child("Object");
    node.append_attribute("type") = typeName(object.type);
    addChild(node, "x", formatFloat(object.x));
    addChild(node, "y", formatFloat(object.y));

    switch (object.type) {
      case ObjectType::Static:
        addChild(node, "orientation",
                 std::format("{} {} {} {}", object.orientation[0],
                             object.orientation[1], object.orientation[2],
                             object.orientation[3]));
        break;
      case ObjectType::Collision:
        addChild(node, "width", formatFloat(object.width));
        addChild(node, "height", formatFloat(object.height));
        break;
      case ObjectType::Explosive:
        addChild(node, "affect_character",
                 object.affect_character ? "true" : "false");
        addChild(node, "orientation", std::to_string(object.orientation[0]));
        break;
      default:
        break;
    }
  }

  if (!help.empty()) {
    addChild(root, "Help", help);
  }

  return doc.save_file(path.c_str());
}
