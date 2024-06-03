#ifndef __CENTIPIDEENEMYATTACKSTATE_H__
#define __CENTIPIDEENEMYATTACKSTATE_H__

#include "Gameplay/Entities/Enemies/CentipideEnemy.h"
#include "Utils/State.h"
#include "Utils/SString.h"
#include "Utils/Defs.h"
#include "Gameplay/Entities/Player.h"

class Player;
class CentipideEnemyAttackState : public State<CentipideEnemy> {
private:
    CentipideEnemy* CentipideEnemy;
    Player* player;

public:
    CentipideEnemyAttackState(SString name) : State(name) {}
    inline void Enter() override
    {


        CentipideEnemy = StateMachineReference->owner;
    }
    inline void Update(float dt) override
    {


        /*b2Vec2 attackDirection = { (float32)player->position.x - CentipideEnemy->position.x, (float32)player->position.y - CentipideEnemy->position.y };
        attackDirection.Normalize(); 

        b2Vec2 attackImpulse = { attackDirection.x, attackDirection.y }; 

        CentipideEnemy->pbody->body->ApplyLinearImpulse(attackImpulse, CentipideEnemy->pbody->body->GetWorldCenter(), true);*/
        app->render->DrawRectangle({ CentipideEnemy->position.x - 5, CentipideEnemy->position.y - 2, 36, 36 }, 0, 50, 255);

        CentipideEnemy->attackTimer.Start(); 
        StateMachineReference->ChangeState("move"); 
    }
    inline void Exit() override
    {

    }
};
#endif // __CENTIPIDEENEMYATTACKSTATE_H__