#pragma once

#include "Character.h"
#include <vector>

class Warrior : public Character {
    protected:
        void set_values(int p, int lvl);

    public:
        Warrior(int p);
        Warrior(int p, int lvl);

        virtual void move(SDL_Surface* surface);
        virtual bool attack(SDL_Surface* surface);
};
