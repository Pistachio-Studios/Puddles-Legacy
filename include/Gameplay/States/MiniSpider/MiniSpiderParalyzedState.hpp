#ifndef __MINISPIDERPARALYZEDSTATE_H__
#define __MINISPIDERPARALYZEDSTATE_H__

#include "Gameplay/Entities/Enemies/MiniSpider.h"
#include "Utils/State.h"
#include "Utils/SString.h"
#include "Utils/Defs.h"
#include "Utils/Timer.h"

class Player;
class MiniSpiderParalyzedState : public State<MiniSpider> {
private:
    MiniSpider* minispider;
    Player* player;

    Timer timer;

public:
    MiniSpiderParalyzedState(SString name) : State(name) {}
    inline void Enter() override
    {
        minispider = StateMachineReference->owner;
        timer.Start();
    }
    inline void Update(float dt) override
    {
        // Animation
        app->render->DrawTexture(minispider->spiderIdle.texture, minispider->position.x - 60, minispider->position.y - 60, &minispider->spiderIdle.GetCurrentFrame(), 1.0f, minispider->pbody->body->GetAngle() * RADTODEG, 0.5f, 1.0f, minispider->flip);
        minispider->spiderIdle.Update(dt);

        minispider->paralyzedParticles->emiting = true;

        player = app->entityManager->GetPlayerEntity();

        if (timer.ReadSec() >= 3.0f) {
            StateMachineReference->ChangeState("move");
        }
    }
    inline void Exit() override
    {
    }
};
#endif // __MINISPIDERPARALYZEDSTATE_H__