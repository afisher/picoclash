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

State* current_state = NULL;
State* previous_state = NULL;

Tile* selected_tile = NULL;
Tile* inspected_tile = NULL;

void StateMachine::init() {
    current_state = new SelectedState();
    previous_state = new IdleState();

    selected_tile  = NULL;
    inspected_tile = NULL;
}

void StateMachine::execute(SDL_Event event, SDL_Surface* surface, SDL_Surface* screen) {
    current_state->execute(event, surface);

    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_LSHIFT ||
            event.key.keysym.sym == SDLK_RSHIFT) {

            Grid::toggle_show_lines(surface);
        } else if (event.key.keysym.sym == SDLK_v) {
            // end the turn if it is within a player's control
            if (Grid::get_game_type() == Constants::P_V_P ||
               (Grid::get_game_type() == Constants::P_V_CPU &&
               Grid::get_current_player() == 1)) { 

                Grid::new_turn();
                StateMachine::get_inspected_tile()->set_selected(false);
                StateMachine::get_selected_tile()->set_selected(false);
                Grid::draw_grid(surface);
            }
        }
    } else if (event.type == SDL_MOUSEMOTION) {
        if (Grid::get_current_player() == 2 && Grid::get_game_type() != Constants::P_V_P) return;

        Tile* inspected_tile = StateMachine::get_inspected_tile();
        Character* inspected_character = NULL;
        if (inspected_tile != NULL) {
            inspected_character = inspected_tile->get_character();
        }

        if (inspected_tile != NULL && (inspected_tile->get_x() != selected_tile->get_x()
                                   || inspected_tile->get_y() != selected_tile->get_y())) {
            inspected_tile->set_selected(false);
            Grid::draw_tile(inspected_tile, surface);
        }

        int x = event.motion.x / (Util::X_RATIO * Constants::SPRITE_SIZE); 
        int y = event.motion.y / (Util::Y_RATIO * Constants::SPRITE_SIZE); 

        if (x >= 0 && y >= 0 && x < Constants::GRID_WIDTH && y < Constants::GRID_HEIGHT) { 
            StateMachine::set_inspected_tile(Grid::get(x, y));
            inspected_tile = StateMachine::get_inspected_tile();

            // highlight the tile
            inspected_tile->set_selected(true);
            Grid::draw_tile(inspected_tile, surface);
        }

        Grid::draw_sidebar(surface);
    }

}

void StateMachine::set_current_state(State* s)  { current_state = s;  }
void StateMachine::set_previous_state(State* s) { previous_state = s; }

void StateMachine::set_selected_tile(Tile* t)  { selected_tile = t;  }
void StateMachine::set_inspected_tile(Tile* t) { inspected_tile = t; }

State* StateMachine::get_current_state() { return current_state; }
State* StateMachine::get_previous_state() { return previous_state; }

Tile* StateMachine::get_selected_tile() { return selected_tile; }
Tile* StateMachine::get_inspected_tile() { return inspected_tile; }
