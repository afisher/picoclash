#include <string>
#include <iostream>
#include <fstream>

#include "Grid.h"

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
