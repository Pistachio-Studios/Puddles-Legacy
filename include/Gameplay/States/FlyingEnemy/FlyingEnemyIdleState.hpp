#ifndef __FLYINGENEMYIDLESTATE_H__
#define __FLYINGENEMYIDLESTATE_H__

#include "Gameplay/Entities/Enemies/FlyingEnemy.h"
#include "Utils/State.h"
#include "Utils/SString.h"
#include "Utils/Defs.h"

class Player;
class FlyingEnemyIdleState : public State<FlyingEnemy> {
private:
    FlyingEnemy* flyingenemy;
    Player* player;

public:
    FlyingEnemyIdleState(SString name) : State(name) {}
    inline void Enter() override
    {


        flyingenemy = StateMachineReference->owner;
    }
    inline void Update(float dt) override
    {


        player = app->entityManager->GetPlayerEntity();

        if (PIXEL_TO_METERS(player->position.DistanceTo(flyingenemy->position)) < 3.0f)
        {
            StateMachineReference->ChangeState("move");
            // AUDIO DONE dog idle
            //app->audio->PlayFx(dogEnemy->dogBark);
        }
    }
    inline void Exit() override
    {

    }
};
#endif // __FLYINGENEMYIDLESTATE_H__