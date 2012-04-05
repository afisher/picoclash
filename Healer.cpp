#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "Healer.h"
#include "Util.h"
#include <vector>
#include <iostream>

using namespace std;

Healer::Healer(int p) : Character() {
    set_values(p, 1);
}

Healer::Healer(int p, int lvl) : Character() {
    set_values(p, lvl);
}

void Healer::set_values(int p, int lvl) {
    player    = p;
    level     = lvl;
    mobility  = 2;
    increment = 5;
    health    = increment  * level;
    strength  = 1  * level;
    range     = 1;
    magic     = 10 * level;

    healed_this_turn = false;

    if (player == 1) {
        image      = Util::load_image("sprites/healer-blue.png");
        grey_image = Util::load_image("sprites/healer-blue-desat.png");
    } else {
        image      = Util::load_image("sprites/healer-red.png");
        grey_image = Util::load_image("sprites/healer-red-desat.png");
    }
}

int Healer::get_magic() { return magic; }
bool Healer::can_heal() { return true; }

bool Healer::get_healed_this_turn() { return healed_this_turn; }

void Healer::set_healed_this_turn(bool h) {
    healed_this_turn = h;
}

// moves toward its closest ally that is not a healer
void Healer::move(int x, int y, vector<Tile*> move_tiles, SDL_Surface* surface) {

    vector<Tile*> ally_tiles = Grid::get_character_tiles(player);

    int min_dist = 9999;
    Tile* closest_move_tile = NULL;

    for (int i = 0; i < move_tiles.size(); i++) {
        if (move_tiles[i]->get_character() == NULL) {
            for (int j = 0; j < ally_tiles.size(); j++) {
                int dist = Grid::distance(move_tiles[i]->get_x(), move_tiles[i]->get_y(),
                                          ally_tiles[j]->get_x(), ally_tiles[j]->get_y());
                if (dist < min_dist && !ally_tiles[j]->get_character()->can_heal()) {
                    min_dist = dist;
                    closest_move_tile = move_tiles[i];
                }
            }
        }
    }

    if (closest_move_tile != NULL) {
        if (Grid::move(y, x, closest_move_tile->get_y(), closest_move_tile->get_x(), surface)) {
            cout << "Move!" << endl;
        } else {
            cout << "Just chillin" << endl;
        }
    }
}

// for now, heal code is here -- heals if there's an ally in range, otherwise heals self
bool Healer::attack(int x, int y, vector<Tile*> attack_tiles, SDL_Surface* surface) {
    for (int i = 0; i < attack_tiles.size(); i++) {
        Character* cur_char = attack_tiles[i]->get_character();

        if (cur_char != NULL && cur_char->get_player() == player) {
            Grid::heal(y, x, attack_tiles[i]->get_y(), attack_tiles[i]->get_x(), surface);
            return true;
        }
    }

    return false;
}
