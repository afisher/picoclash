#pragma once

#include "SDL/SDL.h"
#include "State.h"
#include <string>

class HealingState : public State {
    public:
        HealingState(StateMachine* s);

        State execute(SDL_Event event);
        std::string sidebar_tip();
};
