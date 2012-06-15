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

void IdleState::execute(SDL_Event event, SDL_Surface* surface) {
    if (event.type == SDL_MOUSEMOTION) {
        Tile* selected_tile = StateMachine::get_selected_tile();
        if (selected_tile != NULL) {
            selected_tile->set_selected(false);
        }
        
        int x = event.motion.x / (Util::X_RATIO * Constants::SPRITE_SIZE); 
        int y = event.motion.y / (Util::Y_RATIO * Constants::SPRITE_SIZE); 

        if (x >= 0 && y >= 0 && x < Constants::GRID_WIDTH && y < Constants::GRID_HEIGHT) { 
            StateMachine::set_selected_tile(Grid::get(x, y));
            selected_tile = StateMachine::get_selected_tile();

            // highlight the tile
            selected_tile->set_selected(true);
            Grid::draw_grid(surface);

            //SRPG::draw_sidebar();

            StateMachine::set_previous_state(this);
            StateMachine::set_current_state(new SelectedState());
        }
    }
}

std::string IdleState::sidebar_tip() {
    return "Select a character";
}
