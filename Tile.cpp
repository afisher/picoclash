#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "Util.h"
#include "Constants.h"
#include "Tile.h"
#include "Warrior.h"
#include "Archer.h"
#include "Healer.h"
#include "Grid.h"
#include "Character.h"
#include <vector>

using namespace std;

SDL_Surface* Tile::default_image  = NULL;
SDL_Surface* Tile::selected_image = NULL;

Tile::Tile(int i, int j) {
    x = i;
    y = j;

    character = NULL;

    set_selected(false);
}

Tile::Tile(int i, int j, int type) {
    x = i;
    y = j;

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

void Tile::set_character(Character* c) { character = c; }

bool Tile::get_selected() {
    return selected;
}

int Tile::get_x() { return x; }
int Tile::get_y() { return y; }

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

void Tile::move_character(vector<Tile*> move_tiles, SDL_Surface* surface) {
    character->move(x, y, move_tiles, surface);    
}
