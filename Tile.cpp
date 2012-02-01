#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "Tile.h"
#include "Util.h"

Tile::Tile() {
    image = Util::load_image("sprites/grass.png");
}

SDL_Surface* Tile::get_image() { return image; }
