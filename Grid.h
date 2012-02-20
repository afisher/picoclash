#pragma once

#include "SDL/SDL.h"
#include "Tile.h"
#include "Util.h"

class Grid {
    private:
        Tile* grid[Util::GRID_HEIGHT][Util::GRID_WIDTH];
        void load_file();
        int distance(int i, int j, int x, int y);

    public:
        Grid();
        void draw_grid(SDL_Surface* screen);
        Tile* get(int i, int j);
        void show_move_tiles(int i, int j, SDL_Surface* screen);
        bool move(int i, int j, int x, int y, SDL_Surface* screen);
};
