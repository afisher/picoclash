#pragma once

#include "Character.h"

class Warrior : public Character {
    protected:
        void set_values(int lvl);

    public:
        Warrior();
        Warrior(int lvl);
};
