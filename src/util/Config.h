/**
 * Config
 * Allan Legemaate
 * Global configuration. Defaults come from the assets folder,
 *   changes are saved to the user's save folder.
 * 22/11/2018
 **/

#pragma once

#include <string>

namespace Config {

// Load defaults, then the user's saved values over them
void load();

// Write all values to the user's save folder
void save();

std::string getString(const std::string& key);
int getInt(const std::string& key);
bool getBool(const std::string& key);

void setString(const std::string& key, const std::string& value);
void setInt(const std::string& key, int value);
void setBool(const std::string& key, bool value);

// Folder for user files such as the config and editor levels
std::string savePath();

}  // namespace Config
