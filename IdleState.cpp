#include "Header.h"

void IdleState::execute(SDL_Event event, SDL_Surface* surface) {
    if (event.type == SDL_MOUSEMOTION) {
        Tile* selected_tile = StateMachine::selected_tile;
        if (selected_tile != NULL) {
            selected_tile->set_selected(false);
        }
        
        int x = Constants::X_RATIO * event.motion.x / Constants::SPRITE_SIZE; 
        int y = Constants::Y_RATIO * event.motion.y / Constants::SPRITE_SIZE; 

        if (x < Constants::GRID_WIDTH && y < Constants::GRID_HEIGHT) { 
            selected_tile = Grid::get(x, y);

            // highlight the tile
            selected_tile->set_selected(true);
            Grid::draw_grid(surface);

            //SRPG::draw_sidebar();

            StateMachine::previous_state = this;
            StateMachine::current_state = new SelectedState();
        }
    }
}

std::string IdleState::sidebar_tip() {
    return "Select a character";
}
