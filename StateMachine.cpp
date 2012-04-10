#include "Header.h"

void StateMachine::init() {
    current_state = new IdleState();
    previous_state = NULL;

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
