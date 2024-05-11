#ifndef __FLYINGENEMYMOVESTATE_H__
#define __FLYINGENEMYMOVESTATE_H__

#include "Gameplay/Entities/Enemies/FlyingEnemy.h"
#include "Utils/State.h"
#include "Utils/SString.h"
#include "Utils/Defs.h"
#include "Gameplay/Entities/Player.h"

class Player;
class FlyingEnemyMoveState : public State<FlyingEnemy> {
private:
    FlyingEnemy* flyingenemy;
    Player* player;

public:
    FlyingEnemyMoveState(SString name) : State(name) {}
    inline void Enter() override
    {
        LOG("FlyingEnemyMoveState Enter()");

        flyingenemy = StateMachineReference->owner;
    }
    inline void Update(float dt) override
    {
        LOG("FlyingEnemyMoveState Update()");

        player = app->entityManager->GetPlayerEntity();

        flyingenemy->pathfindingMovement(dt);
        if (PIXEL_TO_METERS(player->position.DistanceTo(flyingenemy->position)) < 3.0f) {
            if (flyingenemy->attackTimer.ReadSec() >= 2)
            {
                StateMachineReference->ChangeState("attack");
            }
        }
        else if ((PIXEL_TO_METERS(player->position.DistanceTo(flyingenemy->position)) > 10.0f)) {
            //flyingenemy->moveToSpawnPoint();
            flyingenemy->StopMoving();
            StateMachineReference->ChangeState("idle");
        }
    }
    inline void Exit() override
    {
        LOG("FlyingEnemyMoveState Exit()");
    }
};
#endif // __FLYINGENEMYMOVESTATE_H__