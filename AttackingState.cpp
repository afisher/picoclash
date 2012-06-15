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

void AttackingState::execute(SDL_Event event, SDL_Surface* surface) {
    Tile* selected_tile = StateMachine::get_selected_tile();
    Character* selected_character = NULL;
    if (selected_tile != NULL) {
        selected_character = selected_tile->get_character();
    }

    if (event.type == SDL_MOUSEBUTTONDOWN &&
        event.button.button == SDL_BUTTON_LEFT) {
        
        int x = event.motion.x / (Util::X_RATIO * Constants::SPRITE_SIZE); 
        int y = event.motion.y / (Util::Y_RATIO * Constants::SPRITE_SIZE); 

        bool success = Grid::attack(selected_tile->get_x(), selected_tile->get_y(), x, y, surface);

        if (success) {
            StateMachine::set_selected_tile(Grid::get(selected_character->get_x(),
                                                      selected_character->get_y()));
            StateMachine::get_selected_tile()->set_selected(true);
            Grid::draw_grid(surface);

            StateMachine::set_previous_state(this);
            StateMachine::set_current_state(new SelectedState());
        }
    } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
        // this happens if we cancel
        bool success = Grid::show_attack_tiles(selected_tile->get_x(),
                                               selected_tile->get_y(), surface, false);

        Grid::get(selected_tile->get_x(), selected_tile->get_y())->set_selected(true);
        Grid::draw_grid(surface);
        if (success) {
            StateMachine::set_previous_state(this);
            StateMachine::set_current_state(new SelectedState());
        }
    }
}

std::string AttackingState::sidebar_tip() {
    return "Select a victim";
}


