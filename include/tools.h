#ifndef TOOLS_H
#define TOOLS_H

#include <string>
#include <sstream>
#include <fstream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>


class tools{
    public:
        // Convert char to float
        static int char_to_float( std::string newChar);
        static void abort_on_error(std::string);

        static ALLEGRO_BITMAP * load_bitmap_ex( std::string file);

    protected:

    private:
};

#endif // TOOLS_H
