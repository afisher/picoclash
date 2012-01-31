#pragma once

#include "Character.h"

class Warrior : public Character {
    private:
        void set_values(int lvl);

    public:
        Warrior();
        Warrior(int lvl);
};
