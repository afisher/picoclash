#pragma once

#include "Character.h"

class Tile {
    private:
        SDL_Surface* image;
        Character* character;

    public:
        Tile();
        Tile(int type);
        SDL_Surface* get_image();
        Character* get_character();
};
