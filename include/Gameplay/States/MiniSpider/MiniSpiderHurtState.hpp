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

        minispider = StateMachineReference->owner;
    }
    inline void Update(float dt) override
    {

        //Animation
        app->render->DrawTexture(minispider->spiderDamage.texture, minispider->position.x - 60, minispider->position.y - 60, &minispider->spiderDamage.GetCurrentFrame(), 1.0f, minispider->pbody->body->GetAngle() * RADTODEG, 0.5f, 1.0f, minispider->flip);
        minispider->spiderDamage.Update(dt);

        if (minispider->spiderDamage.GetCurrentFrameCount() >= 7) {
            if (app->entityManager->GetPlayerEntity()->paralysis) {
				// 10% chance to paralyze
				if (rand() % 100 < app->entityManager->GetPlayerEntity()->paralysisChance) {
					// TODO add paralysis effect
					StateMachineReference->ChangeState("paralyzed");
					LOG("MiniSpider Paralyzed!");
				}
			} else {
                StateMachineReference->ChangeState("idle");
            }
        }
    }
    inline void Exit() override
    {
    }
};
#endif // __MINISPIDERHURTSTATE_H__