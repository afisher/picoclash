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
            Grid::new_turn();
            Grid::draw_grid(surface);
            if (Grid::get_current_player() == 2) {
                //SRPG::draw_sidebar();
                Grid::play_ai_turn(surface, screen);
                Grid::new_turn();
                Grid::draw_grid(surface);
            }
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
