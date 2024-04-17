#ifndef __FLYINGENEMYATTACKSTATE_H__
#define __FLYINGENEMYATTACKSTATE_H__

#include "Gameplay/Entities/Enemies/FlyingEnemy.h"
#include "Utils/State.h"
#include "Utils/SString.h"
#include "Utils/Defs.h"
#include "Gameplay/Entities/Player.h"

class Player;
class FlyingEnemyAttackState : public State<FlyingEnemy> {
private:
    FlyingEnemy* flyingenemy;
    Player* player;

public:
    FlyingEnemyAttackState(SString name) : State(name) {}
    inline void Enter() override
    {
        LOG("FlyingEnemyAttackState Enter()");

        flyingenemy = StateMachineReference->owner;
    }
    inline void Update(float dt) override
    {
        LOG("FlyingEnemyAttackState Update()");

        /*b2Vec2 attackDirection = { (float32)player->position.x - enemyboss->position.x, (float32)player->position.y - enemyboss->position.y };
        attackDirection.Normalize();

        b2Vec2 attackImpulse = { attackDirection.x, attackDirection.y };

        enemyboss->pbody->body->ApplyLinearImpulse(attackImpulse, enemyboss->pbody->body->GetWorldCenter(), true);*/
        app->render->DrawRectangle({ flyingenemy->position.x - 5, flyingenemy->position.y - 2, 36, 36 }, 0, 50, 255);

        flyingenemy->attackTimer.Start();
        StateMachineReference->ChangeState("move");
    }
    inline void Exit() override
    {
        LOG("FlyingEnemyAttackState Exit()");
    }
};
#endif // __FLYINGENEMYATTACKSTATE_H__