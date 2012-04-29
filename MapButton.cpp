#include "Header.h"

using namespace std;

MapButton::MapButton(string file) {
    width = Constants::WIDTH - 200;
    height = Constants::HEIGHT / 4 + 10;

    Grid::load_file(file);

    filename = file;

    int begin = filename.find_last_of('/') + 1;
    int end = filename.find_last_of('.');

    name = filename.substr(begin, end - begin);

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
                                   width,
                                   height,
                                   Constants::SCREEN_BPP,
                                   0, 0, 0, 0);

    Util::apply_surface(5, 5, preview, surface);
    Util::apply_surface(130, 50, name_surface, surface);

    SDL_FreeSurface(preview);
    SDL_FreeSurface(name_surface);
}

int MapButton::get_width()  { return width;  }
int MapButton::get_height() { return height; }

SDL_Surface* MapButton::get_button() {
    return surface;
}

string MapButton::get_filename() { return filename; }

MapButton::~MapButton() {
    SDL_FreeSurface(surface);
}
