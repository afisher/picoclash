#pragma once

class Tile {
    private:
        SDL_Surface* image;

    public:
        Tile();
        SDL_Surface* get_image();
};
