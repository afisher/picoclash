#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

class Tile {
    private:
        static SDL_Surface* image;

    public:
        SDL_Surface* get_image();
};

Tile::Tile() {
    static image = Util::load_image("sprites/grass.png");
}

SDL_Surface* Tile::get_image() { return image; }
