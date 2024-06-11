#ifndef __WASPPARALYZEDSTATE_H__
#define __WASPPARALYZEDSTATE_H__

#include "Gameplay/Entities/Enemies/Wasp.h"
#include "Utils/State.h"
#include "Utils/SString.h"
#include "Utils/Defs.h"
#include "Utils/Timer.h"

class Player;
class WaspParalyzedState : public State<Wasp> {
private:
    Wasp* wasp;
    Player* player;

    Timer timer;

public:
    WaspParalyzedState(SString name) : State(name) {}
    inline void Enter() override
    {
        wasp = StateMachineReference->owner;
        timer.Start();
    }
    inline void Update(float dt) override
    {
        //Animation
        app->render->DrawTexture(wasp->waspIdle.texture, wasp->position.x - 60, wasp->position.y - 150, &wasp->waspIdle.GetCurrentFrame(), 1.0f, wasp->pbody->body->GetAngle() * RADTODEG, 1.0f, 1.0f, wasp->flip);
        wasp->waspIdle.Update(dt);

        wasp->paralyzedParticles->emiting = true;

        player = app->entityManager->GetPlayerEntity();

        if (timer.ReadSec() >= 3.0f) {
            StateMachineReference->ChangeState("move");
        }
    }
    inline void Exit() override
    {
    }
};
#endif // __WASPPARALYZEDSTATE_H__