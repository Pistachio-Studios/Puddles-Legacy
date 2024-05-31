#ifndef __WASPDEADSTATE_H__
#define __WASPDEADSTATE_H__

#include "Gameplay/Entities/Enemies/Wasp.h"
#include "Utils/State.h"
#include "Utils/SString.h"
#include "Utils/Defs.h"

class WaspDeadState : public State<Wasp> {
private:
    Wasp* wasp;

public:
    WaspDeadState(SString name) : State(name) {}
    inline void Enter() override
    {
        LOG("WaspHurtState Enter()");

        wasp = StateMachineReference->owner;
    }
    inline void Update(float dt) override
    {
        LOG("WaspDeadState Update()");

        //Animation
        app->render->DrawTexture(wasp->waspDeath.texture, wasp->position.x-120, wasp->position.y-130, &wasp->waspDeath.GetCurrentFrame(), 1.0f, wasp->pbody->body->GetAngle() * RADTODEG, wasp->flip);
        wasp->waspDeath.Update(dt);

        if (wasp->waspDeath.GetCurrentFrameCount() >= 15) {
            app->physics->DestroyBody(wasp->pbody);
            wasp->Disable();
        }

    }
    inline void Exit() override
    {
        LOG("WaspDeadState Exit()");
    }
};
#endif // __WASPDEADSTATE_H__