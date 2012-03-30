#pragma once

#include "SDL/SDL.h"
#include "Tile.h"
#include "Constants.h"

class Grid {
    public:
        static void load_file();
        static void draw_grid(SDL_Surface* surface);
        static Tile* get(int i, int j);

        static int get_current_player();

        static int distance(int i, int j, int x, int y);
        static void select_tiles(int i, int j, int range, bool show);

        static bool show_move_tiles  (int i, int j, SDL_Surface* surface, bool show);
        static bool show_attack_tiles(int i, int j, SDL_Surface* surface, bool show);

        static bool move  (int i, int j, int x, int y, SDL_Surface* surface);
        static bool attack(int i, int j, int x, int y, SDL_Surface* surface);
        static bool heal  (int i, int j, int x, int y, SDL_Surface* surface);

        static void new_turn();
};
