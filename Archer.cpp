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

    Tile* closest_enemy_tile = NULL;
    vector<Character*> enemies = Grid::get_player_characters();
    int min_dist = INT_MAX;
    for (int i = 0; i < enemies.size(); i++) {
        int dist = Grid::distance(x, y, enemies[i]->get_x(), enemies[i]->get_y());
        if (dist < min_dist) {
            min_dist = dist;
            closest_enemy_tile = Grid::get(enemies[i]->get_x(), enemies[i]->get_y());
        }
    }
    int max_dist = 0;
    Tile* best_move_tile = NULL;
    if (closest_enemy_tile != NULL) {
        for (int i = 0; i < move_tiles.size(); i++) {
            int dist = Grid::distance(move_tiles[i], closest_enemy_tile);

            // if this is the biggest distance to a closest enemy, then save this move as the best
            if (dist <= range && dist > max_dist) {
                max_dist = dist;
                best_move_tile = move_tiles[i];
            }
        }
    }

    vector<Tile*> path;
    if (best_move_tile != NULL) {
        path = Grid::path_search(Grid::get(x, y), best_move_tile);
    } else if (closest_enemy_tile != NULL) {
        path = Grid::path_search(Grid::get(x, y), closest_enemy_tile);
    } else {
        return;
    }

    Tile* closest_move_tile = NULL;

    int size = path.size();
    if (size > 0) {
        int index = min(size-1, mobility);
        closest_move_tile = path[index];
    }

    if (closest_move_tile != NULL) {
        Grid::move(x, y, closest_move_tile->get_x(), closest_move_tile->get_y(), surface);
    }
}

bool Archer::attack(SDL_Surface* surface, SDL_Surface* screen) {
    vector<Tile*> attack_tiles = Grid::get_range_tiles(Grid::get(x, y), range);

    for (int i = 0; i < attack_tiles.size(); i++) {
        Character* cur_char = attack_tiles[i]->get_character();

        if (cur_char != NULL && cur_char->get_player() != player) {
            attack_tiles[i]->set_attack_on(true);
            Grid::draw_tile(attack_tiles[i], surface);
            Util::update_screen(surface, screen);
            SDL_Delay(400);

            Grid::attack(x, y, attack_tiles[i]->get_x(), attack_tiles[i]->get_y(), surface);
            return true;;
        }
    }

    return false;
}
