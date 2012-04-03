#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "Healer.h"
#include "Util.h"
#include <vector>

using namespace std;

Healer::Healer(int p) : Character() {
    set_values(p, 1);
}

Healer::Healer(int p, int lvl) : Character() {
    set_values(p, lvl);
}

void Healer::set_values(int p, int lvl) {
    player    = p;
    level     = lvl;
    mobility  = 2;
    increment = 5;
    health    = increment  * level;
    strength  = 1  * level;
    range     = 1;
    magic     = 10 * level;

    healed_this_turn = false;

    if (player == 1) {
        image      = Util::load_image("sprites/healer-blue.png");
        grey_image = Util::load_image("sprites/healer-blue-desat.png");
    } else {
        image      = Util::load_image("sprites/healer-red.png");
        grey_image = Util::load_image("sprites/healer-red-desat.png");
    }
}

int Healer::get_magic() { return magic; }
bool Healer::can_heal() { return true; }

bool Healer::get_healed_this_turn() { return healed_this_turn; }

void Healer::set_healed_this_turn(bool h) {
    healed_this_turn = h;
}

void Healer::move(int x, int y, vector<Tile*> move_tiles, SDL_Surface* surface) {}
void Healer::attack(int x, int y, vector<Tile*> attack_tiles, SDL_Surface* surface) {}
