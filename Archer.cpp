#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "Archer.h"
#include "Util.h"
#include <vector>
#include <iostream>

using namespace std;

Archer::Archer(int p) : Character() {
    set_values(p, 1);
}

Archer::Archer(int p, int lvl) : Character() {
    set_values(p, lvl);
}

void Archer::set_values(int p, int lvl) {
    player    = p;
    level     = lvl;
    mobility  = 3;
    increment = 8;
    health    = increment * level;
    strength  = 3  * level;
    range     = 6;

    if (player == 1) {
        image      = Util::load_image("sprites/archer-blue.png");
        grey_image = Util::load_image("sprites/archer-blue-desat.png");
    } else {
        image      = Util::load_image("sprites/archer-red.png");
        grey_image = Util::load_image("sprites/archer-red-desat.png");
    }
}

void Archer::move(int x, int y, vector<Tile*> move_tiles, SDL_Surface* surface) {
    int enemy_player = 1;

    int max_dist = 0;
    Tile* best_move_tile = NULL;
    for (int i = 0; i < move_tiles.size(); i++) {
        vector<Tile*> attack_tiles = Grid::get_range_tiles(move_tiles[i], range);

        // calculate the closest enemy within attacking range
        int closest_dist = 9999;
        for (int j = 0; j < attack_tiles.size(); j++) {
            int dist = Grid::distance(move_tiles[i]->get_x(), move_tiles[i]->get_y(),
                                      attack_tiles[j]->get_x(), attack_tiles[j]->get_y());

            // if this is the biggest distance to a closest enemy, then save this move as the best
            if (dist > max_dist && dist < closest_dist && attack_tiles[j]->get_character() != NULL
                && attack_tiles[j]->get_character()->get_player() == enemy_player) {
                closest_dist = dist;
                max_dist = dist;
                best_move_tile = move_tiles[i];
            }
        }
    }

    if (best_move_tile != NULL) {
        Grid::move(y, x, best_move_tile->get_y(), best_move_tile->get_x(), surface);
    } else { // if we couldn't find a good place to move to, just move towards the closest enemy
        vector<Tile*> enemy_tiles = Grid::get_character_tiles(enemy_player);

        int min_dist = 9999;
        Tile* closest_move_tile = NULL;

        for (int i = 0; i < move_tiles.size(); i++) {
            if (move_tiles[i]->get_character() == NULL) {
                for (int j = 0; j < enemy_tiles.size(); j++) {
                    int dist = Grid::distance(move_tiles[i]->get_x(), move_tiles[i]->get_y(), 
                            enemy_tiles[j]->get_x(), enemy_tiles[j]->get_y());
                    if (dist < min_dist) {
                        min_dist = dist;
                        closest_move_tile = move_tiles[i];
                    }
                }
            }
        }

        Grid::move(y, x, closest_move_tile->get_y(), closest_move_tile->get_x(), surface);
    }
}

bool Archer::attack(int x, int y, vector<Tile*> attack_tiles, SDL_Surface* surface) {

    int enemy_player = 1; // since this method is for AI, enemy is player 1

    for (int i = 0; i < attack_tiles.size(); i++) {
        Character* cur_char = attack_tiles[i]->get_character();

        if (cur_char != NULL && cur_char->get_player() == enemy_player) {
            Grid::attack(y, x, attack_tiles[i]->get_y(), attack_tiles[i]->get_x(), surface);
            return true;;
        }
    }

    return false;
}
