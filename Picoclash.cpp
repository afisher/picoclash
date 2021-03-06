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

using namespace std;

SDL_Surface* screen       = NULL;
SDL_Surface* surface      = NULL;
SDL_Surface* title_screen = NULL;
SDL_Surface* win_blue     = NULL;
SDL_Surface* win_red      = NULL;

SDL_Event event;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

static void clean_up() {
    SDL_FreeSurface(Tile::default_image);
    SDL_FreeSurface(Tile::alt_image);
    SDL_FreeSurface(Tile::selected_image);
    SDL_FreeSurface(Tile::move_image);
    SDL_FreeSurface(Tile::attack_image);
    SDL_FreeSurface(Tile::heal_image);

    SDL_FreeSurface(RockTile::default_image);
    SDL_FreeSurface(RockTile::alt_image);
    SDL_FreeSurface(RockTile::selected_image);
    SDL_FreeSurface(RockTile::move_image);
    SDL_FreeSurface(RockTile::attack_image);
    SDL_FreeSurface(RockTile::heal_image);

    SDL_FreeSurface(title_screen);
    SDL_FreeSurface(win_blue);
    SDL_FreeSurface(win_red);

    SDL_FreeSurface(Grid::grid_image);
    SDL_FreeSurface(surface);
    SDL_FreeSurface(screen);
    
    Grid::clean_up();
    Sound::clean_up();
}

static void handle_resize(SDL_Surface* screen) {
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_1) {
        screen == Util::init_screen(640, 480, Constants::SCREEN_BPP);
    } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_2) {
        screen == Util::init_screen(800, 600, Constants::SCREEN_BPP);
    } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_3) {
        screen == Util::init_screen(960, 720, Constants::SCREEN_BPP);
    } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_4) {
        screen == Util::init_screen(1280, 960, Constants::SCREEN_BPP);
    } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_5) {
        screen == Util::init_screen(1400, 1080, Constants::SCREEN_BPP);
    }
}

