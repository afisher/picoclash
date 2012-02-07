#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <string>
#include <iostream>
#include <fstream>

#include "Util.h"
#include "Tile.h"
#include "PlayerWarrior.h"
#include "PlayerArcher.h"
#include "PlayerHealer.h"

const int SCREEN_WIDTH  = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP    = 32;

const int SPRITE_SIZE = 16;

const int GRID_WIDTH  = 30;
const int GRID_HEIGHT = 30;

SDL_Surface* screen = NULL;

Tile* grid [GRID_HEIGHT][GRID_WIDTH];

SDL_Event event;

/*
void print_menu() {
    std::cout << "Please choose a character type to place.\n"
              << "1. Warrior\n"
              << "2. Archer\n"
              << "3. Healer\n"
              << "0. Done\n";
}

Character* menu() {
    int choice;

    print_menu();
    std::cin >> choice;

    Character* c;

    if (choice == 1) {
        c = new PlayerWarrior;
    } else if (choice == 2) {
        c = new PlayerArcher;
    } else if (choice == 3) {
        c = new PlayerHealer;
    } else c = NULL;

    return c;
}*/

// loads the test map into the grid
void load_file() {
    std::ifstream file("testmap.txt");
    std::string line;

    for (int i = 0; i < GRID_HEIGHT; i++) {
        std::getline(file, line);    
        for (int j = 0; j < GRID_WIDTH; j++) {
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
bool draw_grid() {
    for (int i = 0; i < GRID_HEIGHT; i++) {
        for (int j = 0; j < GRID_WIDTH; j++) {
            int x = i * SPRITE_SIZE;
            int y = j * SPRITE_SIZE;

            Util::apply_surface(x, y, grid[i][j]->get_image(), screen);

            if (grid[i][j]->get_character() != NULL) {
                Util::apply_surface(x, y, grid[i][j]->get_character()->get_image(), screen);
            }
        }
    }

    return SDL_Flip(screen);
}

int main(int argc, char* args[]) {
    bool quit = false;

    screen = Util::init_screen(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP);

    if (screen == NULL) return 1;

    /*Tile grass;
    tile_image = grass.get_image();
    if (draw_grid() == -1) return 1;*/

    load_file();
    draw_grid();


/*    while (true) {

        Character* c = menu();
        if (c == NULL) {
            break;
        }

        while (SDL_WaitEvent(&event)) {
            if (event.type == SDL_MOUSEBUTTONDOWN &&
                        event.button.button == SDL_BUTTON_LEFT) {

                // integer division -- round down to the nearest multiple of SPRITE_SIZE
                int x = (event.button.x / SPRITE_SIZE) * SPRITE_SIZE;
                int y = (event.button.y / SPRITE_SIZE) * SPRITE_SIZE;

                SDL_Surface* image = c->get_image();
                Util::apply_surface(x, y, image, screen);

                SDL_Flip(screen);

                break;
            }
        }
    }
*/
    while (quit == false) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                std::cout << "Quit Event";
                quit = true;
            }
        }
    }

    // TODO create cleanup function to free ALL the surfaces
    //SDL_FreeSurface(image);
    SDL_Quit();

    return 0;
}
