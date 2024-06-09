#ifndef __MINISPIDERIDLESTATE_H__
#define __MINISPIDERIDLESTATE_H__

#include "Gameplay/Entities/Enemies/MiniSpider.h"
#include "Utils/State.h"
#include "Utils/SString.h"
#include "Utils/Defs.h"

class Player;
class MiniSpiderIdleState : public State<MiniSpider> {
private:
    MiniSpider* minispider;
    Player* player;

public:
    MiniSpiderIdleState(SString name) : State(name) {}
    inline void Enter() override
    {
        LOG("MiniSpiderIdleState Enter()");

        minispider = StateMachineReference->owner;
    }
    inline void Update(float dt) override
    {
        LOG("EnemyBossIdleState Update()");


        player = app->entityManager->GetPlayerEntity();

        //Animation
        app->render->DrawTexture(minispider->spiderIdle.texture, minispider->position.x - 60, minispider->position.y - 60, &minispider->spiderIdle.GetCurrentFrame(), 1.0f, minispider->pbody->body->GetAngle() * RADTODEG, 0.5f, 1.0f, minispider->flip);
        minispider->spiderIdle.Update(dt);

        if (minispider->isTouchingPlayer) {
            if (minispider->attackTimer.ReadSec() >= 1)
            {
                minispider->spiderAttack.Reset();
                StateMachineReference->ChangeState("attack");
            }
        }

        if (PIXEL_TO_METERS(player->position.DistanceTo(minispider->position)) < 7.0f && PIXEL_TO_METERS(player->position.DistanceTo(minispider->position)) > 2.0f)
        {
            StateMachineReference->ChangeState("move");
            // AUDIO DONE dog idle
            //app->audio->PlayFx(dogEnemy->dogBark);
        }
    }
    inline void Exit() override
    {
        LOG("MiniSpiderIdleState Exit()");
    }
};
#endif // __MINISPIDERIDLESTATE_H__