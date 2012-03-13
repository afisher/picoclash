#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "PlayerHealer.h"
#include "Util.h"

PlayerHealer::PlayerHealer() : Healer() {
    set_values(1);
    image = Util::load_image("sprites/healer-blue.png");
    grey_image = Util::load_image("sprites/healer-blue-desat.png");
    player = 1;
}

PlayerHealer::PlayerHealer(int lvl) : Healer(lvl) {
    set_values(lvl);
    image = Util::load_image("sprites/healer-blue.png");
    grey_image = Util::load_image("sprites/healer-blue-desat.png");
    player = 1;
}

