#ifndef __CENTIPIDEENEMYDEADSTATE_H__
#define __CENTIPIDEENEMYDEADSTATE_H__

#include "Gameplay/Entities/Enemies/CentipideEnemy.h"
#include "Utils/State.h"
#include "Utils/SString.h"
#include "Utils/Defs.h"

class CentipideEnemyDeadState : public State<CentipideEnemy> {
private:
    CentipideEnemy* CentipideEnemy;

public:
    CentipideEnemyDeadState(SString name) : State(name) {}
    inline void Enter() override
    {
        CentipideEnemy = StateMachineReference->owner;
    }
    inline void Update(float dt) override
    {
        //if (CentipideEnemy->reviveTimer.ReadSec() >= 5)
        //{
        //    StateMachineReference->ChangeState("idle");
        //    //CentipideEnemy->health = 5;
        //}
    }
    inline void Exit() override
    {
    }
};
#endif // __CENTIPIDEENEMYDEADSTATE_H__