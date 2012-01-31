#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <string>

class Character {
    private:
        int level;
        int mobility;
        int health;
        int strength;
        int range;
        std::string name;
        SDL_Surface* image;

    public:
        int level();
        int mobility();
        int health();
        int strength();
        std::string name();
        SDL_Surface* image();
};
