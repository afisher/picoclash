#include "Healer.h"

class Healer: public Character {
    Healer() {
        set_values(1);
    }

    Healer(int lvl) {
        set_values(lvl);
    }

    void set_values(int lvl) {
        level    = lvl;
        mobility = 2;
        health   = 5  * level;
        strength = 1  * level;
        range    = 1;
        magic    = 10 * level;

        image = Util::load_image("sprites/healer-blue.png");
    }

    int magic() { return magic; }
}
