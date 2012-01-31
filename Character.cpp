#include "Character.h"

class Character {
    int level()          { return level;    }
    int mobility()       { return mobility; }
    int health()         { return health;   }
    int strength()       { return strength; }
    int range()          { return range;    }
    std::string name()   { return name;     }
    SDL_Surface* image() { return image;    }
}
