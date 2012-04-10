#pragma once

#include "State.h"
#include <string>

class IdleState : public State {
    public:
        IdleState(StateMachine* s);

        State execute(SDL_Event event);
        std::string sidebar_tip();
};
