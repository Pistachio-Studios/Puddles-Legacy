#ifndef __ENEMYBOSSBODYATTACKSTATE_H__
#define __ENEMYBOSSBODYATTACKSTATE_H__

#include "Gameplay/Entities/Enemies/EnemyBoss.h"
#include "Utils/State.h"
#include "Utils/SString.h"
#include "Utils/Defs.h"
#include "Gameplay/Entities/Player.h"

class Player;
class EnemyBossBodyAttackState : public State<EnemyBoss> {
private:
    EnemyBoss* enemyboss;
    Player* player;

public:
    EnemyBossBodyAttackState(SString name) : State(name) {}
    inline void Enter() override
    {
        LOG("EnemyBossBodyAttackState Enter()");

        enemyboss = StateMachineReference->owner;
    }
    inline void Update(float dt) override
    {
        LOG("EnemyBossBodyAttackState Update()");

        player = app->entityManager->GetPlayerEntity();

        app->render->DrawTexture(enemyboss->bossBodyAttack.texture, enemyboss->position.x - 100, enemyboss->position.y - 150, &enemyboss->bossBodyAttack.GetCurrentFrame(), 1.0f, enemyboss->pbody->body->GetAngle() * RADTODEG, enemyboss->flip);
        enemyboss->bossBodyAttack.Update(dt);

        if (PIXEL_TO_METERS(player->position.DistanceTo(enemyboss->position)) < 2.0f) {
            player->vida -= enemyboss->dano;
        }

        if (enemyboss->bossBodyAttack.GetCurrentFrameCount() >= 8) {
            enemyboss->attackTimer.Start();
            StateMachineReference->ChangeState("move");
        }
    }
    inline void Exit() override
    {
        LOG("EnemyBossBodyAttackState Exit()");
    }
};
#endif // __ENEMYBOSSBODYATTACKSTATE_H__