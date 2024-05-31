#ifndef __MINISPIDERHURTSTATE_H__
#define __MINISPIDERHURTSTATE_H__

#include "Gameplay/Entities/Enemies/MiniSpider.h"
#include "Utils/State.h"
#include "Utils/SString.h"
#include "Utils/Defs.h"

class MiniSpiderHurtState : public State<MiniSpider> {
private:
    MiniSpider* minispider;

public:
    MiniSpiderHurtState(SString name) : State(name) {}
    inline void Enter() override
    {
        LOG("FlingEnemyHurtState Enter()");

        minispider = StateMachineReference->owner;
    }
    inline void Update(float dt) override
    {
        LOG("MiniSpiderHurtState Update()");

        //Animation
        app->render->DrawTexture(minispider->spiderDamage.texture, minispider->position.x - 100, minispider->position.y - 150, &minispider->spiderDamage.GetCurrentFrame(), 1.0f, minispider->pbody->body->GetAngle() * RADTODEG, minispider->flip);
        minispider->spiderDamage.Update(dt);

        if (minispider->spiderDamage.GetCurrentFrameCount() >= 7) {
            StateMachineReference->ChangeState("idle");
        }
    }
    inline void Exit() override
    {
        LOG("MiniSpiderHurtState Exit()");
    }
};
#endif // __MINISPIDERHURTSTATE_H__