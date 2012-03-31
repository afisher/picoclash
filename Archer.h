#pragma once

#include "Character.h"
#include <vector>

class Archer : public Character {
    protected:
        void set_values(int p, int lvl);

    public:
        Archer(int p);
        Archer(int p, int lvl);
        virtual void move(int x, int y, std::vector<Tile*> move_tiles, SDL_Surface* surface);
};
