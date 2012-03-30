#pragma once

#include "Character.h"
//#include "Grid.h"

class Grid;

class Tile {
    private:
        SDL_Surface* image;
        Character* character;
        bool selected;

        Grid* grid;
        int x;
        int y;

        void update_image();

    public:
        Tile(Grid* g, int i, int j);
        Tile(Grid* g, int i, int j, int type);

        void set_selected(bool s);
        bool get_selected();
        SDL_Surface* get_image();
        Character* get_character();

        static SDL_Surface* selected_image; 
        static SDL_Surface* default_image;  

        void character_died();
};
