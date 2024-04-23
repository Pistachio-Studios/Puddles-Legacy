#ifndef __CENTIPIDEENEMYIDLESTATE_H__
#define __CENTIPIDEENEMYIDLESTATE_H__

#include "Gameplay/Entities/Enemies/CentipideEnemy.h"
#include "Utils/State.h"
#include "Utils/SString.h"
#include "Utils/Defs.h"

class Player;
class CentipideEnemyIdleState : public State<CentipideEnemy> {
private:
    CentipideEnemy* CentipideEnemy;
    Player* player;

public:
    CentipideEnemyIdleState(SString name) : State(name) {}
    inline void Enter() override
    {
        LOG("CentipideEnemyIdleState Enter()");

        CentipideEnemy = StateMachineReference->owner;
    }
    inline void Update(float dt) override
    {
        LOG("CentipideEnemyIdleState Update()");

        player = app->entityManager->GetPlayerEntity();

        if (PIXEL_TO_METERS(player->position.DistanceTo(CentipideEnemy->position)) < 3.0f)
        {
            StateMachineReference->ChangeState("move");
            // AUDIO DONE dog idle
            //app->audio->PlayFx(dogEnemy->dogBark);
        }
    }
    inline void Exit() override
    {
        LOG("CentipideEnemyIdleState Exit()");
    }
};
#endif // __CENTIPIDEENEMYIDLESTATE_H__