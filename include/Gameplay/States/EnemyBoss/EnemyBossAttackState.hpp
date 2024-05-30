#ifndef __ENEMYBOSSATTACKSTATE_H__
#define __ENEMYBOSSATTACKSTATE_H__

#include "Gameplay/Entities/Enemies/EnemyBoss.h"
#include "Utils/State.h"
#include "Utils/SString.h"
#include "Utils/Defs.h"
#include "Gameplay/Entities/Player.h"

class Player;
class EnemyBossAttackState : public State<EnemyBoss> {
private:
    EnemyBoss* enemyboss;
    Player* player;

public:
    EnemyBossAttackState(SString name) : State(name) {}
    inline void Enter() override
    {

        enemyboss = StateMachineReference->owner;
    }
    inline void Update(float dt) override
    {

        /*b2Vec2 attackDirection = { (float32)player->position.x - enemyboss->position.x, (float32)player->position.y - enemyboss->position.y };
        attackDirection.Normalize(); 

        b2Vec2 attackImpulse = { attackDirection.x, attackDirection.y }; 

        enemyboss->pbody->body->ApplyLinearImpulse(attackImpulse, enemyboss->pbody->body->GetWorldCenter(), true);*/
        app->render->DrawRectangle({ enemyboss->position.x - 5, enemyboss->position.y - 2, 36, 36 }, 0, 50, 255);

        enemyboss->attackTimer.Start(); 
        StateMachineReference->ChangeState("move"); 
    }
    inline void Exit() override
    {
    }
};
#endif // __ENEMYBOSSATTACKSTATE_H__