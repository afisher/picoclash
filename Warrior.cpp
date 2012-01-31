#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

class Warrior: public Character {
    private:
        void set_values(int lvl);
};

Warrior::Warrior() {
    set_values(1);
}

Warrior::Warrior(int lvl) {
    set_values(lvl);
}

void Warrior::set_values(int lvl) {
    level    = lvl;
    mobility = 3;
    health   = 10 * level;
    strength = 5  * level;
    range    = 1;

    image = Util::load_image("sprites/warrior-blue.cpp");
}
