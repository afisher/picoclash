#include "Header.h"

void MovingState::execute(SDL_Event event, SDL_Surface* surface) {
    Tile* selected_tile = StateMachine::get_selected_tile();
    Character* selected_character = NULL;
    if (selected_tile != NULL) {
        selected_character = selected_tile->get_character();
    }

    if (event.type == SDL_MOUSEBUTTONDOWN &&
        event.button.button == SDL_BUTTON_LEFT) {
        
        int x = event.motion.x / (Constants::X_RATIO * Constants::SPRITE_SIZE); 
        int y = event.motion.y / (Constants::Y_RATIO * Constants::SPRITE_SIZE); 

        bool success = Grid::move(selected_tile->get_x(), selected_tile->get_y(), x, y, surface);

        if (success) {
            selected_tile->set_selected(false);
            Grid::select_move_tiles(selected_tile->get_x(), selected_tile->get_y(), selected_character->get_mobility(), false);

            Grid::draw_grid(surface);

            StateMachine::set_selected_tile(Grid::get(selected_character->get_x(),
                                                      selected_character->get_y()));
            StateMachine::get_selected_tile()->set_selected(true);
            StateMachine::get_selected_tile()->set_move_on(false);
            Grid::draw_grid(surface);

            StateMachine::set_previous_state(this);
            StateMachine::set_current_state(new SelectedState());
        }
    } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
        // this happens if we cancel
        bool success = Grid::show_move_tiles(selected_tile->get_x(),
                                             selected_tile->get_y(), surface, false);

        Grid::get(selected_tile->get_x(), selected_tile->get_y())->set_selected(true);
        Grid::draw_grid(surface);
        if (success) {
            StateMachine::set_previous_state(this);
            StateMachine::set_current_state(new SelectedState());
        }
    }
}

std::string MovingState::sidebar_tip() {
    return "Select a square";
}

