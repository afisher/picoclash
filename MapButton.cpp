#include "Header.h"

using namespace std;

MapButton::MapButton(string filename) {
    Grid::load_file(filename);

    name = filename;

    SDL_Surface* temp_preview = SDL_CreateRGBSurface(SDL_SWSURFACE,
                                   Constants::HEIGHT,
                                   Constants::HEIGHT,
                                   Constants::SCREEN_BPP, 0, 0, 0, 0);
    Grid::draw_grid(temp_preview); 

    preview = SDL_CreateRGBSurface(SDL_SWSURFACE,
              Constants::HEIGHT / 4,
              Constants::HEIGHT / 4,
              Constants::SCREEN_BPP, 0, 0, 0, 0);

    Util::scale(temp_preview, preview);

    SDL_Color text_color = { 255, 255, 255 };
    name_surface = TTF_RenderText_Solid(Grid::font, name.c_str(), text_color);

    surface = SDL_CreateRGBSurface(SDL_SWSURFACE,
                               Constants::WIDTH - 220,
                               Constants::HEIGHT / 4 + 10,
                               Constants::SCREEN_BPP, 0, 0, 0, 0);

    Util::apply_surface(5, 5, preview, surface);
    Util::apply_surface(130, 50, name_surface, surface);

    SDL_FreeSurface(preview);
    SDL_FreeSurface(name_surface);
}

SDL_Surface* MapButton::get_button() {
    return surface;
}

MapButton::~MapButton() {
    SDL_FreeSurface(surface);
}
