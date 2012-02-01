#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <string>
#include <iostream>

#include "Util.h"
#include "Tile.h"
#include "Warrior.h"
#include "Archer.h"
#include "Healer.h"

const int SCREEN_WIDTH  = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP    = 32;

const int GRID_WIDTH  = 20;
const int GRID_HEIGHT = 15;

const int SPRITE_SIZE = 16;

SDL_Surface* screen = NULL;
SDL_Surface* tile_image = NULL;

SDL_Event event;

bool draw_grid() {
    for (int x = 0; x < GRID_WIDTH*SPRITE_SIZE; x += SPRITE_SIZE) {
        for (int y = 0; y < GRID_HEIGHT*SPRITE_SIZE; y += SPRITE_SIZE) {
          Util::apply_surface(x, y, tile_image, screen);
        }
    }

    return SDL_Flip(screen);
}

void print_menu() {
    std::cout << "Please choose a character type to place.\n";
    std::cout << "1. Warrior\n";
    std::cout << "2. Archer\n";
    std::cout << "3. Healer\n";
    std::cout << "0. Done\n";
}

Character* menu() {
    int choice;

    print_menu();
    std::cin >> choice;

    /*int x;
    int y;
    std::cout << "Please enter an x coordinate: ";
    std::cin >> x;
    std::cout << "Please enter a y coordinate: ";
    std::cin >> y;*/

    Character* c;

    if (choice == 1) {
        c = new Warrior;
    } else if (choice == 2) {
        c = new Archer;
    } else if (choice == 3) {
        c = new Healer;
    } else c = NULL;

    return c;
/*
    SDL_Surface* image = c->get_image();
    Util::apply_surface(x * SPRITE_SIZE, y * SPRITE_SIZE, image, screen);

    SDL_Flip(screen);
*/
}

int main(int argc, char* args[]) {
    bool quit = false;

    screen = Util::init_screen(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP);

    if (screen == NULL) return 1;

    Tile grass;
    tile_image = grass.get_image();
    if (draw_grid() == -1) return 1;

    Character* c = menu();

    while (quit == false) {

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                std::cout << "Quit Event";
                quit = true;
            } else if (event.type == SDL_MOUSEBUTTONDOWN &&
                        event.button.button == SDL_BUTTON_LEFT) {

                // integer division -- round down to the nearest multiple of SPRITE_SIZE
                int x = (event.button.x / SPRITE_SIZE) * SPRITE_SIZE;
                int y = (event.button.y / SPRITE_SIZE) * SPRITE_SIZE;

                SDL_Surface* image = c->get_image();
                Util::apply_surface(x, y, image, screen);

                SDL_Flip(screen);
            }
        }
    }

    // TODO create cleanup function to free ALL the surfaces
    //SDL_FreeSurface(image);
    SDL_Quit();

    return 0;
}
