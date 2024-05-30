#ifndef __MINISPIDERDEADSTATE_H__
#define __MINISPIDERDEADSTATE_H__

#include "Gameplay/Entities/Enemies/MiniSpider.h"
#include "Utils/State.h"
#include "Utils/SString.h"
#include "Utils/Defs.h"

class MiniSpiderDeadState : public State<MiniSpider> {
private:
    MiniSpider* minispider;

public:
    MiniSpiderDeadState(SString name) : State(name) {}
    inline void Enter() override
    {
        LOG("MiniSpiderDeadState Enter()");

        minispider = StateMachineReference->owner;
    }
    inline void Update(float dt) override
    {
        LOG("MiniSpiderDeadState Update()");

        //Animation
        app->render->DrawTexture(minispider->spiderDeath.texture, minispider->position.x - 100, minispider->position.y - 150, &minispider->spiderDeath.GetCurrentFrame());
        minispider->spiderDeath.Update(dt);

        //if (minispider->reviveTimer.ReadSec() >= 5)
        //{
        //    StateMachineReference->ChangeState("idle");
        //    //enemyboss->health = 5;
        //}
    }
    inline void Exit() override
    {
        LOG("MiniSpiderDeadState Exit()");
    }
};
#endif // __MINISPIDERDEADSTATE_H__