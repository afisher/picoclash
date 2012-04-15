#include "Header.h"

using namespace std;

SDL_Surface* RockTile::default_image  = NULL;
SDL_Surface* RockTile::alt_image      = NULL;
SDL_Surface* RockTile::selected_image = NULL;
SDL_Surface* RockTile::move_image     = NULL;
SDL_Surface* RockTile::attack_image   = NULL;

RockTile::RockTile(int i, int j) : Tile(i, j) {
    update_image();
    update_overlay_image();
}

void RockTile::update_overlay_image() {
    if (selected) {
        overlay_image = selected_image;
    } else if (move_on) {
        overlay_image = move_image;
    } else if (attack_on) {
        overlay_image = attack_image;
    } else {
        overlay_image = NULL;
    }
}

void RockTile::update_image() {
    if (use_alt) {
        image = alt_image;
    } else {
        image = default_image;
    }
}

bool RockTile::is_standable() { return false; }

SDL_Surface* RockTile::get_image()         { return image;     }
SDL_Surface* RockTile::get_overlay_image() { return overlay_image; }
