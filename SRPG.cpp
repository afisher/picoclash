#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Util.h"
#include "Constants.h"
#include "Grid.h"
#include "Healer.h"

SDL_Surface* screen  = NULL;
SDL_Surface* surface = NULL;
SDL_Surface* sidebar = NULL;

TTF_Font* font = NULL;
SDL_Color text_color = { 255, 255, 255 };
SDL_Color grey_color = { 130, 130, 130 };

SDL_Event event;

int x = 0, y = 0; // current selected position
int new_x = 0, new_y = 0; // position for moving/attacking

Tile* selected_tile = NULL;
Character* selected_character = NULL;

// temporary home for the possible states
enum States { IDLE, SELECTED, MOVING, ATTACKING, HEALING };

int state = IDLE;

using namespace std;

void draw_sidebar() {
    Util::apply_surface(480, 0, sidebar, surface);

    if (selected_character != NULL) {
        // Build stat info
        stringstream health;
        stringstream strength;
        stringstream mobility;
        stringstream range;

        health   << "Health: "   << selected_character->get_health()
                 << " / "        << selected_character->get_max_health();
        strength << "Strength: " << selected_character->get_strength();
        mobility << "Mobility: " << selected_character->get_mobility();
        range    << "Range: "    << selected_character->get_range();

        SDL_Surface* health_stats   = TTF_RenderText_Solid(font, health.str().c_str(),   text_color);
        SDL_Surface* strength_stats = TTF_RenderText_Solid(font, strength.str().c_str(), text_color);
        SDL_Surface* mobility_stats = TTF_RenderText_Solid(font, mobility.str().c_str(), text_color);
        SDL_Surface* range_stats    = TTF_RenderText_Solid(font, range.str().c_str(),    text_color);

        Util::apply_surface(486, 10, health_stats, surface);
        Util::apply_surface(486, 30, strength_stats, surface);
        Util::apply_surface(486, 50, mobility_stats, surface);
        Util::apply_surface(486, 70, range_stats, surface);

        SDL_FreeSurface(health_stats);
        SDL_FreeSurface(strength_stats);
        SDL_FreeSurface(mobility_stats);
        SDL_FreeSurface(range_stats);

        // Build controls info
        
        string move   = "z - Move";
        string attack = "x - Attack";
        string heal   = "c - Heal";

        SDL_Color color;
        if (selected_character->get_moved_this_turn()) {
            color = grey_color;
        } else {
            color = text_color;
        }
        SDL_Surface* move_control = TTF_RenderText_Solid(font, move.c_str(), color);

        if (selected_character->get_attacked_this_turn()) {
            color = grey_color;
        } else {
            color = text_color;
        }
        SDL_Surface* attack_control = TTF_RenderText_Solid(font, attack.c_str(), color);

        if (!selected_character->can_heal() || selected_character->get_attacked_this_turn()) {
            color = grey_color;
        } else {
            color = text_color;
        }
        SDL_Surface* heal_control = TTF_RenderText_Solid(font, heal.c_str(), color);


        Util::apply_surface(486, 200, move_control, surface);
        Util::apply_surface(486, 220, attack_control, surface);
        Util::apply_surface(486, 240, heal_control, surface);

        SDL_FreeSurface(move_control);
        SDL_FreeSurface(attack_control);
        SDL_FreeSurface(heal_control);

    }

    string end = "v - End turn";
    SDL_Surface* end_control = TTF_RenderText_Solid(font, end.c_str(), text_color);
    Util::apply_surface(486, 260, end_control, surface);
    SDL_FreeSurface(end_control);

    string grid = "shift - Toggle grid";
    SDL_Surface* grid_control = TTF_RenderText_Solid(font, grid.c_str(), text_color);
    Util::apply_surface(486, 300, grid_control, surface);
    SDL_FreeSurface(grid_control);

    string turn_str = "";
    int current_player = Grid::get_current_player();
    if (current_player == 1) {
        turn_str = "Blue turn";
    } else {
        turn_str = "Red turn";
    }
    SDL_Surface* turn_info = TTF_RenderText_Solid(font, turn_str.c_str(), text_color);
    Util::apply_surface(486, 440, turn_info, surface);
    SDL_FreeSurface(turn_info);

    string state_str = "";
    switch (state) {
        case IDLE:      state_str = "Select a character";   break;
        case SELECTED:  state_str = "Choose an action";     break;
        case MOVING:    state_str = "Select a square";      break;
        case ATTACKING: state_str = "Select a victim";      break;
        case HEALING:   state_str = "Select an ally";       break;
        default: break;
    }
    SDL_Surface* state_info = TTF_RenderText_Solid(font, state_str.c_str(), text_color);
    Util::apply_surface(486, 460, state_info, surface);
    SDL_FreeSurface(state_info);
}

