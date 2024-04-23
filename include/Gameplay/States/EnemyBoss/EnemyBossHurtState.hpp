#ifndef __ENEMYBOSSHURTSTATE_H__
#define __ENEMYBOSSHURTSTATE_H__

#include "Gameplay/Entities/Enemies/EnemyBoss.h"
#include "Utils/State.h"
#include "Utils/SString.h"
#include "Utils/Defs.h"

class EnemyBossHurtState : public State<EnemyBoss> {
private:
    EnemyBoss* enemyboss;

public:
    EnemyBossHurtState(SString name) : State(name) {}
    inline void Enter() override
    {
        LOG("EnemyBossHurtState Enter()");

        enemyboss = StateMachineReference->owner;
    }
    inline void Update(float dt) override
    {
        LOG("EnemyBossHurtState Update()");

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
        LOG("EnemyBossHurtState Exit()");
    }
};
#endif // __ENEMYBOSSHURTSTATE_H__