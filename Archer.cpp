#include "Archer.h"

class Archer: public Character {
    Archer() {
        set_values(1);
    }

    Archer(int lvl) {
        set_values(lvl);
    }

    void set_values(int lvl) {
        level    = lvl;
        mobility = 3;
        health   = 8 * level;
        strength = 3  * level;
        range    = 6;

        image = Util::load_image("sprites/warrior-blue.cpp");
    }
}
