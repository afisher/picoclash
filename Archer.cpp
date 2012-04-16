#include "Header.h"

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

void Archer::move(SDL_Surface* surface) {
    vector<Tile*> move_tiles = Grid::get_move_tiles(Grid::get(x, y), mobility);

    int max_dist = 0;
    Tile* best_move_tile = NULL;
    for (int i = 0; i < move_tiles.size(); i++) {
        vector<Tile*> attack_tiles = Grid::get_range_tiles(move_tiles[i], range);

        // calculate the closest enemy within attack range of the move tile
        int closest_dist = INT_MAX;
        for (int j = 0; j < attack_tiles.size(); j++) {
            set<Tile*> attack_set;
            for (int k = 0; k < attack_tiles.size(); k++) {
                attack_set.insert(attack_tiles[k]);
            }

            int dist = Grid::real_distance(move_tiles[i], attack_tiles[j], &attack_set);

            // if this is the biggest distance to a closest enemy, then save this move as the best
            if (dist > max_dist && dist < closest_dist && attack_tiles[j]->get_character() != NULL
                && attack_tiles[j]->get_character()->get_player() != player) {
                closest_dist = dist;
                max_dist = dist;
                best_move_tile = move_tiles[i];
            }
        }
    }

    if (best_move_tile != NULL) {
        Grid::move(x, y, best_move_tile->get_x(), best_move_tile->get_y(), surface);
    } else { // if we couldn't find a good place to move to, just move towards the closest enemy
        vector<Character*> enemies = Grid::get_player_characters();

        int min_dist = 9999;
        Tile* closest_move_tile = NULL;

        for (int i = 0; i < move_tiles.size(); i++) {
            if (move_tiles[i]->get_character() == NULL) {

                set<Tile*> move_set;
                for (int j = 0; j < move_tiles.size(); j++) {
                    move_set.insert(move_tiles[j]);
                }

                for (int j = 0; j < enemies.size(); j++) {
                    int dist = Grid::real_distance(move_tiles[i], Grid::get(enemies[j]->get_x(), enemies[j]->get_y()), &move_set);
                    if (dist < min_dist) {
                        min_dist = dist;
                        closest_move_tile = move_tiles[i];
                    }
                }
            }
        }

        if (closest_move_tile != NULL) {
            Grid::move(x, y, closest_move_tile->get_x(), closest_move_tile->get_y(), surface);
        }
    }

    /*vector<Tile*> move_tiles = Grid::get_move_tiles(Grid::get(x, y), mobility);

    vector<Character*> enemies = Grid::get_player_characters();

    int min_dist = INT_MAX;
    Tile* closest_enemy_tile = NULL;
    for (int i = 0; i < enemies.size(); i++) {
        int dist = Grid::distance(x, y, enemies[i]->get_x(), enemies[i]->get_y());
        if (dist < min_dist) {
            min_dist = dist;
            closest_enemy_tile = Grid::get(enemies[i]->get_x(), enemies[i]->get_y());
        }
    }

    if (closest_enemy_tile == NULL) return;

    for (int i = 0; i < move_tiles.size(); i++) {
        vector<Tile*> nbrs = Grid::get_neighbors(move_tiles[i]);
        for (int j = 0; j < nbrs.size(); j++) {
            if (nbrs[j] == closest_enemy_tile) {
                Grid::move(x, y, move_tiles[i]->get_x(), move_tiles[i]->get_y(), surface);
                return;
            }
        }
    }

    min_dist = 0; 
    Tile* closest_move_tile = NULL;

    for (int i = 0; i < move_tiles.size(); i++) {
        if (move_tiles[i]->get_character() == NULL) {
            set<Tile*> move_set;
            for (int j = 0; j < move_tiles.size(); j++) {
                move_set.insert(move_tiles[j]);
            }

            int dist = Grid::real_distance(move_tiles[i], closest_enemy_tile, &move_set);
            if (dist > min_dist && dist != 0) {
                min_dist = dist;
                closest_move_tile = move_tiles[i];
            }
        }
    }

    if (closest_move_tile != NULL) {
        Grid::move(x, y, closest_move_tile->get_x(), closest_move_tile->get_y(), surface);
    }*/
}

bool Archer::attack(SDL_Surface* surface) {
    vector<Tile*> attack_tiles = Grid::get_range_tiles(Grid::get(x, y), range);

    for (int i = 0; i < attack_tiles.size(); i++) {
        Character* cur_char = attack_tiles[i]->get_character();

        if (cur_char != NULL && cur_char->get_player() != player) {
            Grid::attack(x, y, attack_tiles[i]->get_x(), attack_tiles[i]->get_y(), surface);
            return true;;
        }
    }

    return false;
}
