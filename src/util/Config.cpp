#include "Config.h"

#include <asw/asw.h>
#include <charconv>
#include <map>
#include <pugixml.hpp>

namespace {
constexpr auto DEFAULTS_FILE = "assets/data/config.xml";
constexpr auto CONFIG_FILE = "config.xml";

std::map<std::string, std::string, std::less<>> values;

// Each entry is stored as <entry key="value"/>
void readFile(const std::string& path) {
  pugi::xml_document doc;
  if (!doc.load_file(path.c_str())) {
    return;
  }

  for (const auto& entry : doc.child("data").children("entry")) {
    const auto attribute = entry.first_attribute();
    if (attribute) {
      values[attribute.name()] = attribute.value();
    }
  }
}
}  // namespace

std::string Config::savePath() {
  return asw::assets::get_save_path("adsgames", "jumping-jimothy");
}

void Config::load() {
  values.clear();
  readFile(asw::assets::get_path(DEFAULTS_FILE));
  readFile(savePath() + CONFIG_FILE);
}

void Config::save() {
  pugi::xml_document doc;
  auto data = doc.append_child("data");

  for (const auto& [key, value] : values) {
    data.append_child("entry").append_attribute(key.c_str()).set_value(
        value.c_str());
  }

  const auto path = savePath() + CONFIG_FILE;
  if (!doc.save_file(path.c_str())) {
    asw::log::warn("Could not save config to {}", path);
  }
}

std::string Config::getString(const std::string& key) {
  const auto it = values.find(key);
  return it != values.end() ? it->second : "";
}

int Config::getInt(const std::string& key) {
  const auto value = getString(key);
  int result = 0;
  std::from_chars(value.data(), value.data() + value.size(), result);
  return result;
}

bool Config::getBool(const std::string& key) {
  return getInt(key) != 0;
}

void Config::setString(const std::string& key, const std::string& value) {
  values[key] = value;
}

void Config::setInt(const std::string& key, int value) {
  setString(key, std::to_string(value));
}

void Config::setBool(const std::string& key, bool value) {
  setInt(key, value ? 1 : 0);
}
