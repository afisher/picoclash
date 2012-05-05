#include "Header.h"

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
    magic     = 4 * level;

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
void Healer::move(SDL_Surface* surface) {
    vector<Tile*> move_tiles = Grid::get_move_tiles(Grid::get(x, y), mobility);

    vector<Character*> allies = Grid::get_enemy_characters();

    // Find closest ally
    int min_dist = INT_MAX;
    Tile* closest_ally_tile = NULL;
    for (int i = 0; i < allies.size(); i++) {
        int dist = Grid::distance(x, y, allies[i]->get_x(), allies[i]->get_y());
        if (dist < min_dist && !allies[i]->can_heal()) {
            min_dist = dist;
            closest_ally_tile = Grid::get(allies[i]->get_x(), allies[i]->get_y());
        }
    }

    if (closest_ally_tile == NULL) return;

    Tile* closest_move_tile = NULL;

    vector<Tile*> path = Grid::path_search(Grid::get(x, y), closest_ally_tile);
    int size = path.size();
    if (size > 1) {
        int index = min(size-2, mobility);
        closest_move_tile = path[index];
    }

    if (closest_move_tile != NULL) {
        Grid::move(x, y, closest_move_tile->get_x(), closest_move_tile->get_y(), surface);
    }
}

// for now, heal code is here -- heals if there's an ally in range, otherwise heals self
bool Healer::attack(SDL_Surface* surface, SDL_Surface* screen) {
    vector<Tile*> attack_tiles = Grid::get_range_tiles(Grid::get(x, y), range);

    for (int i = 0; i < attack_tiles.size(); i++) {
        Character* cur_char = attack_tiles[i]->get_character();

        if (cur_char != NULL && cur_char->get_player() == player && cur_char->get_health() < cur_char->get_max_health() && 
            (cur_char->get_x() != x || cur_char->get_y() != y)) {

            attack_tiles[i]->set_heal_on(true);
            Grid::draw_tile(attack_tiles[i], surface);
            Util::update_screen(surface, screen);
            SDL_Delay(400);

            Grid::heal(x, y, attack_tiles[i]->get_x(), attack_tiles[i]->get_y(), surface);
            attack_tiles[i]->set_heal_on(false);
            return true;
        }
    }

    return false;
}
