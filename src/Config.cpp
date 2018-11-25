#include "Config.h"

#include <fstream>
#include <vector>

#include "rapidxml.hpp"
#include "rapidxml_print.hpp"

#include "Tools.h"

bool Config::music_enabled = true;
bool Config::sfx_enabled = true;
bool Config::draw_cursor = false;
int Config::graphics_mode = 0;

int Config::level_to_start = 1;

bool Config::joystick_mode = false;
std::string Config::joystick_data = "";

// Load options from xml
void Config::read_data(std::string path) {
  // Create XML doc
  rapidxml::xml_document<> doc;

  // Load XML from file
  std::ifstream theFile(path.c_str());
  std::vector<char> xml_buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());

  // Push EOF
  xml_buffer.push_back('\0');

  // Parse the buffer using the xml file parsing library into doc
  doc.parse<0>(&xml_buffer[0]);

  // Find our root node
  rapidxml::xml_node<> * root_node;
  root_node = doc.first_node("data");

  // Iterate over the nodes
  for (rapidxml::xml_node<> * object_node = root_node -> first_node("value"); object_node; object_node = object_node -> next_sibling()){
    // SFX
    if(object_node -> first_attribute("sfx") != nullptr){
      std::string result = object_node -> first_attribute("sfx") -> value();
      sfx_enabled = result == "enabled";
    }
    // Music
    if(object_node -> first_attribute("music") != nullptr){
      std::string result = object_node -> first_attribute("music") -> value();
      music_enabled = result == "enabled";
    }
    // Graphics Mode
    if(object_node -> first_attribute("graphics_mode") != nullptr){
      std::string result = object_node -> first_attribute("graphics_mode") -> value();
      graphics_mode = tools::stringToInt(result);
    }
  }
}

void Config::write_data(std::string path){
  // Create XML doc
  rapidxml::xml_document<> doc;

  // Push encoding and version data
  rapidxml::xml_node<>* decl = doc.allocate_node(rapidxml::node_declaration);
  decl -> append_attribute(doc.allocate_attribute("version", "1.0"));
  decl -> append_attribute(doc.allocate_attribute("encoding", "utf-8"));
  doc.append_node(decl);

  // Create root node
  rapidxml::xml_node<>* root_node = doc.allocate_node( rapidxml::node_element, "data");
  doc.append_node(root_node);

  // SFX
  char *node_name = doc.allocate_string("value");
  rapidxml::xml_node<>* object_node = doc.allocate_node( rapidxml::node_element, node_name);

  if(sfx_enabled)
    object_node -> append_attribute( doc.allocate_attribute("sfx", doc.allocate_string("enabled")));
  else
    object_node -> append_attribute( doc.allocate_attribute("sfx", doc.allocate_string("disabled")));

  root_node -> append_node( object_node);

  // Music
  object_node = doc.allocate_node( rapidxml::node_element, node_name);

  if(music_enabled)
    object_node -> append_attribute( doc.allocate_attribute("music", doc.allocate_string("enabled")));
  else
    object_node -> append_attribute( doc.allocate_attribute("music", doc.allocate_string("disabled")));

  root_node -> append_node( object_node);

  // Graphics
  object_node = doc.allocate_node( rapidxml::node_element, node_name);
  object_node -> append_attribute( doc.allocate_attribute("graphics_mode", doc.allocate_string(tools::toString(graphics_mode).c_str())));
  root_node -> append_node( object_node);

  // Save to file
  std::ofstream file_stored(path);
  file_stored << doc;
  file_stored.close();
  doc.clear();
}
