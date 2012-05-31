#include "Header.h"

using namespace std;

SDL_Surface* CheckButton::box_checked = NULL;
SDL_Surface* CheckButton::box_unchecked = NULL;

CheckButton::CheckButton(string label_string, bool checked) {
    width = 160;
    height = 32;

    surface = Util::load_image("sprites/check_bg.png");
                                /*SDL_CreateRGBSurface(SDL_SWSURFACE,
                                   width,
                                   height,
                                   Constants::SCREEN_BPP,
                                   0, 0, 0, 0);*/

    if (checked) {
        Util::apply_surface(0, 0, box_checked, surface);
    } else {
        Util::apply_surface(0, 0, box_unchecked, surface);
    }

    SDL_Color text_color = { 255, 255, 255 };
    SDL_Surface* label = TTF_RenderText_Solid(Grid::font, label_string.c_str(), text_color);

    Util::apply_surface(34, 0, label, surface);

    SDL_FreeSurface(label);
}

int CheckButton::get_width()  { return width;  }
int CheckButton::get_height() { return height; }

SDL_Surface* CheckButton::get_button() {
    return surface;
}

void CheckButton::set_checked() {
    Util::apply_surface(0, 0, box_checked, surface);
}

void CheckButton::set_unchecked() {
    Util::apply_surface(0, 0, box_unchecked, surface);
}

CheckButton::~CheckButton() {
    SDL_FreeSurface(surface);
}

