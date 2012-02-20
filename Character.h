#pragma once

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <string>

class Character {
    protected:
        int level;
        int mobility;
        int health;
        int strength;
        int range;
        int increment;

        std::string name;

        SDL_Surface* image;

    public:
        Character();

        int get_level();          
        int get_mobility();       
        int get_health();         
        int get_strength();       
        int get_range();          

        int get_max_health();

        std::string get_name();   
        virtual SDL_Surface* get_image(); 
};
