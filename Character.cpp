#include "Character.h"

Character::Character() {
    level    = 0;
    mobility = 0;
    health   = 0;
    strength = 0;
    range    = 0;

    name = "";

    image = NULL;
}

int Character::get_level()          { return level;    }
int Character::get_mobility()       { return mobility; }
int Character::get_health()         { return health;   }
int Character::get_strength()       { return strength; }
int Character::get_range()          { return range;    }
std::string Character::get_name()   { return name;     }
SDL_Surface* Character::get_image() { return image;    }
