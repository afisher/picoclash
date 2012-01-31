#pragma once

#include "Character.h"

class Archer : public Character {
    private:
        void set_values(int lvl);

    public:
        Archer();
        Archer(int lvl);
};

