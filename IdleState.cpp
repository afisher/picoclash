#include "Header.h"

void IdleState::execute(SDL_Event event, SDL_Surface* surface) {
    if (event.type == SDL_MOUSEMOTION) {
        Tile* selected_tile = StateMachine::get_selected_tile();
        if (selected_tile != NULL) {
            selected_tile->set_selected(false);
        }
        
        int x = event.motion.x / (Constants::X_RATIO * Constants::SPRITE_SIZE); 
        int y = event.motion.y / (Constants::Y_RATIO * Constants::SPRITE_SIZE); 

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
