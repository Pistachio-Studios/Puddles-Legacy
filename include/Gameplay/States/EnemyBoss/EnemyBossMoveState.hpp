#ifndef __ENEMYBOSSMOVESTATE_H__
#define __ENEMYBOSSMOVESTATE_H__

#include "Gameplay/Entities/Enemies/EnemyBoss.h"
#include "Utils/State.h"
#include "Utils/SString.h"
#include "Utils/Defs.h"

class EnemyBossMoveState : public State<EnemyBoss> {
private:
    EnemyBoss* enemyboss;

public:
    EnemyBossMoveState(SString name) : State(name) {}
    inline void Enter() override
    {
        LOG("EnemyBossMoveState Enter()");

        enemyboss = StateMachineReference->owner;
    }
    inline void Update(float dt) override
    {
        LOG("EnemyBossMoveState Update()");

        //enemyboss->pathfindingMovement(dt);
        //if (PIXEL_TO_METERS(app->scene->player->position.DistanceTo(enemyboss->position)) < 1.0f) {
        //    if (enemyboss->attackTimer.ReadSec() >= 2)
        //    {
        //        StateMachineReference->ChangeState("attack");
        //    }
        //}
        //else if ((PIXEL_TO_METERS(app->scene->player->position.DistanceTo(enemyboss->position)) > 5.0f)) {
        //    enemyboss->moveToSpawnPoint();
        //    StateMachineReference->ChangeState("idle");
        //}
    }
    inline void Exit() override
    {
        LOG("EnemyBossMoveState Exit()");
    }
};
#endif // __ENEMYBOSSMOVESTATE_H__