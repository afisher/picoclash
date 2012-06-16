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

void SelectedState::execute(SDL_Event event, SDL_Surface* surface) {
    Tile* selected_tile = StateMachine::get_selected_tile();
    Character* selected_character = NULL;
    if (selected_tile != NULL) {
        selected_character = selected_tile->get_character();
        Grid::draw_sidebar(surface);
    }

    if (event.type == SDL_MOUSEMOTION) {
        if (Grid::get_current_player() == 2 && Grid::get_game_type() != Constants::P_V_P) return;
        int x = event.motion.x / (Util::X_RATIO * Constants::SPRITE_SIZE); 
        int y = event.motion.y / (Util::Y_RATIO * Constants::SPRITE_SIZE); 

        if (selected_tile != NULL) {
            if (selected_tile->get_x() == x && selected_tile->get_y() == y) return;

            selected_tile->set_selected(false);
            Grid::draw_tile(selected_tile, surface);
        }
        

        if (x >= 0 && y >= 0 && x < Constants::GRID_WIDTH && y < Constants::GRID_HEIGHT) { 
            StateMachine::set_selected_tile(Grid::get(x, y));
            selected_tile = StateMachine::get_selected_tile();

            // highlight the tile
            selected_tile->set_selected(true);
            Grid::draw_tile(selected_tile, surface);

            StateMachine::set_previous_state(this);
            StateMachine::set_current_state(new SelectedState());
        }
    } else if (event.type == SDL_KEYDOWN) {

        if (selected_character != NULL) {
            if (selected_character->get_player() == 1 &&
                Grid::get_game_type() == Constants::CPU_V_CPU) return;

            if (selected_character->get_player() == 2 &&
                Grid::get_game_type() != Constants::P_V_P) return;
        }

        switch (event.key.keysym.sym) {
            case SDLK_z:
                if (selected_character != NULL && !selected_character->get_moved_this_turn()) {
                    Grid::show_move_tiles(selected_character->get_x(),
                                                         selected_character->get_y(), surface, true);

                    StateMachine::set_previous_state(this);
                    StateMachine::set_current_state(new MovingState());
                }
                break;

            case SDLK_x:
                if (selected_character != NULL && !selected_character->get_attacked_this_turn()) {
                    Grid::show_attack_tiles(selected_character->get_x(),
                                                           selected_character->get_y(), surface, true);

                    StateMachine::set_previous_state(this);
                    StateMachine::set_current_state(new AttackingState());
                }
                break;

            case SDLK_c:
                if (selected_character != NULL && selected_character->can_heal()
                        && !((Healer*)selected_character)->get_healed_this_turn()) {
                    // just use the attack range for now
                    Grid::show_heal_tiles(selected_character->get_x(),
                                                         selected_character->get_y(), surface, true);

                    StateMachine::set_previous_state(this);
                    StateMachine::set_current_state(new HealingState());
                }
                break;

            default: break;
        }
    }
}

std::string SelectedState::sidebar_tip() {
    return "Choose an action";
}

