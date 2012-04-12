#include "Header.h"

using namespace std;

SDL_Surface* Tile::default_image  = NULL;
SDL_Surface* Tile::alt_image      = NULL;
SDL_Surface* Tile::selected_image = NULL;
SDL_Surface* Tile::move_image     = NULL;
SDL_Surface* Tile::attack_image   = NULL;

Tile::Tile(int i, int j) {
    x = i;
    y = j;

    character = NULL;

    use_alt = false;
    if ((x*y)%11 == 3) use_alt = true;

    selected  = false;
    move_on   = false;
    attack_on = false;

    update_image();
    update_overlay_image();
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

    selected  = false;
    move_on   = false;
    attack_on = false;

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
    update_overlay_image();
}

void Tile::set_selected(bool s) {
    selected = s;
    update_overlay_image();
}

void Tile::set_move_on(bool s) {
    move_on = s;
    update_overlay_image();
}

void Tile::set_attack_on(bool s) {
    attack_on = s;
    update_overlay_image();
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

void Tile::update_overlay_image() {
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

void Tile::update_image() {
    if (use_alt) {
        image = alt_image;
    } else {
        image = default_image;
    }
}

SDL_Surface* Tile::get_image()         { return image;     }
SDL_Surface* Tile::get_overlay_image() { return overlay_image; }
Character*   Tile::get_character()     { return character; }

void Tile::character_died() {
    character = NULL;
}
