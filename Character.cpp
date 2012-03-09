#include "Character.h"
#include "SDL/SDL.h"

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
}

int  Character::get_level()              { return level;              }
int  Character::get_mobility()           { return mobility;           }
int  Character::get_health()             { return health;             }
int  Character::get_strength()           { return strength;           }
int  Character::get_range()              { return range;              }
int  Character::get_max_health()         { return increment * level;  }
bool Character::get_moved_this_turn()    { return moved_this_turn;    }
bool Character::get_attacked_this_turn() { return attacked_this_turn; }

std::string  Character::get_name()   { return name;  }
SDL_Surface* Character::get_image()  { return image; }

void Character::set_moved_this_turn(bool moved)       { moved_this_turn = moved;       }
void Character::set_attacked_this_turn(bool attacked) { attacked_this_turn = attacked; }

void Character::take_damage(int d)  { health -= d;              }

Character::~Character() {
    SDL_FreeSurface(image);
}
