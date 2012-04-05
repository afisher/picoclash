#pragma once

#include "Character.h"
#include <vector>

class Warrior : public Character {
    protected:
        void set_values(int p, int lvl);

    public:
        Warrior(int p);
        Warrior(int p, int lvl);

        virtual void move(int x, int y, std::vector<Tile*> move_tiles, SDL_Surface* surface);
        virtual bool attack(int x, int y, std::vector<Tile*> attack_tiles, SDL_Surface* surface);
};
