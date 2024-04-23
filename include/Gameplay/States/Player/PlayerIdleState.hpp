#ifndef __PLAYERIDLESTATE_H__
#define __PLAYERIDLESTATE_H__

#include "Core/App.h"
#include "Gameplay/Entities/Player.h"
#include "Utils/State.h"
#include "Utils/SString.h"
#include "Utils/Defs.h"

class PlayerIdleState : public State<Player> {
private:
    Player* player;

public:
    PlayerIdleState(SString name) : State(name) {}
    inline void Enter() override
    {
        player = StateMachineReference->owner;
    }
    inline void Update(float dt) override
    {

        GamePad& pad = app->input->pads[0];

        if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT or
            app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT or
            app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT or
            app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT or
            pad.l_x != 0.0f or
            pad.l_y != 0.0f) {

            StateMachineReference->ChangeState("move");
        }
    }
    inline void Exit() override
    {
        LOG("PlayerIdleState Exit()");
    }
};
#endif // __PLAYERIDLESTATE_H__