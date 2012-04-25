#include "Header.h"
#include <dirent.h>

using namespace std;

MapSelector::MapSelector() {
    surface = SDL_CreateRGBSurface(SDL_SWSURFACE,
                  Constants::WIDTH,
                  Constants::HEIGHT,
                  Constants::SCREEN_BPP, 0, 0, 0, 0);

    DIR* dir;
    struct dirent *entry;
    if(dir = opendir("maps/"))
    {
        while(entry = readdir(dir)) {
            //printf("%s\n", entry->d_name);
            if (strcmp(entry->d_name, "..") != 0 && strcmp(entry->d_name, ".") != 0) {
                string name = "maps/";
                name.append(entry->d_name);
                MapButton* current_button = new MapButton(name);
                buttons.push_back(current_button);
            }
        }
        closedir(dir);
    }  

    for (int i = 0; i < buttons.size(); i++) {
        SDL_Surface* button = buttons[i]->get_button();
        Util::apply_surface(120, 30 + 130*i, button, surface);
    }
}

SDL_Surface* MapSelector::get_surface() {
    return surface;
}

MapButton* MapSelector::get_selected_button(int x, int y) {
    for (int i = 0; i < buttons.size(); i++) {
        if (x >= Constants::X_RATIO*120 && x <= Constants::X_RATIO*(120+Constants::WIDTH - 220) &&
            y >= Constants::Y_RATIO*(30 + 130*i) && y <= Constants::Y_RATIO*(30 + 130*i + Constants::HEIGHT / 4 + 10)) {

            return buttons[i];
        }
    }

    return NULL;
}

MapSelector::~MapSelector() {
    SDL_FreeSurface(surface);
}
