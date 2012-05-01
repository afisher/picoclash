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
            // end the turn
            Grid::new_turn();
            Grid::draw_grid(surface);
            if (Grid::get_current_player() == 2) {
                Grid::draw_sidebar(surface);
                Grid::play_ai_turn(surface, screen);
                Grid::new_turn();
                Grid::draw_grid(surface);
            }
        }
    } else if (event.type == SDL_MOUSEMOTION) {
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

        int x = event.motion.x / (Constants::X_RATIO * Constants::SPRITE_SIZE); 
        int y = event.motion.y / (Constants::Y_RATIO * Constants::SPRITE_SIZE); 

        if (x >= 0 && y >= 0 && x < Constants::GRID_WIDTH && y < Constants::GRID_HEIGHT) { 
            StateMachine::set_inspected_tile(Grid::get(x, y));
            inspected_tile = StateMachine::get_inspected_tile();

            // highlight the tile
            inspected_tile->set_selected(true);
            Grid::draw_tile(inspected_tile, surface);
        }
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
