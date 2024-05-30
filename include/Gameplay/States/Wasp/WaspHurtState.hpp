#ifndef __WASPHURTSTATE_H__
#define __WASPHURTSTATE_H__

#include "Gameplay/Entities/Enemies/Wasp.h"
#include "Utils/State.h"
#include "Utils/SString.h"
#include "Utils/Defs.h"

class WaspHurtState : public State<Wasp> {
private:
    Wasp* wasp;

public:
    WaspHurtState(SString name) : State(name) {}
    inline void Enter() override
    {
        LOG("WaspHurtState Enter()");

        wasp = StateMachineReference->owner;
    }
    inline void Update(float dt) override
    {
        LOG("WaspHurtState Update()");

        //Animation
        app->render->DrawTexture(wasp->waspDamage.texture, wasp->position.x, wasp->position.y, &wasp->waspDamage.GetCurrentFrame());
        wasp->waspDamage.Update(dt);

        //Wasp->invencible = true;
        //if (Wasp->currentAnimation->HasFinished()) {
        //    Wasp->hurtAnim.Reset();
        //    Wasp->hurtAnim.ResetLoopCount();
        //    Wasp->invencible = false;
        //    StateMachineReference->ChangeState("idle");
        //}
    }
    inline void Exit() override
    {
        LOG("WaspHurtState Exit()");
    }
};
#endif // __WASPHURTSTATE_H__