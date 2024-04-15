#ifndef __ENEMYBOSSDEADSTATE_H__
#define __ENEMYBOSSDEADSTATE_H__

#include "Gameplay/Entities/Enemies/EnemyBoss.h"
#include "Utils/State.h"
#include "Utils/SString.h"
#include "Utils/Defs.h"

class EnemyBossDeadState : public State<EnemyBoss> {
private:
    EnemyBoss* enemyboss;

public:
    EnemyBossDeadState(SString name) : State(name) {}
    inline void Enter() override
    {
        LOG("EnemyBossHurtState Enter()");

        enemyboss = StateMachineReference->owner;
    }
    inline void Update(float dt) override
    {
        LOG("EnemyBossDeadState Update()");

        if (enemyboss->reviveTimer.ReadSec() >= 5)
        {
            StateMachineReference->ChangeState("idle");
            //enemyboss->health = 5;
        }
    }
    inline void Exit() override
    {
        LOG("EnemyBossDeadState Exit()");
    }
};
#endif // __ENEMYBOSSDEADSTATE_H__