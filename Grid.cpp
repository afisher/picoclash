#include <string>
#include <iostream>
#include <fstream>
#include <cmath>

#include "Grid.h"
#include "Healer.h"

using namespace std;

Grid::Grid() {
    load_file();
    current_player = 1;
}

// loads the test map into the grid
void Grid::load_file() {
    std::ifstream file("testmap.txt");
    //std::ifstream file("offensemap.txt");
    std::string line;

    for (int i = 0; i < Constants::GRID_HEIGHT; i++) {
        std::getline(file, line);
        for (int j = 0; j < Constants::GRID_WIDTH; j++) {
            char c = line.at(j);
            std::cout << c;

            switch(c) {
                case 'w':
                    grid[i][j] = new Tile(this, j, i, Constants::PLAYER_WARRIOR); break;
                case 'a':
                    grid[i][j] = new Tile(this, j, i, Constants::PLAYER_ARCHER); break;
                case 'h':
                    grid[i][j] = new Tile(this, j, i, Constants::PLAYER_HEALER); break;
                case 'W':
                    grid[i][j] = new Tile(this, j, i, Constants::ENEMY_WARRIOR); break;
                case 'A':
                    grid[i][j] = new Tile(this, j, i, Constants::ENEMY_ARCHER); break;
                case 'H':
                    grid[i][j] = new Tile(this, j, i, Constants::ENEMY_HEALER); break;
                default:
                    grid[i][j] = new Tile(this, j, i);
            }
        }
        std::cout << "\n";
    }

    file.close();
}

// draw the grid -- assumes the file has been loaded
void Grid::draw_grid(SDL_Surface* surface) {
    for (int i = 0; i < Constants::GRID_HEIGHT; i++) {
        for (int j = 0; j < Constants::GRID_WIDTH; j++) {
            int x = j * Constants::SPRITE_SIZE;
            int y = i * Constants::SPRITE_SIZE;

            Util::apply_surface(x, y, grid[i][j]->get_image(), surface);

            if (grid[i][j]->get_character() != NULL) {
                Util::apply_surface(x, y, grid[i][j]->get_character()->get_image(), surface);
            }
        }
    }
}

Tile* Grid::get(int i, int j) { return grid[i][j]; }
int Grid::get_current_player() { return current_player; }

bool Grid::show_move_tiles(int i, int j, SDL_Surface* surface, bool show) {
    Character* selected_character = grid[i][j]->get_character();
    if (selected_character->get_player() != current_player) return false;

    int mobility = 0;

    if (selected_character != NULL) {
        mobility = selected_character->get_mobility();
        select_tiles(i, j, mobility, show);
    } else return false;

    draw_grid(surface);

    return true;
}

bool Grid::show_attack_tiles(int i, int j, SDL_Surface* surface, bool show) {
    Character* selected_character = grid[i][j]->get_character();
    if (selected_character->get_player() != current_player) return false;

    int range = 0;

    if (selected_character != NULL) {
        range = selected_character->get_range();
        select_tiles(i, j, range, show);
    } else return false;

    draw_grid(surface);

    return true;
}

void Grid::select_tiles(int i, int j, int range, bool show) {
    // interate over the range*range square
    for (int x = i - range; x <= i + range; x++) {
        for (int y = j - range; y <= j + range; y++) {
            // if the tile is within the range, light it up
            if (distance(i, j, x, y) <= range && x < Constants::GRID_WIDTH && y < Constants::GRID_HEIGHT) {
                grid[x][y]->set_selected(show);
            }
        }
    }
}

bool Grid::move(int i, int j, int x, int y, SDL_Surface* surface) {
    Character* curChar = grid[i][j]->get_character();
    if (curChar->get_player() != current_player) return false;

    int mobility = 0;

    if (curChar != NULL) {
        mobility = curChar->get_mobility();
    }

    // don't do anything if we try to move outside our mobility
    if (distance(i, j, x, y) > mobility) return false;

    Tile* selected_tile = grid[x][y];

    // move if we picked an empty square
    if (selected_tile->get_character() == NULL) {
        grid[x][y] = grid[i][j];
        grid[i][j] = new Tile(this, j, i);
        grid[x][y]->get_character()->set_moved_this_turn(true);
    } else return false;

    select_tiles(i, j, mobility, false);

    draw_grid(surface);

    return true;
}

bool Grid::attack(int i, int j, int x, int y, SDL_Surface* surface) {
    Character* character1 = grid[i][j]->get_character();
    Character* character2 = grid[x][y]->get_character();

    if (character1->get_player() != current_player) return false;
    if (character1 == NULL || character2 == NULL) return false;

    int range = character1->get_range();

    if (distance(i, j, x, y) <= range) {
        character2->take_damage(character1->get_strength());
        if (character2->get_health() <= 0) {
            grid[x][y]->character_died();
        }

        select_tiles(i, j, range, false);

        character1->set_attacked_this_turn(true);

        draw_grid(surface);

        return true;
    } else return false;
}

bool Grid::heal(int i, int j, int x, int y, SDL_Surface* surface) {
    Healer* character1 = (Healer*)(grid[i][j]->get_character());
    Character* character2 = grid[x][y]->get_character();

    if (character1->get_player() != current_player) return false;
    if (character1 == NULL || character2 == NULL) return false;

    int range = character1->get_range();

    if (distance(i, j, x, y) <= range) {
        character2->gain_health(character1->get_magic());
        character1->set_healed_this_turn(true); // tell the first character that it just healed

        select_tiles(i, j, range, false);

        character1->set_attacked_this_turn(true);

        draw_grid(surface);

        return true;
    } else return false;
}

int Grid::distance(int i, int j, int x, int y) {
    return abs(i - x) + abs(j - y);
}

void Grid::new_turn() {
    for (int j = 0; j < Constants::GRID_HEIGHT; j++) {
        for (int i = 0; i < Constants::GRID_WIDTH; i++) {
            Character* curChar = grid[j][i]->get_character();

            if (curChar != NULL) {
                curChar->set_moved_this_turn(false);
                curChar->set_attacked_this_turn(false);
                if (curChar->can_heal()) {
                    ((Healer*)curChar)->set_healed_this_turn(false);
                }
            }
        }
    }

    if (current_player == 1) current_player = 2;
    else current_player = 1;
}
