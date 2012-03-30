#pragma once

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <string>

class Tile;

class Character {
    protected:
        int player;
        int level;
        int mobility;
        int health;
        int strength;
        int range;
        int increment;

        bool moved_this_turn;
        bool attacked_this_turn;

        std::string name;

        SDL_Surface* image;
        SDL_Surface* grey_image;

        Tile* tile;

    public:
        Character();
        ~Character();

        int get_player();
        int get_level();
        int get_mobility();
        int get_health();
        int get_strength();
        int get_range();

        int get_max_health();

        void take_damage(int d);
        void gain_health(int h);

        bool get_moved_this_turn();
        bool get_attacked_this_turn();
        virtual bool get_healed_this_turn();

        virtual bool can_heal();

        std::string get_name();
        virtual SDL_Surface* get_image();

        void set_moved_this_turn(bool moved);
        void set_attacked_this_turn(bool attacked);
        void set_tile(Tile* t);
};
