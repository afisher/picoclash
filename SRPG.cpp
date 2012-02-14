#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <string>
#include <iostream>
#include <fstream>

#include "Util.h"
#include "Grid.h"
#include "PlayerWarrior.h"
#include "PlayerArcher.h"
#include "PlayerHealer.h"

const int SCREEN_WIDTH  = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP    = 32;

SDL_Surface* screen = NULL;

SDL_Event event;

int main(int argc, char* args[]) {
    bool quit = false;

    screen = Util::init_screen(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP);

    if (screen == NULL) return 1;

    //load_file();
    Grid grid;
    grid.draw_grid(screen);
    SDL_Flip(screen);


    while (quit == false) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                std::cout << "Quit Event";
                quit = true;
            } else if (event.type == SDL_MOUSEBUTTONDOWN &&
                       event.button.button == SDL_BUTTON_LEFT) {

                // integer division -- round down to the nearest multiple of SPRITE_SIZE
                int x = (event.button.x / Util::SPRITE_SIZE);
                int y = (event.button.y / Util::SPRITE_SIZE);

                Tile* selectedTile = grid.get(y, x);
                Character* selectedCharacter = selectedTile->get_character();

                if (selectedCharacter != NULL) {
                    // TODO make this into a sidebar display
                    std::cout << "Health: " << selectedCharacter->get_health() << "\n";
                    std::cout << "Strength: " << selectedCharacter->get_strength() << "\n";
                    std::cout << "Mobility: " << selectedCharacter->get_mobility() << "\n";
                    std::cout << "Range: " << selectedCharacter->get_range() << "\n";
                }

                std::cout << x << "\n";
                std::cout << y << "\n";
            }
        }
    }

    // TODO create cleanup function to free ALL the surfaces
    //SDL_FreeSurface(image);
    SDL_Quit();

    return 0;
}
