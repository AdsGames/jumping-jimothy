#include "tools.h"


 void tools::abort_on_error( std::string message){
  al_show_native_message_box( nullptr, "Error", "Warning", message.c_str(), nullptr, ALLEGRO_MESSAGEBOX_YES_NO);
	 //set_window_title("Error!");
	 //if (screen != NULL){
	 //   set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
	 //}
	 //allegro_message("%s.\n %s\n", message.c_str());

	 exit(-1);
}

// Checks if file exists
 ALLEGRO_BITMAP * tools::load_bitmap_ex( std::string file){
  ALLEGRO_BITMAP *temp_image = nullptr;
  if( !(temp_image = al_load_bitmap( file.c_str())))
    abort_on_error( std::string("Cannot find image " + file + "\nYour file is gone and there's nothing I can do. Sorry."));

  return temp_image;
}
// A function to streamline error reporting in file loading

// Convert char to float
float tools::string_to_float( std::string newChar){
  float number = 0.0f;
  int sign = 1;
  // Parse string
  // Negative
  if( newChar[0] == '-'){
    sign = -1;
    newChar = newChar.substr(1, newChar.length());
  }
  // Location of decimal
  int decimal_loc = newChar.length();
  for( int i = 0; i < newChar.length(); i++){
    if( newChar[i] == '.'){
      decimal_loc = i;
      newChar = newChar.substr(0, i) + newChar.substr(i + 1, newChar.length() - i);
    }
  }
  // Numbers
  for( int i = 0; i < newChar.length(); i++){
    int tempNumber = int(newChar[i]) - 48;
    number += tempNumber * pow(10, (newChar.length()) - (i + 1));
  }
  return (number / float(pow(10, newChar.length() - decimal_loc))) * sign;
}

template <class T> T tools::clamp( T min_val, T max_val, T value) {
    if( value < min_val)
      return min_val;
    if( value > max_val)
      return max_val;
    return value;
}

// Function to check for collision
bool tools::collision(float xMin1, float xMax1, float xMin2, float xMax2, float yMin1, float yMax1, float yMin2, float yMax2){
  if (xMin1 < xMax2 && yMin1 < yMax2 && xMin2 < xMax1 && yMin2 < yMax1){
    return true;
  }
  return false;
}