int main(int argc, char* args[]) {

    bool quit = false;

    screen = Util::init_screen(SCREEN_WIDTH,
                               SCREEN_HEIGHT,
                               Constants::SCREEN_BPP);

    surface = SDL_CreateRGBSurface(SDL_SWSURFACE,
                                   Constants::WIDTH,
                                   Constants::HEIGHT,
                                   Constants::SCREEN_BPP, 0, 0, 0, 0);

    Util::update_ratios(screen);

    Tile::default_image  = Util::load_image("sprites/grass-plain.png");
    Tile::alt_image      = Util::load_image("sprites/grass-alt.png");
    Tile::selected_image = Util::load_image("sprites/overlay-selected.png");
    Tile::move_image     = Util::load_image("sprites/overlay-move.png");
    Tile::attack_image   = Util::load_image("sprites/overlay-attack.png");
    Tile::heal_image     = Util::load_image("sprites/overlay-heal.png");

    RockTile::default_image  = Util::load_image("sprites/rock.png");
    RockTile::alt_image      = Util::load_image("sprites/rock.png");
    RockTile::selected_image = Util::load_image("sprites/overlay-selected.png");
    RockTile::move_image     = Util::load_image("sprites/overlay-move.png");
    RockTile::attack_image   = Util::load_image("sprites/overlay-attack.png");
    RockTile::heal_image     = Util::load_image("sprites/overlay-heal.png");

    CheckButton::box_checked   = Util::load_image("sprites/box_checked.png");
    CheckButton::box_unchecked = Util::load_image("sprites/box_unchecked.png");

    if (screen == NULL)   return 1;
    if (TTF_Init() == -1) return 1;

    Grid::font = TTF_OpenFont("fonts/04B-03/04B_03__.TTF", 14);
    if (Grid::font == NULL) return 1;

    // make the sidebar surface
    Grid::sidebar = Util::load_image("sprites/sidebar-bg.png");

    // make the "are you sure?" surface
    SDL_Surface* quit_screen = Util::load_image("sprites/quit.png");

    StateMachine::init();
    Sound::init();

    // make the title screen surface
    title_screen = Util::load_image("sprites/title.png");

    SDL_Color text_color = { 255, 255, 255 };

    string copyright_str = "Copyright (C) 2012 Ashley S. Fisher";
    SDL_Surface* copyright_info = TTF_RenderText_Solid(Grid::font, copyright_str.c_str(), text_color);
    Util::apply_surface(10, 10, copyright_info, title_screen);

    string resize_str = "1 thru 5 - Resize Window";
    SDL_Surface* resize_info = TTF_RenderText_Solid(Grid::font, resize_str.c_str(), text_color);
    Util::apply_surface(440, 10, resize_info, title_screen);

    SDL_FreeSurface(resize_info);
    SDL_FreeSurface(copyright_info);

    Util::update_screen(title_screen, screen);

    while (quit == false) {
        int start_ticks = SDL_GetTicks();

        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
                break;
            }
            handle_resize(screen);
        }

        int ticks = SDL_GetTicks() - start_ticks;
        if (ticks < 1000 / Constants::FRAMES_PER_SECOND) {
            SDL_Delay((1000 / Constants::FRAMES_PER_SECOND) - ticks);
        }

        Util::update_screen(title_screen, screen);
    }

    // make the map selector
    MapSelector* selector = new MapSelector();

    bool game_started = false;
    bool game_paused  = false;

    // make surfaces for win screens
    win_blue = Util::load_image("sprites/win_blue.png");
    win_red = Util::load_image("sprites/win_red.png");

    while (quit == false) {

        int start_ticks = SDL_GetTicks();

        while (SDL_PollEvent(&event)) {
            handle_resize(screen);

            if (event.type == SDL_QUIT) {
                quit = true;
            } else if (event.type == SDL_MOUSEBUTTONDOWN && !game_started) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    int x = event.button.x;
                    int y = event.button.y;
                    
                    MapButton* selected_button = selector->get_selected_button(x, y);
                    if (selected_button != NULL) {
                        string filename = selector->get_selected_button(x, y)->get_filename();

                        Grid::load_file(filename);
                        Grid::draw_grid(surface);

                        Grid::draw_sidebar(surface);

                        Util::update_screen(surface, screen);

                        if (Grid::get_game_type() == Constants::CPU_V_CPU) {
                            Grid::set_characters_to_act(Grid::get_player_characters());
                        }

                        game_started = true;
                    }
                }
            } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_LEFT && !game_started) {
                selector->previous_page();
            } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RIGHT && !game_started) {
                selector->next_page();
            } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN && game_started && Grid::game_over()) {
                game_started = false;
            } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_BACKSPACE && game_started && !game_paused) {
                game_paused = true;
            } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN && game_paused) {
                game_paused  = false;
                game_started = false;
            } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE && game_paused) {
                game_paused = false;
            } else if (game_started && Grid::get_game_type() != Constants::CPU_V_CPU) {
                StateMachine::execute(event, surface, screen);
            }
        }

        /*if (game_started && !Grid::game_over() && !game_paused && Grid::get_game_type() == Constants::CPU_V_CPU) {
            Grid::play_ai_turn(surface, screen);
        }*/

        if (game_started && !Grid::game_over() && !game_paused) { 
            Grid::move_next_character(surface, screen);
        }

        int ticks = SDL_GetTicks() - start_ticks;
        if (ticks < 1000 / Constants::FRAMES_PER_SECOND) {
            SDL_Delay((1000 / Constants::FRAMES_PER_SECOND) - ticks);
        }

        if (Grid::game_over() && game_started) {
            if (Grid::get_player_characters().size() == 0) {
                Util::update_screen(win_red, screen);
            } else if (Grid::get_enemy_characters().size() == 0) {
                Util::update_screen(win_blue, screen);
            }

        } else if (game_paused) {
            Util::update_screen(quit_screen, screen);
        } else if (game_started) {
            Util::update_screen(surface, screen);
        } else {
            Util::update_screen(selector->get_surface(), screen);
        }
    }

    delete selector;
    clean_up();
    SDL_Quit();

    return 0;
}
