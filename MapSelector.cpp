#include "Header.h"
#include <dirent.h>

using namespace std;

MapSelector::MapSelector() {
    buttons_per_page = 3;
    current_page = 0;
    
    x_padding = 120;
    y_padding = 30;

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

    int num_pages = (buttons.size() / buttons_per_page) + 1;

    for (int i = 0; i < num_pages; i++) {
        pages.push_back(SDL_CreateRGBSurface(SDL_SWSURFACE,
                          Constants::WIDTH,
                          Constants::HEIGHT,
                          Constants::SCREEN_BPP, 0, 0, 0, 0));
    }

    SDL_Surface* arrow_left  = Util::load_image("sprites/arrow-left.png");
    SDL_Surface* arrow_right = Util::load_image("sprites/arrow.png");

    for (int i = 0; i < buttons.size(); i++) {
        int page_number = i / buttons_per_page;
        int position = i % buttons_per_page;

        SDL_Surface* button = buttons[i]->get_button();

        Util::apply_surface(x_padding, y_padding + 130*position, button, pages[page_number]);

        Util::apply_surface(10, Constants::HEIGHT / 2 - 32, arrow_left, pages[page_number]);
        Util::apply_surface(Constants::WIDTH - 74, Constants::HEIGHT / 2 - 32, arrow_right, pages[page_number]);
    }
}

SDL_Surface* MapSelector::get_surface() {
    return pages[current_page];
}

MapButton* MapSelector::get_selected_button(int x, int y) {
    int start_index = current_page * buttons_per_page;
    int end_index = min(start_index + buttons_per_page, (int)(buttons.size()));

    for (int i = start_index; i < end_index; i++) {
        if (x >= Constants::X_RATIO*x_padding &&
            x <= Constants::X_RATIO*(x_padding+buttons[i]->get_width()) &&
            y >= Constants::Y_RATIO*(y_padding + buttons[i]->get_height()*(i%buttons_per_page)) &&
            y <= Constants::Y_RATIO*(y_padding + buttons[i]->get_height()*(i%buttons_per_page) + buttons[i]->get_height())) {

            return buttons[i];
        }
    }

    if (y >= Constants::Y_RATIO*(Constants::HEIGHT / 2 - 32) &&
        y <= Constants::Y_RATIO*(Constants::HEIGHT / 2 - 32 + 64)) {

        if (x >= Constants::X_RATIO*10 &&
            x <= Constants::X_RATIO*74) {
            previous_page();
        } else if (x >= Constants::X_RATIO*(Constants::WIDTH - 74) &&
                   x <= Constants::X_RATIO*(Constants::WIDTH - 74 + 64)) {
            next_page();
        }
    }


    return NULL;
}

void MapSelector::next_page() {
    if (current_page < pages.size() - 1) current_page++;
}

void MapSelector::previous_page() {
    if (current_page > 0) current_page--;
}

MapSelector::~MapSelector() {
    for (int i = 0; i < pages.size(); i++) {
        SDL_FreeSurface(pages[i]);
    }

    for (int i = 0; i < buttons.size(); i++) {
        delete buttons[i];
    }
}
