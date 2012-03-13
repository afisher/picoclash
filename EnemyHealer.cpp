#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "EnemyHealer.h"
#include "Util.h"

EnemyHealer::EnemyHealer() : Healer() {
    set_values(1);
    image = Util::load_image("sprites/healer-red.png");
    grey_image = Util::load_image("sprites/healer-red-desat.png");
}

EnemyHealer::EnemyHealer(int lvl) : Healer(lvl) {
    set_values(lvl);
    image = Util::load_image("sprites/healer-red.png");
    grey_image = Util::load_image("sprites/healer-red-desat.png");
}


