#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "EnemyWarrior.h"
#include "Util.h"

EnemyWarrior::EnemyWarrior() : Warrior() {
    set_values(1);
    image = Util::load_image("sprites/warrior-red.png");
}

EnemyWarrior::EnemyWarrior(int lvl) : Warrior(lvl) {
    set_values(lvl);
    image = Util::load_image("sprites/warrior-red.png");
}

