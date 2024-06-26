#ifndef __MINISPIDERMOVESTATE_H__
#define __MINISPIDERMOVESTATE_H__

#include "Gameplay/Entities/Enemies/MiniSpider.h"
#include "Utils/State.h"
#include "Utils/SString.h"
#include "Utils/Defs.h"
#include "Gameplay/Entities/Player.h"

class Player;
class MiniSpiderMoveState : public State<MiniSpider> {
private:
    MiniSpider* minispider;
    Player* player;

public:
    MiniSpiderMoveState(SString name) : State(name) {}
    inline void Enter() override
    {

        minispider = StateMachineReference->owner;
    }
    inline void Update(float dt) override
    {

        player = app->entityManager->GetPlayerEntity();

        //Animation
        app->render->DrawTexture(minispider->spiderMove.texture, minispider->position.x - 60, minispider->position.y - 60, &minispider->spiderMove.GetCurrentFrame(), 1.0f, minispider->pbody->body->GetAngle() * RADTODEG, 0.5f, 1.0f, minispider->flip);
        minispider->spiderMove.Update(dt);

        minispider->pathfindingMovement(dt);

        if (PIXEL_TO_METERS(player->position.DistanceTo(minispider->position)) < 2.0f) {
            if (minispider->attackTimer.ReadSec() >= 1)
            {
                minispider->spiderAttack.Reset();
                StateMachineReference->ChangeState("attack");
            }
        }

        if (PIXEL_TO_METERS(player->position.DistanceTo(minispider->position)) > 7.0f) {
            //minispider->moveToSpawnPoint();
            minispider->StopMoving();
            StateMachineReference->ChangeState("idle");
        }
    }
    inline void Exit() override
    {
    }
};
#endif // __MINISPIDERMOVESTATE_H__