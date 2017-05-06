#ifndef TOOLS_H
#define TOOLS_H

#include <string>
#include <math.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>


class tools{
    public:
        // Convert char to float
        static float string_to_float( std::string newChar);
        static void abort_on_error(std::string);

        template <class T> static T clamp( T min_val, T max_val, T value);

        static ALLEGRO_BITMAP * load_bitmap_ex( std::string file);

    protected:

    private:
};

#endif // TOOLS_H
