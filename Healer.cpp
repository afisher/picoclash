#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

class Healer: public Character {
    private:
        int magic;
        void set_values(int lvl);

    public:
        int get_magic();
};

Healer::Healer() {
    set_values(1);
}

Healer::Healer(int lvl) {
    set_values(lvl);
}

void Healer::set_values(int lvl) {
    level    = lvl;
    mobility = 2;
    health   = 5  * level;
    strength = 1  * level;
    range    = 1;
    magic    = 10 * level;

    image = Util::load_image("sprites/healer-blue.png");
}

int Healer:;get_magic() { return magic; }
