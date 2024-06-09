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

        wasp = StateMachineReference->owner;
    }
    inline void Update(float dt) override
    {

        player = app->entityManager->GetPlayerEntity();

        //Animation
        app->render->DrawTexture(wasp->waspAttack.texture, wasp->position.x - 88, wasp->position.y - 104, &wasp->waspAttack.GetCurrentFrame(), 1.0f, wasp->pbody->body->GetAngle() * RADTODEG, 0.8f, 1.0f, wasp->flip);
        wasp->waspAttack.Update(dt);

        if (wasp->waspAttack.GetCurrentFrameCount() == 0) {
            wasp->force = wasp->calculateForce();
        }

        //Attack
        if (wasp->waspAttack.GetCurrentFrameCount() >= 14 && wasp->waspAttack.GetCurrentFrameCount() <= 17) {
            wasp->attackMovement(wasp->force);
            if (wasp->isTouchingPlayer && wasp->waspAttack.GetCurrentFrameCount() == 15) {
                player->vida -= wasp->dano;
                wasp->vida += wasp->dano;
                if (wasp->vida > 15.0f) wasp->vida = 15;
            }
        }

        //Sound
        if (wasp->waspAttack.GetCurrentFrameCount() == 9) {
            app->audio->PlayFx(wasp->attackFx);
        }

        if (wasp->waspAttack.GetCurrentFrameCount() >= 24) {
            wasp->attackTimer.Start();
            StateMachineReference->ChangeState("idle");
        }

    }
    inline void Exit() override
    {
    }
};
#endif // __WASPATTACKSTATE_H__