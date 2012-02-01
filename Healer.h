#pragma once

#include "Character.h"

class Healer : public Character {
    private:
        int magic;
        void set_values(int lvl);

    public:
        Healer();
        Healer(int lvl);

        int get_magic();
};
