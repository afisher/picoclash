#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Util.h"
#include "Grid.h"
#include "PlayerWarrior.h"
#include "PlayerArcher.h"
#include "PlayerHealer.h"

const int SCREEN_WIDTH  = 1280;
const int SCREEN_HEIGHT = 960;
const int SCREEN_BPP    = 32;

const int WIDTH  = 640;
const int HEIGHT = 480;

const double X_RATIO = WIDTH  / (double)SCREEN_WIDTH;
const double Y_RATIO = HEIGHT / (double)SCREEN_HEIGHT;

SDL_Surface* screen  = NULL;
SDL_Surface* surface = NULL;
SDL_Surface* sidebar = NULL;

TTF_Font* font = NULL;
SDL_Color textColor = { 255, 255, 255 };

SDL_Event event;

int x = 0, y = 0; // current selected position
int new_x = 0, new_y = 0; // position for moving/attacking

Tile* selected_tile = NULL;
Character* selected_character = NULL;

// temporary home for the possible states
enum States { IDLE, SELECTED, MOVING, MOVED, ATTACKING, ATTACKED };

int state = IDLE;

using namespace std;

void draw_sidebar(Grid grid) {
    Util::apply_surface(480, 0, sidebar, surface);

    if (selected_character != NULL) {
        stringstream health;
        stringstream strength;
        stringstream mobility;
        stringstream range;

        health   << "Health: "   << selected_character->get_health()
                 << " / "        << selected_character->get_max_health();
        strength << "Strength: " << selected_character->get_strength();
        mobility << "Mobility: " << selected_character->get_mobility();
        range    << "Range: "    << selected_character->get_range();

        SDL_Surface* health_stats   = TTF_RenderText_Solid(font, health.str().c_str(), textColor);
        SDL_Surface* strength_stats = TTF_RenderText_Solid(font, strength.str().c_str(), textColor);
        SDL_Surface* mobility_stats = TTF_RenderText_Solid(font, mobility.str().c_str(), textColor);
        SDL_Surface* range_stats    = TTF_RenderText_Solid(font, range.str().c_str(), textColor);

        Util::apply_surface(486, 10, health_stats, surface);
        Util::apply_surface(486, 30, strength_stats, surface);
        Util::apply_surface(486, 50, mobility_stats, surface);
        Util::apply_surface(486, 70, range_stats, surface);

        SDL_FreeSurface(health_stats);
        SDL_FreeSurface(strength_stats);
        SDL_FreeSurface(mobility_stats);
        SDL_FreeSurface(range_stats);
    }

    string turn_str = "";
    int current_player = grid.get_current_player();
    if (current_player == 1) {
        turn_str = "Blue turn";
    } else {
        turn_str = "Red turn";
    }
    SDL_Surface* turn_info = TTF_RenderText_Solid(font, turn_str.c_str(), textColor);
    Util::apply_surface(486, 260, turn_info, surface);
    SDL_FreeSurface(turn_info);

    string state_str = "";
    switch (state) {
        case IDLE:      state_str = "Select a character";   break;
        case SELECTED:  state_str = "Choose an action";     break;
        case MOVING:    state_str = "Select a square";      break;
        case MOVED:     state_str = "Select a character";   break;
        case ATTACKING: state_str = "Select a victim";      break;
        case ATTACKED:  state_str = "Select a character";   break;
        default: break;
    }
    SDL_Surface* state_info = TTF_RenderText_Solid(font, state_str.c_str(), textColor);
    Util::apply_surface(486, 360, state_info, surface);
    SDL_FreeSurface(state_info);

    Util::update_screen(surface, screen);
}

