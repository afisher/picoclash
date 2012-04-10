#pragma once

#include "SDL/SDL.h"
#include "State.h"
#include <string>

class SelectedState : public State {
    public:
        SelectedState(StateMachine* s);

        State execute(SDL_Event event);
        std::string sidebar_tip();
};
