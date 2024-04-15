#ifndef __PLAYERCOMBATATTACKSTATE_H__
#define __PLAYERCOMBATATTACKSTATE_H__

#include "Gameplay/Entities/Player.h"
#include <Core/Input.h>
#include <Core/App.h>
#include "Utils/State.h"
#include "Utils/SString.h"
#include "Utils/Defs.h"

class PlayerCombatAttackState : public State<Player> {
private:
    Player* player;

    float attackRange;
    float attackValue;
    float playerLookingAngle;
    float attackSpeed;

public:
    PlayerCombatAttackState(SString name) : State(name) {}
    inline void Enter() override
    {
        LOG("PlayerCombatAttackState Enter()");

        player = StateMachineReference->owner;

        attackRange = 150;
        attackValue = -attackRange / 2;
        playerLookingAngle = player->lookingAngle;
        attackSpeed = 500;
    }
    inline void Update(float dt) override
    {
        if (attackValue < attackRange / 2)
        {
            b2Vec2 playerPos = player->pbody->body->GetPosition();
            b2Vec2 swordPos = { playerPos.x + (float)cos(playerLookingAngle + attackValue * DEGTORAD), playerPos.y + (float)sin(playerLookingAngle + attackValue * DEGTORAD) };
            player->sword->body->SetTransform(swordPos, playerLookingAngle + attackValue * DEGTORAD);
            attackValue += dt / 1000 * attackSpeed;
        }
        else
        {
            StateMachineReference->ChangeState("idle");
        }
    }
    inline void Exit() override
    {
    }
};
#endif // __PlayerCombatAttackState_H__