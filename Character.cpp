#include "Header.h"

using namespace std;

Character::Character() {
    level     = 0;
    mobility  = 0;
    health    = 0;
    strength  = 0;
    range     = 0;
    increment = 0;

    moved_this_turn    = false;
    attacked_this_turn = false;

    name = "";
    image = NULL;
    grey_image = NULL;

    player = 0;
}

int  Character::get_player()             { return player;             }
int  Character::get_level()              { return level;              }
int  Character::get_mobility()           { return mobility;           }
int  Character::get_health()             { return health;             }
int  Character::get_strength()           { return strength;           }
int  Character::get_range()              { return range;              }
int  Character::get_max_health()         { return increment * level;  }
bool Character::get_moved_this_turn()    { return moved_this_turn;    }
bool Character::get_attacked_this_turn() { return attacked_this_turn; }
bool Character::get_healed_this_turn()   { return false;              }
bool Character::can_heal()               { return false;              }

int Character::get_x() { return x; }
int Character::get_y() { return y; }

void Character::set_x(int new_x) { x = new_x; }
void Character::set_y(int new_y) { y = new_y; }

std::string Character::get_name() { return name; }

SDL_Surface* Character::get_image() {
    if (moved_this_turn && (attacked_this_turn || get_healed_this_turn())) {
        return grey_image;
    }

    return image;
}

void Character::set_moved_this_turn(bool moved)       { moved_this_turn = moved;       }
void Character::set_attacked_this_turn(bool attacked) { attacked_this_turn = attacked; }

void Character::take_damage(int d)  { health -= d; }
void Character::gain_health(int h)  { health = std::min(health + h, get_max_health()); }

void Character::play_turn(SDL_Surface* surface, SDL_Surface* screen) {
    SDL_Delay(50);
    Tile* my_tile = Grid::get(x, y);
    my_tile->set_selected(true);
    Grid::draw_grid(surface);
    Util::update_screen(surface, screen);

    if (attack(surface)) {
        SDL_Delay(50);

        move(surface);

        my_tile->set_selected(false);
        my_tile = Grid::get(x, y);
        my_tile->set_selected(true);

        Grid::draw_grid(surface);
        Util::update_screen(surface, screen);
    } else {
        move(surface);

        my_tile->set_selected(false);
        my_tile = Grid::get(x, y);
        my_tile->set_selected(true);

        Grid::draw_grid(surface);
        Util::update_screen(surface, screen);

        SDL_Delay(50);

        attack(surface);
        Grid::draw_grid(surface);
        Util::update_screen(surface, screen);
    }

    SDL_Delay(50);
    my_tile->set_selected(false);
    Grid::draw_grid(surface);
    Util::update_screen(surface, screen);
}

// Implemented in subclasses
void Character::move(SDL_Surface* surface) {}
bool Character::attack(SDL_Surface* surface) {}

Character::~Character() {
    SDL_FreeSurface(image);
    SDL_FreeSurface(grey_image);
}
