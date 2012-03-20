#pragma once

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <string>

class Util {
    private:
        static void scale(SDL_Surface* source, SDL_Surface* destination);
    public:
        static const int PLAYER_WARRIOR = 1;
        static const int PLAYER_ARCHER  = 2;
        static const int PLAYER_HEALER  = 3;
        static const int ENEMY_WARRIOR  = 4;
        static const int ENEMY_ARCHER   = 5;
        static const int ENEMY_HEALER   = 6;

        static const int GRID_WIDTH  = 30;
        static const int GRID_HEIGHT = 30;

        static const int SPRITE_SIZE = 16;

        static SDL_Surface* init_screen(int width, int height, int bpp);
        static void update_screen(SDL_Surface* source, SDL_Surface* destination);

        static SDL_Surface* load_image(std::string filename);
        static void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination);
};
