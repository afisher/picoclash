#pragma once

#include "Character.h"
#include <vector>

class Tile {
    private:
        SDL_Surface* image;
        Character* character;
        bool selected;

        int x;
        int y;

        void update_image();

    public:
        Tile(int i, int j);
        Tile(int i, int j, int type);

        void set_selected(bool s);
        void set_character(Character* c);

        bool         get_selected();
        SDL_Surface* get_image();
        Character*   get_character();

        int get_x();
        int get_y();

        static SDL_Surface* selected_image; 
        static SDL_Surface* default_image;  

        void character_died();

        void move_character(std::vector<Tile*> move_tiles, SDL_Surface* surface);
        bool make_character_attack(std::vector<Tile*> attack_tiles, SDL_Surface* surface);
};
