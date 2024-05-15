#ifndef __CENTIPIDEENEMYHURTSTATE_H__
#define __CENTIPIDEENEMYHURTSTATE_H__

#include "Gameplay/Entities/Enemies/CentipideEnemy.h"
#include "Utils/State.h"
#include "Utils/SString.h"
#include "Utils/Defs.h"

class CentipideEnemyHurtState : public State<CentipideEnemy> {
private:
    CentipideEnemy* CentipideEnemy;

public:
    CentipideEnemyHurtState(SString name) : State(name) {}
    inline void Enter() override
    {
        LOG("CentipideEnemyHurtState Enter()");

        CentipideEnemy = StateMachineReference->owner;
    }
    inline void Update(float dt) override
    {
        LOG("CentipideEnemyHurtState Update()");

        //CentipideEnemy->invencible = true;
        //if (CentipideEnemy->currentAnimation->HasFinished()) {
        //    CentipideEnemy->hurtAnim.Reset();
        //    CentipideEnemy->hurtAnim.ResetLoopCount();
        //    CentipideEnemy->invencible = false;
        //    StateMachineReference->ChangeState("idle");
        //}
    }
    inline void Exit() override
    {
        LOG("CentipideEnemyHurtState Exit()");
    }
};
#endif // __ECENTIPIDEENEMYHURTSTATE_H__