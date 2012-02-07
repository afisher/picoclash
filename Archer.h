#pragma once

#include "Character.h"

class Archer : public Character {
    protected:
        void set_values(int lvl);

    public:
        Archer();
        Archer(int lvl);
};

