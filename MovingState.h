#pragma once

#include "SDL/SDL.h"
#include "State.h"
#include <string>

class MovingState : public State {
    public:
        MovingState(StateMachine* s);

        State execute(SDL_Event event);
        std::string sidebar_tip();
};
