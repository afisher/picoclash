#include "Warrior.h"

class Warrior: public Character {
    Warrior() {
        set_values(1);
    }

    Warrior(int lvl) {
        set_values(lvl);
    }

    void set_values(int lvl) {
        level    = lvl;
        mobility = 3;
        health   = 10 * level;
        strength = 5  * level;
        range    = 1;

        image = Util::load_image("sprites/warrior-blue.cpp");
    }
}
