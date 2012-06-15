/*
 * This file is a part of Picoclash
 * Copyright (C) 2012 Ashley S. Fisher
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Header.h"
#include <dirent.h>

using namespace std;

MapSelector::MapSelector() {
    buttons_per_page = 3;
    current_page = 0;
    
    x_padding = 120;
    y_padding = 30;

    SDL_Surface* bg = Util::load_image("sprites/selector_bg.png");

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

    SDL_Surface* arrow_left  = Util::load_image("sprites/arrow-left.png");
    SDL_Surface* arrow_right = Util::load_image("sprites/arrow.png");

    SDL_Color text_color = { 255, 255, 255 };

    p_vs_cpu_button = new CheckButton("PvCPU", true); 
    p_vs_p_button = new CheckButton("PvP", false); 
    cpu_vs_cpu_button = new CheckButton("CPUvCPU", false); 

    int num_pages = (buttons.size() / buttons_per_page);
    if (buttons.size() % buttons_per_page != 0) num_pages++;

    string resize_str = "1 thru 5 - Resize Window";
    SDL_Surface* resize_info = TTF_RenderText_Solid(Grid::font, resize_str.c_str(), text_color);

    for (int i = 0; i < num_pages; i++) {
        SDL_Surface* page = SDL_CreateRGBSurface(SDL_SWSURFACE,
                            Constants::WIDTH,
                            Constants::HEIGHT,
                            Constants::SCREEN_BPP, 0, 0, 0, 0);

        // Apply the background
        Util::apply_surface(0, 0, bg, page);

        // Apply the arrow buttons
        Util::apply_surface(10, Constants::HEIGHT / 2 - 32, arrow_left, page);
        Util::apply_surface(Constants::WIDTH - 74, Constants::HEIGHT / 2 - 32, arrow_right, page);

        Util::apply_surface(440, 10, resize_info, page);

        pages.push_back(page);
    }

    SDL_FreeSurface(resize_info);

    draw_check_buttons();

    for (int i = 0; i < buttons.size(); i++) {
        int page_number = i / buttons_per_page;
        int position = i % buttons_per_page;

        SDL_Surface* button = buttons[i]->get_button();

        Util::apply_surface(x_padding, y_padding + 130*position, button, pages[page_number]);
    }

    SDL_FreeSurface(arrow_left);
    SDL_FreeSurface(arrow_right);

    previous_check_button = p_vs_cpu_button;
}

SDL_Surface* MapSelector::get_surface() {
    return pages[current_page];
}

MapButton* MapSelector::get_selected_button(int x, int y) {
    int start_index = current_page * buttons_per_page;
    int end_index = min(start_index + buttons_per_page, (int)(buttons.size()));

    // Check to see if one of the maps was selected
    for (int i = start_index; i < end_index; i++) {
        if (in_bounds(x, y, buttons[i], i)) {
            return buttons[i];
        }
    }

    // Check to see if one of the arrows was clicked
    if (y >= Util::Y_RATIO*(Constants::HEIGHT / 2 - 32) &&
        y <= Util::Y_RATIO*(Constants::HEIGHT / 2 - 32 + 64)) {

        if (x >= Util::X_RATIO*10 &&
            x <= Util::X_RATIO*74) {
            previous_page();
        } else if (x >= Util::X_RATIO*(Constants::WIDTH - 74) &&
                   x <= Util::X_RATIO*(Constants::WIDTH - 74 + 64)) {
            next_page();
        }
    }

    // Check to see if one of the check boxes was clicked
    if (y >= Util::Y_RATIO*(Constants::HEIGHT - 44) &&
        y <= Util::Y_RATIO*(Constants::HEIGHT - 44 + 32)) {
        
        if (x >= Util::X_RATIO*(x_padding) &&
            x <= Util::X_RATIO*(x_padding + 32)) {

            p_vs_cpu_clicked();
        } else if (x >= Util::X_RATIO*(x_padding + 160) &&
            x <= Util::X_RATIO*(x_padding + 160 + 32)) {

            p_vs_p_clicked();
        } else if (x >= Util::X_RATIO*(x_padding + 320) &&
            x <= Util::X_RATIO*(x_padding + 320 + 32)) {

            cpu_vs_cpu_clicked();
        }
    }

    return NULL;
}

bool MapSelector::in_bounds(int x, int y, MapButton* button, int button_number) {
    return x >= Util::X_RATIO*x_padding &&
           x <= Util::X_RATIO*(x_padding+button->get_width()) &&
           y >= Util::Y_RATIO*(y_padding + button->get_height()*(button_number%buttons_per_page)) &&
           y <= Util::Y_RATIO*(y_padding + button->get_height()*(button_number%buttons_per_page) + button->get_height());
}

void MapSelector::next_page() {
    if (current_page < pages.size() - 1) current_page++;
}

void MapSelector::previous_page() {
    if (current_page > 0) current_page--;
}

void MapSelector::p_vs_cpu_clicked() {
    Grid::set_game_type(Constants::P_V_CPU);

    previous_check_button->set_unchecked();
    previous_check_button = p_vs_cpu_button;

    p_vs_cpu_button->set_checked();

    draw_check_buttons();
}

void MapSelector::p_vs_p_clicked() {
    Grid::set_game_type(Constants::P_V_P);

    previous_check_button->set_unchecked();
    previous_check_button = p_vs_p_button;

    p_vs_p_button->set_checked();

    draw_check_buttons();
}

void MapSelector::cpu_vs_cpu_clicked() {
    Grid::set_game_type(Constants::CPU_V_CPU);

    previous_check_button->set_unchecked();
    previous_check_button = cpu_vs_cpu_button;

    cpu_vs_cpu_button->set_checked();

    draw_check_buttons();
}

void MapSelector::draw_check_buttons() {
    for (int i = 0; i < pages.size(); i++) {
        Util::apply_surface(x_padding, Constants::HEIGHT - 44, p_vs_cpu_button->get_button(), pages[i]);
        Util::apply_surface(x_padding + 160, Constants::HEIGHT - 44, p_vs_p_button->get_button(), pages[i]);
        Util::apply_surface(x_padding + 320, Constants::HEIGHT - 44, cpu_vs_cpu_button->get_button(),  pages[i]);
    }
}

MapSelector::~MapSelector() {
    for (int i = 0; i < pages.size(); i++) {
        SDL_FreeSurface(pages[i]);
    }

    for (int i = 0; i < buttons.size(); i++) {
        delete buttons[i];
    }
}
