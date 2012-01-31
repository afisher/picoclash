#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "Util.h"
#include "Tile.h"

Tile::Tile() {
    image = Util::load_image("sprites/grass.png");
}

SDL_Surface* Tile::get_image() { return image; }
