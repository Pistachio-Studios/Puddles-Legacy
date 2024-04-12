#ifndef __ENEMYBOSSATTACKSTATE_H__
#define __ENEMYBOSSATTACKSTATE_H__

#include "Gameplay/Entities/Enemies/EnemyBoss.h"
#include "Utils/State.h"
#include "Utils/SString.h"
#include "Utils/Defs.h"

class EnemyBossAttackState : public State<EnemyBoss> {
private:
    EnemyBoss* enemyboss;

public:
    EnemyBossAttackState(SString name) : State(name) {}
    inline void Enter() override
    {
        LOG("EnemyBossAttackState Enter()");

        enemyboss = StateMachineReference->owner;
    }
    inline void Update(float dt) override
    {
        LOG("EnemyBossAttackState Update()");

        //b2Vec2 attackDirection = { (float32)app->scene->player->position.x - dogEnemy->position.x, (float32)app->scene->player->position.y - dogEnemy->position.y };
        //attackDirection.Normalize();

        //b2Vec2 attackImpulse = { attackDirection.x / 4, attackDirection.y / 4 };

        //dogEnemy->pbody->body->ApplyLinearImpulse(attackImpulse, dogEnemy->pbody->body->GetWorldCenter(), true);

        //dogEnemy->attackTimer.Start();

        //StateMachineReference->ChangeState("move");
    }
    inline void Exit() override
    {
        LOG("EnemyBossAttackState Exit()");
    }
};
#endif // __ENEMYBOSSATTACKSTATE_H__