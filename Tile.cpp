#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "Util.h"
#include "Constants.h"
#include "Tile.h"
#include "Warrior.h"
#include "Archer.h"
#include "Healer.h"

SDL_Surface* Tile::default_image  = NULL;
SDL_Surface* Tile::selected_image = NULL;

Tile::Tile() {
    character = NULL;

    set_selected(false);
}

Tile::Tile(int type) {
    switch(type) {
        case Constants::PLAYER_WARRIOR:
            character = new Warrior(1); break;
        case Constants::PLAYER_ARCHER:
            character = new Archer(1);  break;
        case Constants::PLAYER_HEALER:
            character = new Healer(1);  break;
        case Constants::ENEMY_WARRIOR:
            character = new Warrior(2); break;
        case Constants::ENEMY_ARCHER:
            character = new Archer(2);  break;
        case Constants::ENEMY_HEALER:
            character = new Healer(2);  break;
        default:
            character = NULL;
    }

    set_selected(false);
}

void Tile::set_selected(bool s) {
    selected = s;
    update_image();
}

bool Tile::get_selected() {
    return selected;
}

void Tile::update_image() {
    if (selected) {
        image = selected_image;
    } else {
        image = default_image;
    }
}

SDL_Surface* Tile::get_image()     { return image;     }
Character*   Tile::get_character() { return character; }

void Tile::character_died() { character = NULL; }
