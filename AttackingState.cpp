#include "Header.h"

void AttackingState::execute(SDL_Event event, SDL_Surface* surface) {
    Tile* selected_tile = StateMachine::selected_tile;
    Character* selected_character = selected_tile->get_character();

    if (event.type == SDL_MOUSEBUTTONDOWN &&
        event.button.button == SDL_BUTTON_LEFT) {
        
        int x = Constants::X_RATIO * event.button.x / Constants::SPRITE_SIZE;
        int y = Constants::Y_RATIO * event.button.y / Constants::SPRITE_SIZE;

        bool success = Grid::attack(selected_tile->get_x(), selected_tile->get_y(), x, y, surface);

        if (success) {
            selected_tile = Grid::get(selected_character->get_x(), selected_character->get_y());
            selected_tile->set_selected(true);
            Grid::draw_grid(surface);

            StateMachine::previous_state = this;
            StateMachine::current_state = new SelectedState();
        }
    }
}

std::string AttackingState::sidebar_tip() {
    return "Select a victim";
}


