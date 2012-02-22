#pragma once

#include "Character.h"

class Tile {
    private:

        SDL_Surface* image;
        Character* character;
        bool selected;

        void update_image();

    public:
        Tile();
        Tile(int type);
        void set_selected(bool s);
        bool get_selected();
        SDL_Surface* get_image();
        Character* get_character();

        static SDL_Surface* selected_image; 
        static SDL_Surface* default_image;  

};
