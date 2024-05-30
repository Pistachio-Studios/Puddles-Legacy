#ifndef __WASPMOVESTATE_H__
#define __WASPMOVESTATE_H__

#include "Gameplay/Entities/Enemies/Wasp.h"
#include "Utils/State.h"
#include "Utils/SString.h"
#include "Utils/Defs.h"
#include "Gameplay/Entities/Player.h"

class Player;
class WaspMoveState : public State<Wasp> {
private:
    Wasp* wasp;
    Player* player;

public:
    WaspMoveState(SString name) : State(name) {}
    inline void Enter() override
    {
        LOG("WaspMoveState Enter()");

        wasp = StateMachineReference->owner;
    }
    inline void Update(float dt) override
    {
        LOG("WaspMoveState Update()");

        player = app->entityManager->GetPlayerEntity();

        //Animation
        app->render->DrawTexture(wasp->waspMove.texture, wasp->position.x - 120, wasp->position.y - 130, &wasp->waspMove.GetCurrentFrame());
        wasp->waspMove.Update(dt);

        wasp->pathfindingMovement(dt);
        if (PIXEL_TO_METERS(player->position.DistanceTo(wasp->position)) < 100.0f) {
            if (wasp->attackTimer.ReadSec() >= 2)
            {
                StateMachineReference->ChangeState("attack");
            }
        }
        else if ((PIXEL_TO_METERS(player->position.DistanceTo(wasp->position)) > 10.0f)) {
            //Wasp->moveToSpawnPoint();
            wasp->StopMoving();
            StateMachineReference->ChangeState("idle");
        }
    }
    inline void Exit() override
    {
        LOG("WaspMoveState Exit()");
    }
};
#endif // __WASPMOVESTATE_H__