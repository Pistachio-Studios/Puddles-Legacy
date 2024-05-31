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

        /*b2Vec2 attackDirection = { (float32)player->position.x - Wasp->position.x, (float32)player->position.y - Wasp->position.y };
        attackDirection.Normalize(); 

        b2Vec2 attackImpulse = { attackDirection.x, attackDirection.y }; 

        Wasp->pbody->body->ApplyLinearImpulse(attackImpulse, Wasp->pbody->body->GetWorldCenter(), true);*/
        //app->render->DrawRectangle({ wasp->position.x - 5, wasp->position.y - 2, 36, 36 }, 0, 50, 255);

        //Animation
        app->render->DrawTexture(wasp->waspAttack.texture, wasp->position.x-120, wasp->position.y-130, &wasp->waspAttack.GetCurrentFrame(), 1.0f, wasp->pbody->body->GetAngle() * RADTODEG, wasp->flip);
        wasp->waspAttack.Update(dt);

        if (wasp->waspAttack.GetCurrentFrameCount() >= 14 && wasp->waspAttack.GetCurrentFrameCount() <= 17) {
            wasp->attackMovement();
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