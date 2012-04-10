#pragma once

#include "SDL/SDL.h"
#include "State.h"
#include <string>

class AttackingState : public State {
    public:
        AttackingState(StateMachine* s);

        State execute(SDL_Event event);
        std::string sidebar_tip();
};

