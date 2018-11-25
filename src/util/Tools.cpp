#include "util/Tools.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>

#include <iostream>
#include <math.h>
#include <fstream>

#include "util/MouseListener.h"

// A function to streamline error reporting in file loading
void tools::abort_on_error(std::string message, std::string title) {
  al_show_native_message_box(nullptr, "Error", title.c_str(), message.c_str(), nullptr, ALLEGRO_MESSAGEBOX_ERROR);
  exit(-1);
}

// Load sample if exits, or throw error
ALLEGRO_SAMPLE *tools::load_sample_ex(std::string file) {
  // Log file
  log_message("Loading sound " + file, true);

  // Check if file exists
  std::ifstream f(file.c_str());
  if(!f.good())
    abort_on_error(std::string("Cannot find sample " + file + "\nYour file is gone and there's nothing I can do. Sorry."), "File Not Found");

  // Attempt to load
  ALLEGRO_SAMPLE *temp_sample = nullptr;
  if(!(temp_sample = al_load_sample(file.c_str())))
    abort_on_error(std::string("There was an error loading " + file + "\nOh no :("), "Loading Error");

  return temp_sample;
}

// Load bitmap if exits, or throw error
ALLEGRO_BITMAP * tools::load_bitmap_ex(std::string file) {
  // Log file
  log_message("Loading bitmap " + file, true);

  // Check if file exists
  std::ifstream f(file.c_str());
  if(!f.good())
    abort_on_error(std::string("Cannot find image " + file + "\nYour file is gone and there's nothing I can do. Sorry."), "File Not Found");

  // Attempt to load
  ALLEGRO_BITMAP *temp_image = nullptr;
  if(!(temp_image = al_load_bitmap(file.c_str())))
    abort_on_error(std::string("There was an error loading " + file + "... \nSorry..."), "Loading Error");

  return temp_image;
}

// Text offset x
int tools::get_text_offset_x(ALLEGRO_FONT *newFont, std::string newText){
  int text_width, text_height, text_offset_x, text_offset_y = 0;
  al_get_text_dimensions(newFont, newText.c_str(), &text_offset_x, &text_offset_y, &text_width, &text_height);
  return text_offset_x;
}

// Text offset y
int tools::get_text_offset_y(ALLEGRO_FONT *newFont, std::string newText){
  int text_width, text_height, text_offset_x, text_offset_y = 0;
  al_get_text_dimensions(newFont, newText.c_str(), &text_offset_x, &text_offset_y, &text_width, &text_height);
  return text_offset_y;
}

// Text offset width
int tools::get_text_width(ALLEGRO_FONT *newFont, std::string newText){
  int text_width, text_height, text_offset_x, text_offset_y = 0;
  al_get_text_dimensions(newFont, newText.c_str(), &text_offset_x, &text_offset_y, &text_width, &text_height);
  return text_width;
}

// Text offset height
int tools::get_text_height(ALLEGRO_FONT *newFont, std::string newText){
  int text_width, text_height, text_offset_x, text_offset_y = 0;
  al_get_text_dimensions(newFont, newText.c_str(), &text_offset_x, &text_offset_y, &text_width, &text_height);
  return text_height;
}

// Convert string to int
int tools::stringToInt(std::string newString) {
  int result;
  std::stringstream(newString) >> result;
  return result;
}

// Convert char to float
float tools::stringToFloat(std::string newChar) {
  float number = 0.0f;
  int sign = 1;
  // Parse string
  // Negative
  if (newChar[0] == '-'){
    sign = -1;
    newChar = newChar.substr(1, newChar.length());
  }
  // Location of decimal
  int decimal_loc = newChar.length();
  for (unsigned int i = 0; i < newChar.length(); i++){
    if (newChar[i] == '.'){
      decimal_loc = i;
      newChar = newChar.substr(0, i) + newChar.substr(i + 1, newChar.length() - i);
    }
  }
  // Numbers
  for (unsigned int i = 0; i < newChar.length(); i++){
    int tempNumber = int(newChar[i]) - 48;
    number += tempNumber * pow(10, (newChar.length()) - (i + 1));
  }
  return (number / float(pow(10, newChar.length() - decimal_loc))) * sign;
}


// Split string
std::vector<std::string> tools::split_string(const std::string& p_pcstStr, char delim ) {
  std::vector<std::string> tokens;
  std::stringstream mySstream(p_pcstStr);
  std::string temp;
  while(getline(mySstream, temp, delim))
    tokens.push_back(temp);
  return tokens;
}

// Random number
int tools::random_int(int min, int max){
  return (rand() % (max + 1 - min)) + min;
}

// Log message to console
void tools::log_message(std::string message, bool debug) {
  if (debug) {
    #ifdef Debug
      std::cout << message << "\n";
    #endif
  }
  else {
    std::cout << message << "\n";
  }
}

// Mouse over
bool tools::mouse_over(int x, int y, int width, int height) {
  return tools::collision(MouseListener::mouse_x, MouseListener::mouse_x,
                          x, x + width,
                          MouseListener::mouse_y, MouseListener::mouse_y,
                          y, y + height);
}

// Mouse released with reset
bool tools::mouse_clicked(int button, bool reset) {
  bool old_released = MouseListener::mouse_released & button;
  if (reset) {
    MouseListener::mouse_released &= 0 << button;
  }
  return old_released;
}
