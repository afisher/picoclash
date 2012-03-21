#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "Healer.h"
#include "Util.h"

Healer::Healer() : Character() {
    set_values(1);
}

Healer::Healer(int lvl) : Character() {
    set_values(lvl);
}

void Healer::set_values(int lvl) {
    level     = lvl;
    mobility  = 2;
    increment = 5;
    health    = increment  * level;
    strength  = 1  * level;
    range     = 1;
    magic     = 10 * level;

    healed_this_turn = false;

    image = Util::load_image("sprites/healer-blue.png");
}

int Healer::get_magic() { return magic; }
bool Healer::can_heal() { return true; }

bool Healer::get_healed_this_turn() { return healed_this_turn; }

void Healer::set_healed_this_turn(bool h) {
    healed_this_turn = h;
}
