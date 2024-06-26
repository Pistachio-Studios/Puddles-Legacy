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
    Timer attackTimer;

public:
    EnemyBossDistanceAttackState(SString name) : State(name) {}
    inline void Enter() override
    {

        enemyboss = StateMachineReference->owner;
    }
    inline void Update(float dt) override
    {

        app->render->DrawTexture(enemyboss->bossDistanceAttack.texture, enemyboss->position.x - 60, enemyboss->position.y - 150, &enemyboss->bossDistanceAttack.GetCurrentFrame(), 1.0f, enemyboss->pbody->body->GetAngle() * RADTODEG, 1.0f, 1.0f, enemyboss->flip);
        enemyboss->bossDistanceAttack.Update(dt);

        if (enemyboss->bossDistanceAttack.GetCurrentFrameCount() == 2 && attackTimer.ReadMSec()>500) {
            enemyboss->justShot = true;
            enemyboss->shootBullet();
            attackTimer.Start();
        }

        if (enemyboss->bossDistanceAttack.GetCurrentFrameCount() >= 4) {
            enemyboss->attackTimer.Start();
            StateMachineReference->ChangeState("move");
        }
    }
    inline void Exit() override
    {
    }
};
#endif // __ENEMYBOSSDISTANCEATTACKSTATE_H__