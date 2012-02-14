#pragma once

#include "SDL/SDL.h"
#include "Tile.h"
#include "Util.h"

class Grid {
    private:
        Tile* grid[Util::GRID_HEIGHT][Util::GRID_WIDTH];
        void load_file();

    public:
        Grid();
        void draw_grid(SDL_Surface* screen);
        Tile* get(int i, int j);
};
