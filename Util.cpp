#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <string>
#include "Util.h"

// intitializes the screen and returns it (returns null if something went wrong)
SDL_Surface* Util::init_screen(int width, int height, int bpp) {
    //Initialize all SDL subsystems
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
        return NULL;
    }

    SDL_Surface* screen = SDL_SetVideoMode(width, height, bpp, SDL_SWSURFACE);

    SDL_WM_SetCaption("Someday soon I'll be a game!", NULL);

    return screen;
}

// loads an image from a file name and returns it as a surface
SDL_Surface* Util::load_image(std::string filename) {
    SDL_Surface* loadedImage    = NULL;
    SDL_Surface* optimizedImage = NULL;

    loadedImage = IMG_Load(filename.c_str());

    // if nothing went wrong on load
    if (loadedImage != NULL) {
        //Create an optimized image
        optimizedImage = SDL_DisplayFormatAlpha(loadedImage);

        //Free the old image
        SDL_FreeSurface(loadedImage);
    }

    return optimizedImage;
}

// applies one surface to another based on x and y coords
void Util::apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination) {
    SDL_Rect offset;
    offset.x = x;
    offset.y = y;

    SDL_BlitSurface(source, NULL, destination, &offset);
}
