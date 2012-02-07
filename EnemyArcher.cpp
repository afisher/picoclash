#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "EnemyArcher.h"
#include "Util.h"

EnemyArcher::EnemyArcher() : Archer() {
    set_values(1);
    image = Util::load_image("sprites/archer-red.png");
}

EnemyArcher::EnemyArcher(int lvl) : Archer(lvl) {
    set_values(lvl);
    image = Util::load_image("sprites/archer-red.png");
}
