/**
 * TOOLS
 * Allan Legemaate
 * 05/05/2017
**/
#ifndef TOOLS_H
#define TOOLS_H

#include <string>
#include <math.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>

#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>



class tools{



    public:

        // Convert char to float
        static float string_to_float( std::string newChar);
        static void abort_on_error( std::string message, std::string title);
        static std::string convertIntToString( int number);
        static int convertStringToInt(std::string);

        static ALLEGRO_BITMAP * load_bitmap_ex( std::string file);
        static ALLEGRO_SAMPLE * load_sample_ex( std::string file);


        // Clamp values
        template <class T> static T clamp( T min_val, T max_val, T value) {
            if( value < min_val)
              return min_val;
            if( value > max_val)
              return max_val;
            return value;
        }

        // Function to check for collision
        template <class T> static bool collision( T xMin1, T xMax1, T xMin2, T xMax2, T yMin1, T yMax1, T yMin2, T yMax2){
          if (xMin1 < xMax2 && yMin1 < yMax2 && xMin2 < xMax1 && yMin2 < yMax1){
            return true;
          }
          return false;
        }

        // Convert
        template <class T> static std::string toString( const T& value){
          std::stringstream ss;
          ss << value;
          return ss.str();
        }

        // Split string
        static std::vector<std::string> split_string( const std::string& p_pcstStr, char delim )  {
          std::vector<std::string> tokens;
          std::stringstream mySstream( p_pcstStr );
          std::string temp;
          while( getline( mySstream, temp, delim ))
            tokens.push_back( temp );
          return tokens;
        }

        // Random number
        static int random_int( int min, int max){
          return (rand() % (max + 1 - min)) + min;
        }

    protected:

    private:
};

#endif // TOOLS_H
