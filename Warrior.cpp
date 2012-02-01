#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "Warrior.h"
#include "Util.h"

Warrior::Warrior() : Character() {
    set_values(1);
}

Warrior::Warrior(int lvl) : Character() {
    set_values(lvl);
}

void Warrior::set_values(int lvl) {
    level    = lvl;
    mobility = 3;
    health   = 10 * level;
    strength = 5  * level;
    range    = 1;

    image = Util::load_image("sprites/warrior-blue.cpp");
}
