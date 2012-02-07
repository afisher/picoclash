#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "PlayerArcher.h"
#include "Util.h"

PlayerArcher::PlayerArcher() : Archer() {
    set_values(1);
    image = Util::load_image("sprites/archer-blue.png");
}

PlayerArcher::PlayerArcher(int lvl) : Archer(lvl) {
    set_values(lvl);
    image = Util::load_image("sprites/archer-blue.png");
}
