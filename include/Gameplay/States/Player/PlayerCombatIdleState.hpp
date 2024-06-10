#ifndef __PLAYERCOMBATIDLESTATE_H__
#define __PLAYERCOMBATIDLESTATE_H__

#include "Gameplay/Entities/Player.h"
#include <Core/Input.h>
#include <Core/App.h>
#include "Utils/State.h"
#include "Utils/SString.h"
#include "Utils/Defs.h"

class PlayerCombatIdleState : public State<Player> {
private:
    Player* player;

public:
    PlayerCombatIdleState(SString name) : State(name) {}
    inline void Enter() override
    {
        player = StateMachineReference->owner;
    }
    inline void Update(float dt) override
    {
        if (app->input->GetMouseButtonDown(1)) {

            StateMachineReference->ChangeState("attack");
        }
        else if (app->input->GetMouseButtonDown(3))
        {
            StateMachineReference->ChangeState("block");
        }

        if ( player->mana < 100.0f ) {
            player->mana += player->manaRegeneration * (dt / 1000);
        }
    }
    inline void Exit() override
    {
    }
};
#endif // __PlayerCombatIdleState_H__