#ifndef __FLYINGENEMYHURTSTATE_H__
#define __FLYINGENEMYHURTSTATE_H__

#include "Gameplay/Entities/Enemies/FlyingEnemy.h"
#include "Utils/State.h"
#include "Utils/SString.h"
#include "Utils/Defs.h"

class FlyingEnemyHurtState : public State<FlyingEnemy> {
private:
    FlyingEnemy* flyingenemy;

public:
    FlyingEnemyHurtState(SString name) : State(name) {}
    inline void Enter() override
    {
        LOG("FlingEnemyHurtState Enter()");

        flyingenemy = StateMachineReference->owner;
    }
    inline void Update(float dt) override
    {
        LOG("FlyingEnemyHurtState Update()");

        //enemyboss->invencible = true;
        //if (enemyboss->currentAnimation->HasFinished()) {
        //    enemyboss->hurtAnim.Reset();
        //    enemyboss->hurtAnim.ResetLoopCount();
        //    enemyboss->invencible = false;
        //    StateMachineReference->ChangeState("idle");
        //}
    }
    inline void Exit() override
    {
        LOG("FlyingEnemyHurtState Exit()");
    }
};
#endif // __FLYINGENEMYHURTSTATE_H__