#pragma once

#include "SDL/SDL.h"
#include "Tile.h"
#include "Util.h"

class Grid {
    private:
        Tile* grid[Util::GRID_HEIGHT][Util::GRID_WIDTH];
        void load_file();
        int distance(int i, int j, int x, int y);
        void select_tiles(int i, int j, int range, bool show);

    public:
        Grid();
        void draw_grid(SDL_Surface* screen);
        Tile* get(int i, int j);

        bool show_move_tiles  (int i, int j, SDL_Surface* screen, bool show);
        bool show_attack_tiles(int i, int j, SDL_Surface* screen, bool show);

        bool move(int i, int j, int x, int y, SDL_Surface* screen);
        bool attack(int i, int j, int x, int y, SDL_Surface* screen);
};
