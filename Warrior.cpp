#include "Header.h"

using namespace std;

Warrior::Warrior(int p) : Character() {
    set_values(p, 1);
}

Warrior::Warrior(int p, int lvl) : Character() {
    set_values(p, lvl);
}

void Warrior::set_values(int p, int lvl) {
    player    = p;
    level     = lvl;
    mobility  = 3;
    increment = 10;
    health    = increment * level;
    strength  = 5  * level;
    range     = 1;

    if (player == 1) {
        image      = Util::load_image("sprites/warrior-blue.png");
        grey_image = Util::load_image("sprites/warrior-blue-desat.png");
    } else {
        image      = Util::load_image("sprites/warrior-red.png");
        grey_image = Util::load_image("sprites/warrior-red-desat.png");
    }
}

// moves toward the closest enemy
void Warrior::move(SDL_Surface* surface) {
    vector<Tile*> move_tiles = Grid::get_move_tiles(Grid::get(x, y), mobility);
    vector<Character*> enemies = Grid::get_player_characters();

    int min_dist = INT_MAX; 
    Tile* closest_move_tile = NULL;

    for (int i = 0; i < move_tiles.size(); i++) {
        if (move_tiles[i]->get_character() == NULL) {
            for (int j = 0; j < enemies.size(); j++) {
                int dist = Grid::distance(move_tiles[i]->get_x(), move_tiles[i]->get_y(),
                                          enemies[j]->get_x(), enemies[j]->get_y());
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

// attacks if there is an enemy within range
bool Warrior::attack(SDL_Surface* surface) {
    vector<Tile*> attack_tiles = Grid::get_range_tiles(Grid::get(x, y), range);

    for (int i = 0; i < attack_tiles.size(); i++) {
        Character* cur_char = attack_tiles[i]->get_character();

        if (cur_char != NULL && cur_char->get_player() != player) {
            Grid::attack(x, y, attack_tiles[i]->get_x(), attack_tiles[i]->get_y(), surface);
            return true;
        }
    }

    return false;
}
