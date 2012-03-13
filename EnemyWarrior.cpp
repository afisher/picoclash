#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "EnemyWarrior.h"
#include "Util.h"

EnemyWarrior::EnemyWarrior() : Warrior() {
    set_values(1);
    image = Util::load_image("sprites/warrior-red.png");
    grey_image = Util::load_image("sprites/warrior-red-desat.png");
    player = 2;
}

EnemyWarrior::EnemyWarrior(int lvl) : Warrior(lvl) {
    set_values(lvl);
    image = Util::load_image("sprites/warrior-red.png");
    grey_image = Util::load_image("sprites/warrior-red-desat.png");
    player = 2;
}

