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

        std::string name;

        SDL_Surface* image;

    public:
        Character();

        int get_level();          
        int get_mobility();       
        int get_health();         
        int get_strength();       
        int get_range();          
        std::string get_name();   
        SDL_Surface* get_image(); 
};
