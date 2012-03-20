#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <string>
#include <iostream>
#include "Util.h"
#include <cmath>

using namespace std;

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

void Util::update_screen(SDL_Surface* source, SDL_Surface* destination) {
    //apply_surface(0, 0, source, destination);
    scale(source, destination);
    SDL_Flip(destination);
}

void Util::scale(SDL_Surface* source, SDL_Surface* destination) {
    SDL_LockSurface(source);
    SDL_LockSurface(destination);

    Uint32* source_pixels      = (Uint32*)source->pixels;
    Uint32* destination_pixels = (Uint32*)destination->pixels;

    int w1 = source->w;
    int h1 = source->h;
    int w2 = destination->w;
    int h2 = destination->h;

    double x_ratio = w1 / (double)w2;
    double y_ratio = h1 / (double)h2;

    for (int i = 0; i < h2; i++) {
        for (int j = 0; j < w2; j++) {
            int px = floor(j*x_ratio);
            int py = floor(i*y_ratio);

            destination_pixels[i*w2 + j] = source_pixels[(int)(py*w1 + px)];
        }
    }

    SDL_UnlockSurface(source);
    SDL_UnlockSurface(destination);
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
