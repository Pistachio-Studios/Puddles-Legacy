#ifndef __PLAYERCOMBATATTACKSTATE_H__
#define __PLAYERCOMBATATTACKSTATE_H__

#include "Gameplay/Entities/Player.h"
#include "Gameplay/Entities/Sword.h"
#include "Gameplay/Entities/Staff.h"
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
    float radius;

    Sword* swordEntity = nullptr;

    Staff* staffEntity = nullptr;

public:
    PlayerCombatAttackState(SString name) : State(name) {}
    inline void Enter() override
    {


        player = StateMachineReference->owner;

        if(player->currentClass == PlayerClass::KNIGHT)
        {
            swordEntity = (Sword*)app->entityManager->CreateEntity(EntityType::SWORD);
            swordEntity->Start();

            attackRange = 150;
            attackValue = -attackRange / 2;
            playerLookingAngle = player->lookingAngle;
            attackSpeed = 500;
            radius = 2.5;
        }
        else if(player->currentClass == PlayerClass::WIZARD)
        {
            staffEntity = (Staff*)app->entityManager->CreateEntity(EntityType::STAFF);
            staffEntity->Start();
            staffEntity->ThrowSpell();
        }
    }
    inline void Update(float dt) override
    {
        if(player->currentClass == PlayerClass::KNIGHT)
        {
            if (attackValue < attackRange / 2)
            {
                b2Vec2 playerPos = player->pbody->body->GetPosition();
                b2Vec2 swordPos = { playerPos.x + (float)cos(playerLookingAngle + attackValue * DEGTORAD) * radius, playerPos.y + (float)sin(playerLookingAngle + attackValue * DEGTORAD) * radius };
                swordEntity->pbody->body->SetTransform(swordPos, playerLookingAngle + attackValue * DEGTORAD);
                attackValue += dt / 1000 * attackSpeed;
            }
            else
            {
                StateMachineReference->ChangeState("idle");
            }
        }
        else if(player->currentClass == PlayerClass::WIZARD)
        {
            b2Vec2 playerPos = player->pbody->body->GetPosition();
            b2Vec2 staffPos = { playerPos.x + (float)cos(player->lookingAngle) * 2, playerPos.y + (float)sin(player->lookingAngle) * 2 };
            staffEntity->pbody->body->SetTransform(staffPos, player->lookingAngle);
        }
    }
    inline void Exit() override
    {
        app->entityManager->DestroyEntity(swordEntity);
    }
};
#endif // __PlayerCombatAttackState_H__