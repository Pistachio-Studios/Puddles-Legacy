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
        b2Vec2 playerPos = player->pbody->body->GetPosition();
        b2Vec2 swordPos = { playerPos.x + (float)cos(player->lookingAngle), playerPos.y + (float)sin(player->lookingAngle) };
        player->sword->body->SetTransform(swordPos, player->lookingAngle);

        if (app->input->GetMouseButtonDown(1)) {

            StateMachineReference->ChangeState("attack");
        }
    }
    inline void Exit() override
    {
    }
};
#endif // __PlayerCombatIdleState_H__