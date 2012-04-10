#pragma once

#include "SDL/SDL.h"
#include <string>

class StateMachine;
class SRPG;

class State {
    protected:
        StateMachine* state_machine;
    public:
        State(StateMachine* s);

        // does appropriate stuff based on the event, and returns new state
        virtual State execute(SDL_Event event) {};
        virtual std::string sidebar_tip() { return ""; };
};
