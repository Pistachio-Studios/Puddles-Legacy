#ifndef __PLAYERCOMBATBLOCKSTATE_H__
#define __PLAYERCOMBATBLOCKSTATE_H__

#include "Gameplay/Entities/Player.h"
#include "Gameplay/Entities/Shield.h"
#include <Core/Input.h>
#include <Core/App.h>
#include "Utils/State.h"
#include "Utils/SString.h"
#include "Utils/Defs.h"

class PlayerCombatBlockState : public State<Player> {
private:
    Player* player;

    Shield* shieldEntity = nullptr;

public:
    PlayerCombatBlockState(SString name) : State(name) {}
    inline void Enter() override
    {
        LOG("PlayerCombatBlockState Enter()");

        player = StateMachineReference->owner;
        shieldEntity = (Shield*)app->entityManager->CreateEntity(EntityType::SHIELD);
        shieldEntity->Start();
    }
    inline void Update(float dt) override
    {
        b2Vec2 playerPos = player->pbody->body->GetPosition();
        b2Vec2 swordPos = { playerPos.x + (float)cos(player->lookingAngle), playerPos.y + (float)sin(player->lookingAngle) };
        shieldEntity->pbody->body->SetTransform(swordPos, player->lookingAngle);
        if (!app->input->GetMouseButtonDown(3))
        {
            StateMachineReference->ChangeState("idle");
        }
    }
    inline void Exit() override
    {
        app->entityManager->DestroyEntity(shieldEntity);
    }
};
#endif // __PlayerCombatAttackState_H__