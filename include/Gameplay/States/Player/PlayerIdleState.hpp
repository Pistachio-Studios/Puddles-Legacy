#ifndef __PLAYERIDLESTATE_H__
#define __PLAYERIDLESTATE_H__

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

        if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT or
            app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT or
            app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT or
            app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {

            StateMachineReference->ChangeState("move");
        }
    }
    inline void Exit() override
    {
    }
};
#endif // __PLAYERIDLESTATE_H__