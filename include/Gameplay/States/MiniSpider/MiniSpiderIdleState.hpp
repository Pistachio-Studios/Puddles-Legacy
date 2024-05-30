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

        //Animation
        app->render->DrawTexture(minispider->spiderIdle.texture, minispider->position.x - 100, minispider->position.y - 150, &minispider->spiderIdle.GetCurrentFrame());
        minispider->spiderIdle.Update(dt);

        player = app->entityManager->GetPlayerEntity();

        if (PIXEL_TO_METERS(player->position.DistanceTo(minispider->position)) < 3.0f)
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