void select_single(Grid grid) {
    x = X_RATIO * event.button.x / Util::SPRITE_SIZE;
    y = Y_RATIO * event.button.y / Util::SPRITE_SIZE;

    selected_tile = grid.get(y, x);
    selected_character = selected_tile->get_character();

    // highlight the character
    if (selected_character != NULL) {
        selected_tile->set_selected(true);
        grid.draw_grid(surface);

        draw_sidebar(grid);
    }
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

    screen = Util::init_screen(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP);
    surface = SDL_CreateRGBSurface(SDL_SWSURFACE, WIDTH, HEIGHT, SCREEN_BPP, 0, 0, 0, 0);

    Tile::default_image  = Util::load_image("sprites/grass2.png");
    Tile::selected_image = Util::load_image("sprites/grass2-selected.png");

    if (screen == NULL)   return 1;
    if (TTF_Init() == -1) return 1;

    font = TTF_OpenFont("fonts/04B-03/04B_03__.TTF", 14);
    if (font == NULL) return 1;

    Grid grid;
    grid.draw_grid(surface);
    Util::update_screen(surface, screen);

    sidebar = Util::load_image("sprites/sidebar-bg.png");
    draw_sidebar(grid);

    bool success;

    while (quit == false) {

        while (SDL_PollEvent(&event)) {

            if (event.type == SDL_QUIT) {
                cout << "Quit Event" << endl;
                quit = true;
            } else if (event.type == SDL_MOUSEBUTTONDOWN &&
                    event.button.button == SDL_BUTTON_LEFT) {

                switch (state) {
                    case IDLE:
                        select_single(grid);
                        state = SELECTED;
                        break;
                    case SELECTED:
                        // this happens if we change our character selection
                        selected_tile = grid.get(y, x);
                        selected_character = selected_tile->get_character();

                        // unhighlight the old selected character
                        if (selected_character != NULL) {
                            selected_tile->set_selected(false);
                            grid.draw_grid(surface);

                            Util::update_screen(surface, screen);
                        }

                        select_single(grid);
                        break;
                    case MOVING:
                        // this happens if we select a place to move to
                        new_x = X_RATIO * event.button.x / Util::SPRITE_SIZE;
                        new_y = Y_RATIO * event.button.y / Util::SPRITE_SIZE;

                        success = grid.move(y, x, new_y, new_x, surface);
                        Util::update_screen(surface, screen);

                        if (success) state = MOVED;
                        selected_character = NULL;
                        break;
                    case MOVED:
                        // this happens if we make another character selection after moving
                        select_single(grid);
                        state = SELECTED;
                        break;
                    case ATTACKING:
                        // this happens if we select a character to attack 
                        new_x = X_RATIO * event.button.x / Util::SPRITE_SIZE;
                        new_y = Y_RATIO * event.button.y / Util::SPRITE_SIZE;

                        success = grid.attack(y, x, new_y, new_x, surface);
                        Util::update_screen(surface, screen);

                        if (success) state = ATTACKED;
                        selected_character = NULL;
                        break;
                    case ATTACKED:
                        // this happens if we make another character selection after attacking 
                        select_single(grid);
                        state = SELECTED;
                        break;
                    default: break; 
                }
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_z) {
                    switch (state) {
                        case SELECTED:
                            // this happens if we choose to move a selected character
                            if (selected_character != NULL && !selected_character->get_moved_this_turn()) {
                                success = grid.show_move_tiles(y, x, surface, true);
                                Util::update_screen(surface, screen);
                                if (success) state = MOVING;
                            }
                            break;
                        case ATTACKED:
                            // this happens if we choose to move after attacking TODO redundant? 
                            if (selected_character != NULL && !selected_character->get_moved_this_turn()) {
                                success = grid.show_move_tiles(y, x, surface, true);
                                Util::update_screen(surface, screen);
                                if (success) state = MOVING;
                            }
                            break;
                        default: break;
                    }
                } else if (event.key.keysym.sym == SDLK_x) {
                    switch (state) {
                        case SELECTED:
                            // this happens if we choose to attack with a selected character 
                            if (selected_character != NULL && !selected_character->get_attacked_this_turn()) {
                                success = grid.show_attack_tiles(y, x, surface, true);
                                Util::update_screen(surface, screen);
                                if (success) state = ATTACKING;
                            }
                            break;
                        case MOVED:
                            // this happens if we choose to attack after moving TODO redundant?
                            if (selected_character != NULL && !selected_character->get_attacked_this_turn()) {
                                success = grid.show_attack_tiles(y, x, surface, true);
                                Util::update_screen(surface, screen);
                                if (success) state = ATTACKING;
                            }
                            break;
                        default: break;
                    }
                } else if (event.key.keysym.sym == SDLK_ESCAPE) {
                    switch (state) {
                        case MOVING:
                            // this happens if we cancel a move
                            success = grid.show_move_tiles(y, x, surface, false);
                            Util::update_screen(surface, screen);

                            grid.get(y, x)->set_selected(true);
                            grid.draw_grid(surface);
                            if (success) state = SELECTED;
                            break;
                        case ATTACKING:
                            // this happens if we cancel an attack
                            success = grid.show_attack_tiles(y, x, surface, false);
                            Util::update_screen(surface, screen);

                            grid.get(y, x)->set_selected(true);
                            grid.draw_grid(surface);
                            if (success) state = SELECTED;
                            break;
                    }
                }
            } else if (event.key.keysym.sym == SDLK_v) {
                // this happens if we choose to end the turn
                grid.new_turn();
                grid.draw_grid(surface);
            }
        }
        draw_sidebar(grid);
    }

    clean_up();
    SDL_Quit();

    return 0;
}
