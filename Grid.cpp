#include <string>
#include <iostream>
#include <fstream>
#include <cmath>

#include "Grid.h"

using namespace std;

Grid::Grid() {
    load_file();
}

// loads the test map into the grid
void Grid::load_file() {
    std::ifstream file("testmap.txt");
    std::string line;

    for (int i = 0; i < Util::GRID_HEIGHT; i++) {
        std::getline(file, line);
        for (int j = 0; j < Util::GRID_WIDTH; j++) {
            char c = line.at(j);
            std::cout << c;

            switch(c) {
                case 'w':
                    grid[i][j] = new Tile(Util::PLAYER_WARRIOR); break;
                case 'a':
                    grid[i][j] = new Tile(Util::PLAYER_ARCHER); break;
                case 'h':
                    grid[i][j] = new Tile(Util::PLAYER_HEALER); break;
                case 'W':
                    grid[i][j] = new Tile(Util::ENEMY_WARRIOR); break;
                case 'A':
                    grid[i][j] = new Tile(Util::ENEMY_ARCHER); break;
                case 'H':
                    grid[i][j] = new Tile(Util::ENEMY_HEALER); break;
                default:
                    grid[i][j] = new Tile();
            }
        }
        std::cout << "\n";
    }

    file.close();
}

// draw the grid -- assumes the file has been loaded
void Grid::draw_grid(SDL_Surface* screen) {
    for (int i = 0; i < Util::GRID_HEIGHT; i++) {
        for (int j = 0; j < Util::GRID_WIDTH; j++) {
            int x = j * Util::SPRITE_SIZE;
            int y = i * Util::SPRITE_SIZE;

            Util::apply_surface(x, y, grid[i][j]->get_image(), screen);

            if (grid[i][j]->get_character() != NULL) {
                Util::apply_surface(x, y, grid[i][j]->get_character()->get_image(), screen);
            }
        }
    }
}

Tile* Grid::get(int i, int j) {
    return grid[i][j];
}

void Grid::show_move_tiles(int i, int j, SDL_Surface* screen) {
    Character* selected_character = grid[i][j]->get_character();
    int mobility = 0;
    if (selected_character != NULL) {
        mobility = selected_character->get_mobility();

        // interate over the mobility*mobility square 
        for (int x = i - mobility; x <= i + mobility; x++) {
            for (int y = j - mobility; y <= j + mobility; y++) {
                // if the tile is within the mobility, light it up
                if (distance(i, j, x, y) <= mobility) {
                    grid[x][y]->set_selected(true);
                }
            }
        }
    }
    draw_grid(screen);
    SDL_Flip(screen);
}

bool Grid::move(int i, int j, int x, int y, SDL_Surface* screen) {
    int mobility = 0;
    
    if (grid[i][j]->get_character() != NULL) {
        mobility = grid[i][j]->get_character()->get_mobility();
    }

    // don't do anything if we try to move outside our mobility
    if (distance(i, j, x, y) > mobility) return false;

    Tile* selected_tile = grid[x][y];
    Character* selected_character = selected_tile->get_character();

    if (selected_character == NULL) {
        grid[x][y] = grid[i][j];
        grid[i][j] = new Tile();
    } else return false; 

    // interate over the mobility*mobility square 
    for (int a = i - mobility; a <= i + mobility; a++) {
        for (int b = j - mobility; b <= j + mobility; b++) {
            grid[a][b]->set_selected(false);
        }
    }
    draw_grid(screen);
    SDL_Flip(screen);

    return true;
}

int Grid::distance(int i, int j, int x, int y) {
    return abs(i - x) + abs(j - y); 
}
