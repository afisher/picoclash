#include "Header.h"

void SelectedState::execute(SDL_Event event, SDL_Surface* surface) {
    Tile* selected_tile = StateMachine::get_selected_tile();
    Character* selected_character = NULL;
    if (selected_tile != NULL) {
        selected_character = selected_tile->get_character();
    }

    if (event.type == SDL_MOUSEMOTION) {
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
            Grid::draw_sidebar(surface);

            StateMachine::set_previous_state(this);
            StateMachine::set_current_state(new SelectedState());
        }
    } else if (event.type == SDL_KEYDOWN) {
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
