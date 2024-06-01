#ifndef __ENEMYBOSSDISTANCEATTACKSTATE_H__
#define __ENEMYBOSSDISTANCEATTACKSTATE_H__

#include "Gameplay/Entities/Enemies/EnemyBoss.h"
#include "Utils/State.h"
#include "Utils/SString.h"
#include "Utils/Defs.h"
#include "Gameplay/Entities/Player.h"

class Player;
class EnemyBossDistanceAttackState : public State<EnemyBoss> {
private:
    EnemyBoss* enemyboss;
    Player* player;

public:
    EnemyBossDistanceAttackState(SString name) : State(name) {}
    inline void Enter() override
    {
        LOG("EnemyBossDistanceAttackState Enter()");

        enemyboss = StateMachineReference->owner;
    }
    inline void Update(float dt) override
    {
        LOG("EnemyBossDistanceAttackState Update()");

        app->render->DrawTexture(enemyboss->bossDistanceAttack.texture, enemyboss->position.x - 100, enemyboss->position.y - 150, &enemyboss->bossDistanceAttack.GetCurrentFrame(), 1.0f, enemyboss->pbody->body->GetAngle() * RADTODEG, enemyboss->flip);
        enemyboss->bossDistanceAttack.Update(dt);

        /*b2Vec2 attackDirection = { (float32)player->position.x - enemyboss->position.x, (float32)player->position.y - enemyboss->position.y };
        attackDirection.Normalize();

        b2Vec2 attackImpulse = { attackDirection.x, attackDirection.y };

        enemyboss->pbody->body->ApplyLinearImpulse(attackImpulse, enemyboss->pbody->body->GetWorldCenter(), true);*/
        //app->render->DrawRectangle({ enemyboss->position.x - 5, enemyboss->position.y - 2, 36, 36 }, 0, 50, 255);

        if (enemyboss->bossDistanceAttack.GetCurrentFrameCount() >= 4) {
            enemyboss->attackTimer.Start();
            StateMachineReference->ChangeState("move");
        }
    }
    inline void Exit() override
    {
        LOG("EnemyBossDistanceAttackState Exit()");
    }
};
#endif // __ENEMYBOSSDISTANCEATTACKSTATE_H__