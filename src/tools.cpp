#include "tools.h"

std::string tools::convertIntToString( int number){
  std::stringstream ss;
  ss << number;
  return ss.str();
}

// Convert string to int
int tools::convertStringToInt( std::string newString){
  int result;
  std::stringstream(newString) >> result;
  return result;
}


// A function to streamline error reporting in file loading
void tools::abort_on_error( std::string message, std::string title){
  al_show_native_message_box( nullptr, "Error", title.c_str(), message.c_str(), nullptr, ALLEGRO_MESSAGEBOX_ERROR);
  exit(-1);
}

int tools::get_text_offset_x(ALLEGRO_FONT *newFont, std::string newText){
  int text_width=0;
  int text_height=0;
  int text_offset_x;
  int text_offset_y;

  al_get_text_dimensions(newFont,newText.c_str(),&text_offset_x,&text_offset_y,&text_width,&text_height);

  return text_offset_x;

}

int tools::get_text_offset_y(ALLEGRO_FONT *newFont, std::string newText){
  int text_width=0;
  int text_height=0;
  int text_offset_x;
  int text_offset_y;

  al_get_text_dimensions(newFont,newText.c_str(),&text_offset_x,&text_offset_y,&text_width,&text_height);

  return text_offset_y;

}

int tools::get_text_width(ALLEGRO_FONT *newFont, std::string newText){
  int text_width=0;
  int text_height=0;
  int text_offset_x;
  int text_offset_y;

  al_get_text_dimensions(newFont,newText.c_str(),&text_offset_x,&text_offset_y,&text_width,&text_height);

  return text_width;

}

int tools::get_text_height(ALLEGRO_FONT *newFont, std::string newText){
  int text_width=0;
  int text_height=0;
  int text_offset_x;
  int text_offset_y;

  al_get_text_dimensions(newFont,newText.c_str(),&text_offset_x,&text_offset_y,&text_width,&text_height);

  return text_height;

}



// Load sample if exits, or throw error
ALLEGRO_SAMPLE *tools::load_sample_ex( std::string file){

  std::cout<<"Loading sound "<<file<<".";

  // Check if file exists
  std::ifstream f( file.c_str());
  if( !f.good())
    abort_on_error( std::string("Cannot find sample " + file + "\nYour file is gone and there's nothing I can do. Sorry."), "File Not Found");

  // Attempt to load
  ALLEGRO_SAMPLE *temp_sample = nullptr;
  if( !(temp_sample = al_load_sample( file.c_str())))
    abort_on_error( std::string("There was an error loading " + file + "\nOh no :("), "Loading Error");

  std::cout<<" Success.\n";

  return temp_sample;
}

// Load bitmap if exits, or throw error
ALLEGRO_BITMAP * tools::load_bitmap_ex( std::string file){

  std::cout<<"Loading bitmap "<<file<<".";

  // Check if file exists
  std::ifstream f( file.c_str());
  if( !f.good())
    abort_on_error( std::string("Cannot find image " + file + "\nYour file is gone and there's nothing I can do. Sorry."), "File Not Found");

  // Attempt to load
  ALLEGRO_BITMAP *temp_image = nullptr;
  if( !(temp_image = al_load_bitmap( file.c_str())))
    abort_on_error( std::string("There was an error loading " + file + "... \nSorry..."), "Loading Error");

  std::cout<<" Success.\n";


  return temp_image;
}

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
  for( unsigned int i = 0; i < newChar.length(); i++){
    if( newChar[i] == '.'){
      decimal_loc = i;
      newChar = newChar.substr(0, i) + newChar.substr(i + 1, newChar.length() - i);
    }
  }
  // Numbers
  for( unsigned int i = 0; i < newChar.length(); i++){
    int tempNumber = int(newChar[i]) - 48;
    number += tempNumber * pow(10, (newChar.length()) - (i + 1));
  }
  return (number / float(pow(10, newChar.length() - decimal_loc))) * sign;
}
