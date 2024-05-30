#ifndef __CENTIPIDEENEMYMOVESTATE_H__
#define __CENTIPIDEENEMYMOVESTATE_H__

#include "Gameplay/Entities/Enemies/CentipideEnemy.h"
#include "Utils/State.h"
#include "Utils/SString.h"
#include "Utils/Defs.h"
#include "Gameplay/Entities/Player.h"

class Player;
class CentipideEnemyMoveState : public State<CentipideEnemy> {
private:
    CentipideEnemy* CentipideEnemy;
    Player* player;

public:
    CentipideEnemyMoveState(SString name) : State(name) {}
    inline void Enter() override
    {


        CentipideEnemy = StateMachineReference->owner;
    }
    inline void Update(float dt) override
    {


        player = app->entityManager->GetPlayerEntity();

        CentipideEnemy->pathfindingMovement(dt);
        if (PIXEL_TO_METERS(player->position.DistanceTo(CentipideEnemy->position)) < 100.0f) {
            if (CentipideEnemy->attackTimer.ReadSec() >= 2)
            {
                StateMachineReference->ChangeState("attack");
            }
        }
        else if ((PIXEL_TO_METERS(player->position.DistanceTo(CentipideEnemy->position)) > 10.0f)) {
            //CentipideEnemy->moveToSpawnPoint();
            CentipideEnemy->StopMoving();
            StateMachineReference->ChangeState("idle");
        }
    }
    inline void Exit() override
    {

    }
};
#endif // __CENTIPIDEENEMYMOVESTATE_H__