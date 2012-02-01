#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <string>

#include "Util.h"
#include "Tile.h"

const int SCREEN_WIDTH  = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP    = 32;

const int GRID_WIDTH  = 20;
const int GRID_HEIGHT = 15;

const int SPRITE_SIZE = 16;

SDL_Surface* screen = NULL;

SDL_Event event;

int main(int argc, char* args[]) {
    bool quit = false;

    screen = Util::init_screen(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP);

    if (screen == NULL) {
        printf("Failed on screen");
        return 1;
    }

    Tile grass;
    SDL_Surface* image = grass.get_image();

    for (int x = 0; x < GRID_WIDTH*SPRITE_SIZE; x += SPRITE_SIZE) {
        for (int y = 0; y < GRID_HEIGHT*SPRITE_SIZE; y += SPRITE_SIZE) {
          Util::apply_surface(x, y, image, screen);
        }
    }

    if (SDL_Flip(screen) == -1) return 1;

    while (quit == false) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                printf("Quit Event");
                quit = true;
            }
        }
    }

    // TODO create cleanup function to free ALL the surfaces
    SDL_FreeSurface(image);
    SDL_Quit();

    return 0;
}
