#ifndef __ENEMYBOSSMOVESTATE_H__
#define __ENEMYBOSSMOVESTATE_H__

#include "Gameplay/Entities/Enemies/EnemyBoss.h"
#include "Utils/State.h"
#include "Utils/SString.h"
#include "Utils/Defs.h"
#include "Gameplay/Entities/Player.h"

class Player;
class EnemyBossMoveState : public State<EnemyBoss> {
private:
    EnemyBoss* enemyboss;
    Player* player;

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

        //Animation
        app->render->DrawTexture(enemyboss->bossMove.texture, enemyboss->position.x - 120, enemyboss->position.y - 230, &enemyboss->bossMove.GetCurrentFrame());
        enemyboss->bossMove.Update(dt);

        player = app->entityManager->GetPlayerEntity();

        enemyboss->pathfindingMovement(dt);
        if (PIXEL_TO_METERS(player->position.DistanceTo(enemyboss->position)) < 3.0f) {
            if (enemyboss->attackTimer.ReadSec() >= 2)
            {
                StateMachineReference->ChangeState("attack");
            }
        }
        else if ((PIXEL_TO_METERS(player->position.DistanceTo(enemyboss->position)) > 10.0f)) {
            //enemyboss->moveToSpawnPoint();
            enemyboss->StopMoving();
            StateMachineReference->ChangeState("idle");
        }
    }
    inline void Exit() override
    {
        LOG("EnemyBossMoveState Exit()");
    }
};
#endif // __ENEMYBOSSMOVESTATE_H__