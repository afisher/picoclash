#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "Archer.h"
#include "Util.h"

Archer::Archer(int p) : Character() {
    set_values(p, 1);
}

Archer::Archer(int p, int lvl) : Character() {
    set_values(p, lvl);
}

void Archer::set_values(int p, int lvl) {
    player    = p;
    level     = lvl;
    mobility  = 3;
    increment = 8;
    health    = increment * level;
    strength  = 3  * level;
    range     = 6;

    if (player == 1) {
        image      = Util::load_image("sprites/archer-blue.png");
        grey_image = Util::load_image("sprites/archer-blue-desat.png");
    } else {
        image      = Util::load_image("sprites/archer-red.png");
        grey_image = Util::load_image("sprites/archer-red-desat.png");
    }
}
