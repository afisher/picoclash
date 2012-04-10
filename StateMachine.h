#pragma once

#include "SDL/SDL.h"
#include "State.h"
#include "Tile.h"

class StateMachine {
    public:
        State* current_state;
        State* previous_state;

        Tile* selected_tile;
        Tile* inspected_tile;

        void execute(SDL_Event event);

        void set_current_state(State s);
        void set_previous_state(State s);

        void set_selected_tile(Tile* t);
        void set_inspected_tile(Tile* t);
};
