#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

class Healer: public Character {
    private:
        int magic;
        void set_values(int lvl);
};
