#ifndef __MINISPIDERATTACKSTATE_H__
#define __MINISPIDERATTACKSTATE_H__

#include "Gameplay/Entities/Enemies/MiniSpider.h"
#include "Utils/State.h"
#include "Utils/SString.h"
#include "Utils/Defs.h"
#include "Gameplay/Entities/Player.h"

class Player;
class MiniSpiderAttackState : public State<MiniSpider> {
private:
    MiniSpider* minispider;
    Player* player;

public:
    MiniSpiderAttackState(SString name) : State(name) {}
    inline void Enter() override
    {
        LOG("MiniSpiderAttackState Enter()");

        minispider = StateMachineReference->owner;
    }
    inline void Update(float dt) override
    {
        LOG("MiniSpiderAttackState Update()");

        player = app->entityManager->GetPlayerEntity();

        //Animation
        app->render->DrawTexture(minispider->spiderAttack.texture, minispider->position.x - 100, minispider->position.y - 150, &minispider->spiderAttack.GetCurrentFrame(), 1.0f, minispider->pbody->body->GetAngle() * RADTODEG, minispider->flip);
        minispider->spiderAttack.Update(dt);

        if (minispider->spiderAttack.GetCurrentFrameCount() == 5 && PIXEL_TO_METERS(player->position.DistanceTo(minispider->position)) < 2.0f) {
            player->vida -= minispider->dano;
        }

        if (minispider->spiderAttack.GetCurrentFrameCount()>=9) {
            minispider->attackTimer.Start();
            StateMachineReference->ChangeState("idle");
        }

    }
    inline void Exit() override
    {
        LOG("MiniSpiderAttackState Exit()");
    }
};
#endif // __MINISPIDERATTACKSTATE_H__