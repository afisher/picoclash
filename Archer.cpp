#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "Archer.h"
#include "Util.h"

Archer::Archer() : Character() {
    set_values(1);
}

Archer::Archer(int lvl) : Character() {
    set_values(lvl);
}

void Archer::set_values(int lvl) {
    level    = lvl;
    mobility = 3;
    health   = 8 * level;
    strength = 3  * level;
    range    = 6;

    image = Util::load_image("sprites/warrior-blue.cpp");
}
