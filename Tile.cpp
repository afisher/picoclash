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
SDL_Surface* Tile::alt_image      = NULL;
SDL_Surface* Tile::selected_image = NULL;

Tile::Tile(int i, int j) {
    x = i;
    y = j;

    character = NULL;

    use_alt = false;
    if ((x*y)%11 == 3) use_alt = true;
    set_selected(false);
    update_image();
}

Tile::Tile(int i, int j, int type) {
    x = i;
    y = j;

    switch(type) {
        case Constants::PLAYER_WARRIOR:
            set_character(new Warrior(1)); break;
        case Constants::PLAYER_ARCHER:
            set_character(new Archer(1));  break;
        case Constants::PLAYER_HEALER:
            set_character(new Healer(1));  break;
        case Constants::ENEMY_WARRIOR:
            set_character(new Warrior(2)); break;
        case Constants::ENEMY_ARCHER:
            set_character(new Archer(2));  break;
        case Constants::ENEMY_HEALER:
            set_character(new Healer(2));  break;
        default:
            character = NULL; break;
    }

    set_selected(false);

    // add new character to the list of characters in the grid
    if (character != NULL) {
        switch (character->get_player()) {
            case 1: Grid::add_player_character(character); break;
            case 2: Grid::add_enemy_character(character);  break;
            default: break;
        }
    }

    use_alt = false;
    if ((x*y)%11 == 3) use_alt = true;
    update_image();
}

void Tile::set_selected(bool s) {
    selected = s;
    update_image();
}

void Tile::set_character(Character* c) { 
    character = c;
    if (character != NULL) {
        character->set_x(x);
        character->set_y(y);
    }
}

bool Tile::get_selected() {
    return selected;
}

int Tile::get_x() { return x; }
int Tile::get_y() { return y; }

void Tile::update_image() {
    if (selected) {
        image = selected_image;
    } else if (use_alt) {
        image = alt_image;
    } else {
        image = default_image;
    }
}

SDL_Surface* Tile::get_image()     { return image;     }
Character*   Tile::get_character() { return character; }

void Tile::character_died() {
    character = NULL;
}
