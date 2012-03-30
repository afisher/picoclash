#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "Warrior.h"
#include "Util.h"

Warrior::Warrior(int p) : Character() {
    set_values(p, 1);
}

Warrior::Warrior(int p, int lvl) : Character() {
    set_values(p, lvl);
}

void Warrior::set_values(int p, int lvl) {
    player    = p;
    level     = lvl;
    mobility  = 3;
    increment = 10;
    health    = increment * level;
    strength  = 5  * level;
    range     = 1;

    if (player == 1) {
        image      = Util::load_image("sprites/warrior-blue.png");
        grey_image = Util::load_image("sprites/warrior-blue-desat.png");
    } else {
        image      = Util::load_image("sprites/warrior-red.png");
        grey_image = Util::load_image("sprites/warrior-red-desat.png");
    }
}
