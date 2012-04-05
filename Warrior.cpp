#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "Warrior.h"
#include "Util.h"
#include <iostream>
#include <vector>

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
void Warrior::move(int x, int y, vector<Tile*> move_tiles, SDL_Surface* surface) {
    int enemy_player = 1; // since this method is for AI, enemy is player 1

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

    if (Grid::move(y, x, closest_move_tile->get_y(), closest_move_tile->get_x(), surface)) {
        cout << "Move!" << endl;
    } else {
        cout << "Just chillin" << endl;
    }
}

// attacks if there is an enemy within range
bool Warrior::attack(int x, int y, std::vector<Tile*> attack_tiles, SDL_Surface* surface) {
    int enemy_player = 1; // since this method is for AI, enemy is player 1

    for (int i = 0; i < attack_tiles.size(); i++) {
        Character* cur_char = attack_tiles[i]->get_character();

        if (cur_char != NULL && cur_char->get_player() == enemy_player) {
            Grid::attack(y, x, attack_tiles[i]->get_y(), attack_tiles[i]->get_x(), surface);

            return true;
        }
    }

    return false;
}
