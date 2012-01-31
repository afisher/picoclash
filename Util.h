#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <string>

class Util {
    public:
        static SDL_Surface* initScreen(); 
        static SDL_Surface* load_image(std::string filename); 
        static void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination);
};
