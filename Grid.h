#pragma once

#include "SDL/SDL.h"
#include "Tile.h"
#include "Util.h"
#include "Constants.h"

class Grid {
    private:
        Tile* grid[Constants::GRID_HEIGHT][Constants::GRID_WIDTH];
        int current_player;
        void load_file();
        int distance(int i, int j, int x, int y);
        void select_tiles(int i, int j, int range, bool show);

    public:
        Grid();
        void draw_grid(SDL_Surface* surface);
        Tile* get(int i, int j);

        int get_current_player();

        bool show_move_tiles  (int i, int j, SDL_Surface* surface, bool show);
        bool show_attack_tiles(int i, int j, SDL_Surface* surface, bool show);

        bool move  (int i, int j, int x, int y, SDL_Surface* surface);
        bool attack(int i, int j, int x, int y, SDL_Surface* surface);
        bool heal  (int i, int j, int x, int y, SDL_Surface* surface);

        void new_turn();
};
