#ifndef __WASPATTACKSTATE_H__
#define __WASPATTACKSTATE_H__

#include "Gameplay/Entities/Enemies/Wasp.h"
#include "Utils/State.h"
#include "Utils/SString.h"
#include "Utils/Defs.h"
#include "Gameplay/Entities/Player.h"

class Player;
class WaspAttackState : public State<Wasp> {
private:
    Wasp* wasp;
    Player* player;

public:
    WaspAttackState(SString name) : State(name) {}
    inline void Enter() override
    {
        LOG("WaspAttackState Enter()");

        wasp = StateMachineReference->owner;
    }
    inline void Update(float dt) override
    {
        LOG("WaspAttackState Update()");

        player = app->entityManager->GetPlayerEntity();

        //Animation
        app->render->DrawTexture(wasp->waspAttack.texture, wasp->position.x-120, wasp->position.y-130, &wasp->waspAttack.GetCurrentFrame(), 1.0f, wasp->pbody->body->GetAngle() * RADTODEG, wasp->flip);
        wasp->waspAttack.Update(dt);

        if (wasp->waspAttack.GetCurrentFrameCount() == 0) {
            wasp->force = wasp->calculateForce();
        }

        if (wasp->waspAttack.GetCurrentFrameCount() >= 14 && wasp->waspAttack.GetCurrentFrameCount() <= 17) {
            wasp->attackMovement(wasp->force);
            if (PIXEL_TO_METERS(player->position.DistanceTo(wasp->position)) < 2.0f) {
                player->vida -= wasp->dano;
                wasp->vida += wasp->dano;
            }
        }

        if (wasp->waspAttack.GetCurrentFrameCount() >= 24) {
            wasp->attackTimer.Start();
            StateMachineReference->ChangeState("idle");
        }

    }
    inline void Exit() override
    {
        LOG("WaspAttackState Exit()");
    }
};
#endif // __WASPATTACKSTATE_H__