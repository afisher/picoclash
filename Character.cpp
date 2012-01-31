#include "Character.h"

class Character {
    int get_level()          { return level;    }
    int get_mobility()       { return mobility; }
    int get_health()         { return health;   }
    int get_strength()       { return strength; }
    int get_range()          { return range;    }
    std::string get_name()   { return name;     }
    SDL_Surface* get_image() { return image;    }
}
