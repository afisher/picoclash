#pragma once

#include "Character.h"
#include <vector>

class Archer : public Character {
    protected:
        void set_values(int p, int lvl);

    public:
        Archer(int p);
        Archer(int p, int lvl);

        virtual void play_turn(SDL_Surface* surface);
        virtual void move(SDL_Surface* surface);
        virtual bool attack(SDL_Surface* surface);
};
