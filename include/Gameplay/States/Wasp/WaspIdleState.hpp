#ifndef __WASPIDLESTATE_H__
#define __WASPIDLESTATE_H__

#include "Gameplay/Entities/Enemies/Wasp.h"
#include "Utils/State.h"
#include "Utils/SString.h"
#include "Utils/Defs.h"
#include "Core/Animation.h"

class Player;
class WaspIdleState : public State<Wasp> {
private:
    Wasp* wasp;
    Player* player;

public:
    WaspIdleState(SString name) : State(name) {}
    inline void Enter() override
    {
        LOG("WaspIdleState Enter()");

        wasp = StateMachineReference->owner;
    }
    inline void Update(float dt) override
    {
        LOG("WaspIdleState Update()");

        //Animation
        app->render->DrawTexture(wasp->waspIdle.texture, wasp->position.x - 88, wasp->position.y - 104, &wasp->waspIdle.GetCurrentFrame(), 1.0f, wasp->pbody->body->GetAngle() * RADTODEG, 0.8f, 1.0f, wasp->flip);
        wasp->waspIdle.Update(dt);

        player = app->entityManager->GetPlayerEntity();

        if (PIXEL_TO_METERS(player->position.DistanceTo(wasp->position)) < 2.0f) {
            if (wasp->attackTimer.ReadSec() >= 1)
            {
                wasp->waspAttack.Reset();
                StateMachineReference->ChangeState("attack");
            }
        }

        if (PIXEL_TO_METERS(player->position.DistanceTo(wasp->position)) < 7.0f && PIXEL_TO_METERS(player->position.DistanceTo(wasp->position)) > 2.0f)
        {
            StateMachineReference->ChangeState("move");
            // AUDIO DONE dog idle
            //app->audio->PlayFx(dogEnemy->dogBark);
        }
    }
    inline void Exit() override
    {
        LOG("WaspIdleState Exit()");
    }
};
#endif // __WASPIDLESTATE_H__