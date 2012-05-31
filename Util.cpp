#include "Header.h"

using namespace std;

// intitializes the screen and returns it (returns null if something went wrong)
SDL_Surface* Util::init_screen(int width, int height, int bpp) {
    //Initialize all SDL subsystems
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
        return NULL;
    }

    SDL_Surface* screen = SDL_SetVideoMode(width, height, bpp, SDL_SWSURFACE);

    SDL_WM_SetCaption("Picoclash", NULL);

    return screen;
}

void Util::update_screen(SDL_Surface* source, SDL_Surface* destination) {
    SDL_Surface* scaled2x = scale2x(source);
    scale(scaled2x, destination);
    SDL_FreeSurface(scaled2x);
    
    SDL_Flip(destination);
}

// uses nearest-neighbor algorithm found here:
// http://tech-algorithm.com/articles/nearest-neighbor-image-scaling/
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

void Util::bilinear_scale(SDL_Surface* source, SDL_Surface* destination) {
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

    SDL_PixelFormat* format = source->format;

    for (int i = 0; i < h2; i++) {
        for (int j = 0; j < w2; j++) {
            int px = floor(j*x_ratio);
            int py = floor(i*y_ratio);

            int num_nbrs = 0;
            int red = 0;
            int green = 0;
            int blue = 0;

            if (py > 0) {
                num_nbrs++;

                Uint32 pixel = source_pixels[(int)((py-1)*w1 + px)];

                red   += (pixel & format->Rmask) >> format->Rshift;
                green += (pixel & format->Gmask) >> format->Gshift; 
                blue  += (pixel & format->Bmask) >> format->Bshift; 
            }

            if (py < h1-1) {
                num_nbrs++;

                Uint32 pixel =  source_pixels[(int)((py+1)*w1 + px)];

                red   += (pixel & format->Rmask) >> format->Rshift;
                green += (pixel & format->Gmask) >> format->Gshift; 
                blue  += (pixel & format->Bmask) >> format->Bshift; 
            }

            if (px > 0) {
                num_nbrs++;

                Uint32 pixel =  source_pixels[(int)(py*w1 + (px-1))];

                red   += (pixel & format->Rmask) >> format->Rshift;
                green += (pixel & format->Gmask) >> format->Gshift; 
                blue  += (pixel & format->Bmask) >> format->Bshift; 
            }

            if (px < w1-1) {
                num_nbrs++;

                Uint32 pixel = source_pixels[(int)(py*w1 + (px+1))];

                red   += (pixel & format->Rmask) >> format->Rshift;
                green += (pixel & format->Gmask) >> format->Gshift; 
                blue  += (pixel & format->Bmask) >> format->Bshift; 
            }

            destination_pixels[i*w2 + j] = SDL_MapRGB(format, red / num_nbrs, green / num_nbrs, blue / num_nbrs); 
        }
    }

    SDL_UnlockSurface(source);
    SDL_UnlockSurface(destination);
}

// uses scale 2x algorithm to create a smoother-looking result
// https://secure.wikimedia.org/wikipedia/en/wiki/Pixel_art_scaling_algorithms
SDL_Surface* Util::scale2x(SDL_Surface* source) {
    int w1 = source->w;
    int h1 = source->h;

    SDL_Surface* ret = SDL_CreateRGBSurface(SDL_SWSURFACE, w1*2, h1*2, 32, 0, 0, 0, 0);
    int w2 = ret->w;
    int h2 = ret->h;

    SDL_LockSurface(source);
    SDL_LockSurface(ret);

    Uint32* source_pixels = (Uint32*)source->pixels;
    Uint32* ret_pixels    = (Uint32*)ret->pixels;

    for (int i = 0; i < h1; i++) {
        for (int j = 0; j < w1; j++) {
            Uint32 a = i > 0      ? (Uint32)source_pixels[(i-1) * w1 + j] : 0; 
            Uint32 b = j < w1 - 1 ? (Uint32)source_pixels[i * w1 + (j+1)] : 0; 
            Uint32 c = j > 0      ? (Uint32)source_pixels[i * w1 + (j-1)] : 0; 
            Uint32 d = i < h1 - 1 ? (Uint32)source_pixels[(i+1) * w1 + j] : 0; 

            ret_pixels[(i*2) * w2 + (j*2)]         = (Uint32)source_pixels[i * w1 +j];
            ret_pixels[(i*2) * w2 + (j*2 + 1)]     = (Uint32)source_pixels[i * w1 +j];
            ret_pixels[(i*2 + 1) * w2 + (j*2 + 1)] = (Uint32)source_pixels[i * w1 +j];
            ret_pixels[(i*2 + 1) * w2 + (j*2)]     = (Uint32)source_pixels[i * w1 +j];

            if (c == a && c != d && a != b) {
                ret_pixels[(i*2) * w2 + (j*2)] = a;
            }

            if (a == b && a != c && b != d) {
                ret_pixels[(i*2) * w2 + (j*2 + 1)] = b;
            }

            if (b == d && b != a && d != c) {
                ret_pixels[(i*2 + 1) * w2 + (j*2 + 1)] = d;
            }

            if (d == c && d != b && c != a) {
                ret_pixels[(i*2 + 1) * w2 + (j*2)] = c;
            }
        }
    }

    SDL_UnlockSurface(source);
    SDL_UnlockSurface(ret);

    return ret;
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
void Util::apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip) {
    SDL_Rect offset;
    offset.x = x;
    offset.y = y;

    SDL_BlitSurface(source, clip, destination, &offset);
}
