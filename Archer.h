#pragma once

#include "Character.h"

class Archer : public Character {
    protected:
        void set_values(int p, int lvl);

    public:
        Archer(int p);
        Archer(int p, int lvl);
};

