#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "PlayerWarrior.h"
#include "Util.h"

PlayerWarrior::PlayerWarrior() : Warrior() {
    set_values(1);
    image = Util::load_image("sprites/warrior-blue.png");
    grey_image = Util::load_image("sprites/warrior-blue-desat.png");
}

PlayerWarrior::PlayerWarrior(int lvl) : Warrior(lvl) {
    set_values(lvl);
    image = Util::load_image("sprites/warrior-blue.png");
    grey_image = Util::load_image("sprites/warrior-blue-desat.png");
}
