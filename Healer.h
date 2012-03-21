#pragma once

#include "Character.h"

class Healer : public Character {
    protected:
        int magic;
        bool healed_this_turn;
        void set_values(int lvl);

    public:
        Healer();
        Healer(int lvl);

        int get_magic();

        virtual bool get_healed_this_turn();
        void set_healed_this_turn(bool h);

        virtual bool can_heal();
};
