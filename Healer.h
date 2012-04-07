#pragma once

#include "Character.h"
#include <vector>

class Healer : public Character {
    protected:
        int magic;
        bool healed_this_turn;
        void set_values(int p, int lvl);

    public:
        Healer(int p);
        Healer(int p, int lvl);

        int get_magic();

        virtual bool get_healed_this_turn();
        void set_healed_this_turn(bool h);

        virtual bool can_heal();

        virtual void move(SDL_Surface* surface);
        virtual bool attack(SDL_Surface* surface);
};