void select_single() {
    x = Constants::X_RATIO * event.button.x / Constants::SPRITE_SIZE;
    y = Constants::Y_RATIO * event.button.y / Constants::SPRITE_SIZE;

    selected_tile = Grid::get(x, y);
    selected_character = selected_tile->get_character();

    // highlight the tile
    selected_tile->set_selected(true);
    Grid::draw_grid(surface);

    draw_sidebar();
}

void clean_up() {
    SDL_FreeSurface(Tile::default_image);
    SDL_FreeSurface(Tile::selected_image);
    SDL_FreeSurface(sidebar);
    SDL_FreeSurface(surface);
    SDL_FreeSurface(screen);
}

int main(int argc, char* args[]) {
    bool quit = false;

    screen = Util::init_screen(Constants::SCREEN_WIDTH,
                               Constants::SCREEN_HEIGHT,
                               Constants::SCREEN_BPP);

    surface = SDL_CreateRGBSurface(SDL_SWSURFACE,
                                   Constants::WIDTH,
                                   Constants::HEIGHT,
                                   Constants::SCREEN_BPP, 0, 0, 0, 0);

    Tile::default_image  = Util::load_image("sprites/grass2.png");
    Tile::selected_image = Util::load_image("sprites/grass2-selected.png");

    if (screen == NULL)   return 1;
    if (TTF_Init() == -1) return 1;

    font = TTF_OpenFont("fonts/04B-03/04B_03__.TTF", 14);
    if (font == NULL) return 1;

    Grid::load_file();
    Grid::draw_grid(surface);
    Util::update_screen(surface, screen);

    sidebar = Util::load_image("sprites/sidebar-bg.png");
    draw_sidebar();

    bool success;

    while (quit == false) {

        int start_ticks = SDL_GetTicks();

        while (SDL_PollEvent(&event)) {

            if (event.type == SDL_QUIT) {
                cout << "Quit Event" << endl;
                quit = true;
            } else if (event.type == SDL_MOUSEMOTION && state != MOVING
                                                     && state != ATTACKING
                                                     && state != HEALING) {
                //selected_tile = Grid::get(x, y);

                if (selected_tile != NULL) {
                    // unhighlight the old selected tile
                    selected_tile->set_selected(false);
                    Grid::draw_grid(surface);
                }

                x = Constants::X_RATIO * event.motion.x / Constants::SPRITE_SIZE; 
                y = Constants::Y_RATIO * event.motion.y / Constants::SPRITE_SIZE; 
                if (x < Constants::GRID_WIDTH && y < Constants::GRID_HEIGHT) { 
                    selected_tile = Grid::get(x, y);
                    selected_character = selected_tile->get_character();

                    state = SELECTED;

                    // highlight the tile
                    selected_tile->set_selected(true);
                    Grid::draw_grid(surface);

                    draw_sidebar();
                }
            } else if (event.type == SDL_MOUSEBUTTONDOWN &&
                       event.button.button == SDL_BUTTON_LEFT) {

                switch (state) {
                    case IDLE:
                        select_single();
                        state = SELECTED;
                        break;
                    case SELECTED:
                        // this happens if we change our character selection
                        selected_tile = Grid::get(x, y);

                        // unhighlight the old selected tile
                        selected_tile->set_selected(false);
                        Grid::draw_grid(surface);

                        select_single();
                        break;
                    case MOVING:
                        // this happens if we select a place to move to
                        new_x = Constants::X_RATIO * event.button.x / Constants::SPRITE_SIZE;
                        new_y = Constants::Y_RATIO * event.button.y / Constants::SPRITE_SIZE;

                        success = Grid::move(x, y, new_x, new_y, surface);

                        if (success) {
                            state = SELECTED;
                            //selected_character = NULL;
                            selected_tile = Grid::get(selected_character->get_x(), selected_character->get_y());
                            select_single();
                        }
                        break;
                    case ATTACKING:
                        // this happens if we select a character to attack
                        new_x = Constants::X_RATIO * event.button.x / Constants::SPRITE_SIZE;
                        new_y = Constants::Y_RATIO * event.button.y / Constants::SPRITE_SIZE;

                        success = Grid::attack(x, y, new_x, new_y, surface);

                        if (success) {
                            state = SELECTED;
                            //selected_character = NULL;
                            selected_tile = Grid::get(selected_character->get_x(), selected_character->get_y());
                            select_single();
                        }
                        break;
                    case HEALING:
                        // this happens if we select a character to heal
                        new_x = Constants::X_RATIO * event.button.x / Constants::SPRITE_SIZE;
                        new_y = Constants::Y_RATIO * event.button.y / Constants::SPRITE_SIZE;

                        success = Grid::heal(x, y, new_x, new_y, surface);

                        if (success) {
                            state = SELECTED;
                            //selected_character = NULL;
                            selected_tile = Grid::get(selected_character->get_x(), selected_character->get_y());
                            select_single();
                        }
                        break;
                    default: break;
                }
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_LSHIFT || event.key.keysym.sym == SDLK_RSHIFT) {
                    Grid::toggle_show_lines(surface);
                }
                if (event.key.keysym.sym == SDLK_z) {
                    if (state == SELECTED) {
                        // this happens if we choose to move after another action
                        if (selected_character != NULL && !selected_character->get_moved_this_turn()) {
                            success = Grid::show_move_tiles(x, y, surface, true);
                            if (success) state = MOVING;
                        }
                    }
                } else if (event.key.keysym.sym == SDLK_x) {
                    if (state == SELECTED) {
                        // this happens if we choose to attack after another action
                        if (selected_character != NULL && !selected_character->get_attacked_this_turn()) {
                            success = Grid::show_attack_tiles(x, y, surface, true);
                            if (success) state = ATTACKING;
                        }
                    }
                } else if (event.key.keysym.sym == SDLK_c) {
                    if (state == SELECTED) {
                        // this happens if we choose to heal after another action
                        if (selected_character != NULL && selected_character->can_heal()
                                                       && !((Healer*)selected_character)->get_healed_this_turn()) {
                            // just use the attack range for now
                            success = Grid::show_attack_tiles(x, y, surface, true);
                            if (success) state = HEALING;
                        }
                    }
                } else if (event.key.keysym.sym == SDLK_ESCAPE) {
                    if (state == MOVING || state == ATTACKING || state == HEALING) {
                        // this happens if we cancel
                        if (state == MOVING) {
                            success = Grid::show_move_tiles(x, y, surface, false);
                        } else {
                            success = Grid::show_attack_tiles(x, y, surface, false);
                        }

                        Grid::get(x, y)->set_selected(true);
                        Grid::draw_grid(surface);
                        if (success) state = SELECTED;
                    }
                }
            } else if (event.key.keysym.sym == SDLK_v) {
                // this happens if we choose to end the turn
                Grid::new_turn();
                Grid::draw_grid(surface);
                if (Grid::get_current_player() == 2) {
                    draw_sidebar();
                    Grid::play_ai_turn(surface, screen);
                    Grid::new_turn();
                    Grid::draw_grid(surface);
                }
                //Grid::new_turn();
                //Grid::draw_grid(surface);
            }
        }
        draw_sidebar();


        int ticks = SDL_GetTicks() - start_ticks;
        if (ticks < 1000 / Constants::FRAMES_PER_SECOND) {
            SDL_Delay((1000 / Constants::FRAMES_PER_SECOND) - ticks);
        }

        Util::update_screen(surface, screen);
    }

    clean_up();
    SDL_Quit();

    return 0;
}
